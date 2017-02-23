/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#include "../../../common/module.h"
#include "../../../common/event.h"
#include "../../../common/error_debug.h"
#include "../../../common/alloc.h"

#include "om_tcp.h"

#define NX_LOGMODULE NX_LOGMODULE_MODULE

#define OM_TCP_DEFAULT_PORT 514
#define OM_TCP_DEFAULT_CONNECT_TIMEOUT (APR_USEC_PER_SEC * 30)
#define OM_TCP_MAX_RECONNECT_INTERVAL 200


static void om_tcp_add_reconnect_event(nx_module_t *module)
{
    nx_event_t *event;
    nx_om_tcp_conf_t *omconf;

    omconf = (nx_om_tcp_conf_t *) module->config;

    if ( omconf->reconnect == 0 )
    {
	omconf->reconnect = 1;
    }
    else
    {
	omconf->reconnect *= 2;
    }
    if ( omconf->reconnect > OM_TCP_MAX_RECONNECT_INTERVAL )
    { // limit
	omconf->reconnect = OM_TCP_MAX_RECONNECT_INTERVAL;
    }

    log_info("reconnecting in %d seconds", omconf->reconnect);
    
    event = nx_event_new();
    event->module = module;
    event->delayed = TRUE;
    event->type = NX_EVENT_RECONNECT;
    event->time = apr_time_now() + APR_USEC_PER_SEC * omconf->reconnect;
    event->priority = module->priority;
    nx_event_add(event);
}



static void om_tcp_stop(nx_module_t *module)
{
    nx_om_tcp_conf_t *omconf;
    apr_pool_t *pool;

    ASSERT(module != NULL);

    log_debug("om_tcp_stop");
    ASSERT(module->config != NULL);
    omconf = (nx_om_tcp_conf_t *) module->config;

    if ( omconf->sock != NULL )
    {
	if ( omconf->connected == TRUE )
	{
	    log_debug("om_tcp closing socket");
	    nx_module_pollset_remove_socket(module, omconf->sock);
	}
	pool = apr_socket_pool_get(omconf->sock);
	apr_pool_destroy(pool);
	omconf->sock = NULL;
    }
    omconf->connected = FALSE;
}



static void om_tcp_write(nx_module_t *module)
{
    nx_om_tcp_conf_t *omconf;
    nx_logdata_t *logdata;
    apr_size_t nbytes;
    boolean done = FALSE;
    apr_status_t rv;

    ASSERT(module != NULL);

    log_debug("om_tcp_write");

    if ( nx_module_get_status(module) != NX_MODULE_STATUS_RUNNING )
    {
	log_debug("module %s not running, not writing any more data", module->name);
	return;
    }

    omconf = (nx_om_tcp_conf_t *) module->config;

    do
    {
	if ( module->output.buflen > 0 )
	{
	    nbytes = module->output.buflen;
	    ASSERT(omconf->sock != NULL);
	    if ( (rv = apr_socket_send(omconf->sock, module->output.buf + module->output.bufstart,
				       &nbytes)) != APR_SUCCESS )
	    {
		if ( (APR_STATUS_IS_EINPROGRESS(rv) == TRUE) ||
		     (APR_STATUS_IS_EAGAIN(rv) == TRUE) )
		{
		    nx_module_pollset_add_socket(module, omconf->sock, 
						 APR_POLLIN | APR_POLLOUT | APR_POLLHUP);
		    nx_module_add_poll_event(module);
		    done = TRUE;
		}
		else
		{
		    throw(rv, "om_tcp send failed");
		}
	    }
	    else
	    { // Sent OK
		log_debug("om_tcp sent %d bytes", (int) nbytes);
		if ( nbytes < module->output.buflen )
		{
		    log_debug("om_tcp sent less (%d) than requested (%d)", (int) nbytes,
			      (int) (module->output.buflen - module->output.bufstart));
		    done = TRUE;
		    nx_module_pollset_add_socket(module, omconf->sock, 
						 APR_POLLIN | APR_POLLOUT | APR_POLLHUP);
		    nx_module_add_poll_event(module);
		}
	    }
	    ASSERT(nbytes <= module->output.buflen);
	    module->output.bufstart += nbytes;
	    module->output.buflen -= nbytes;
	    if ( module->output.buflen == 0 )
	    { // all bytes have been sucessfully written
		module->output.bufstart = 0;
		nx_module_logqueue_pop(module, module->output.logdata);
		nx_logdata_free(module->output.logdata);
		module->output.logdata = NULL;
	    }
	}
	else
	{
	    nx_module_pollset_add_socket(module, omconf->sock, APR_POLLIN | APR_POLLHUP);
	}
	//log_info("output buflen: %d, bufstart: %d", (int) module->output.buflen, (int) module->output.bufstart);

	if ( module->output.buflen == 0 )
	{
	    if ( (logdata = nx_module_logqueue_peek(module)) != NULL )
	    {
		module->output.logdata = logdata;
		module->output.outputfunc->func(&(module->output),
						module->output.outputfunc->data);
	    }
	    else
	    {
		done = TRUE;
	    }
	}
    } while ( done != TRUE );
}



static void om_tcp_read(nx_module_t *module)
{
    nx_om_tcp_conf_t *omconf;
    apr_status_t rv;
    char buf[100];
    apr_size_t nbytes;
    
    // The server disconnected or sent something
    // We close the socket in either case
    ASSERT(module != NULL);

    omconf = (nx_om_tcp_conf_t *) module->config;

    log_debug("om_tcp read");

    if ( omconf->connected != TRUE )
    {
	return;
    }
    nbytes = sizeof(buf);
    ASSERT(omconf->sock != NULL);
    while ( (rv = apr_socket_recv(omconf->sock, buf, &nbytes)) == APR_SUCCESS )
    {
	log_error("om_tcp received data from remote end (got %d bytes)", (int) nbytes);
	nbytes = sizeof(buf);
    }
    if ( APR_STATUS_IS_EAGAIN(rv) )
    {
	nx_module_pollset_add_socket(module, omconf->sock, APR_POLLIN | APR_POLLHUP);
	nx_module_add_poll_event(module);
	return;
    }
    if ( rv != APR_SUCCESS )
    {
	throw(rv, "om_tcp detected a connection error");
    }
}



static void om_tcp_config(nx_module_t *module)
{
    const nx_directive_t *curr;
    nx_om_tcp_conf_t *omconf;
    unsigned int port;

    ASSERT(module->directives != NULL);
    curr = module->directives;

    omconf = apr_pcalloc(module->pool, sizeof(nx_om_tcp_conf_t));
    module->config = omconf;

    while ( curr != NULL )
    {
	if ( nx_module_common_keyword(curr->directive) == TRUE )
	{
	}
	else if ( strcasecmp(curr->directive, "host") == 0 )
	{
	    if ( omconf->host != NULL )
	    {
		nx_conf_error(curr, "host is already defined");
	    }
	    omconf->host = apr_pstrdup(module->pool, curr->args);
	}
	else if ( strcasecmp(curr->directive, "port") == 0 )
	{
	    if ( omconf->port != 0 )
	    {
		nx_conf_error(curr, "port is already defined");
	    }
	    if ( sscanf(curr->args, "%u", &port) != 1 )
	    {
		nx_conf_error(curr, "invalid port: %s", curr->args);
	    }
	    omconf->port = (apr_port_t) port;
	}
	else if ( strcasecmp(curr->directive, "Reconnect") == 0 )
	{
	    log_warn("The 'Reconnect' directive at %s:%d has been deprecated",
		     curr->filename, curr->line_num);
	}
	else if ( strcasecmp(curr->directive, "OutputType") == 0 )
	{
	    if ( module->output.outputfunc != NULL )
	    {
		nx_conf_error(curr, "OutputType is already defined");
	    }

	    if ( curr->args != NULL )
	    {
		module->output.outputfunc = nx_module_output_func_lookup(curr->args);
	    }
	    if ( module->output.outputfunc == NULL )
	    {
		nx_conf_error(curr, "Invalid OutputType '%s'", curr->args);
	    }
	}
	else
	{
	    nx_conf_error(curr, "invalid keyword: %s", curr->directive);
	}
	curr = curr->next;
    }

    if ( module->output.outputfunc == NULL )
    {
	module->output.outputfunc = nx_module_output_func_lookup("linebased");
    }
    ASSERT(module->output.outputfunc != NULL);

    if ( omconf->host == NULL )
    {
	nx_conf_error(module->directives, "Mandatory 'Host' parameter missing");
    }
    if ( omconf->port == 0 )
    {
	omconf->port = OM_TCP_DEFAULT_PORT;
    }

    omconf->connected = FALSE;
}



static void om_tcp_connect(nx_module_t *module)
{
    nx_om_tcp_conf_t *omconf;
    apr_sockaddr_t *sa;
    apr_pool_t *pool = NULL;
    int i;
    apr_status_t rv;

    ASSERT(module->config != NULL);

    omconf = (nx_om_tcp_conf_t *) module->config;

    omconf->connected = FALSE;

    pool = nx_pool_create_child(module->pool);

    CHECKERR_MSG(apr_sockaddr_info_get(&sa, omconf->host, APR_INET, omconf->port, 
				       0, pool),
		 "apr_sockaddr_info failed for %s:%d", omconf->host, omconf->port);
    CHECKERR_MSG(apr_socket_create(&(omconf->sock), sa->family, SOCK_STREAM,
				   APR_PROTO_TCP, pool), "couldn't create tcp socket");
    CHECKERR_MSG(apr_socket_opt_set(omconf->sock, APR_SO_NONBLOCK, 1),
		 "couldn't set SO_NONBLOCK on connecting socket");
    CHECKERR_MSG(apr_socket_timeout_set(omconf->sock, OM_TCP_DEFAULT_CONNECT_TIMEOUT),
		 "couldn't set socket timeout on connecting socket");
    
    log_info("connecting to %s:%d", omconf->host, omconf->port);
    
    for ( i = 0; i < 100; i++ )
    {
	rv = apr_socket_connect(omconf->sock, sa);
	if ( APR_STATUS_IS_EAGAIN(rv) )
	{
	    apr_sleep(100);
	}
	else
	{
	    break;
	}
    }
    CHECKERR_MSG(rv, "couldn't connect to tcp socket on %s:%d", omconf->host, omconf->port);
    omconf->connected = TRUE;
    omconf->reconnect = 0;
    
    CHECKERR_MSG(apr_socket_opt_set(omconf->sock, APR_SO_NONBLOCK, 1),
		 "couldn't set SO_NONBLOCK on tcp socket");
    CHECKERR_MSG(apr_socket_timeout_set(omconf->sock, 0),
		 "couldn't set socket timeout on tcp socket");
}



static void io_err_handler(nx_module_t *module, nx_exception_t *e) NORETURN;
static void io_err_handler(nx_module_t *module, nx_exception_t *e)
{
    ASSERT(e != NULL);
    ASSERT(module != NULL);

    nx_module_stop_self(module);
    om_tcp_stop(module);
    om_tcp_add_reconnect_event(module);
    rethrow(*e);

/*
    //default:
    nx_module_stop_self(module);
    om_tcp_stop(module);
    rethrow_msg(*e, "fatal connection error, reconnection will not be attempted (statuscode: %d)",
		e->code);
*/
}



static void om_tcp_start(nx_module_t *module)
{
    nx_om_tcp_conf_t *omconf;
    nx_exception_t e;

    ASSERT(module->config != NULL);

    omconf = (nx_om_tcp_conf_t *) module->config;

    try
    {
	om_tcp_connect(module);
    }
    catch(e)
    {
	io_err_handler(module, &e);
    }

    if ( omconf->connected == TRUE )
    {
	// POLLIN is to detect disconnection
	nx_module_pollset_add_socket(module, omconf->sock, APR_POLLIN | APR_POLLHUP);
	// write data after connection was established
	nx_module_data_available(module);
	nx_module_add_poll_event(module);
    }
}



static void om_tcp_init(nx_module_t *module)
{
    nx_module_pollset_init(module);
}



static void om_tcp_event(nx_module_t *module, nx_event_t *event)
{
    nx_exception_t e;

    ASSERT(event != NULL);

    switch ( event->type )
    {
        case NX_EVENT_RECONNECT:
	    nx_module_start_self(module);
	    break;
        case NX_EVENT_DISCONNECT:
	    nx_module_stop_self(module);
	    om_tcp_add_reconnect_event(module);
	    break;;
        case NX_EVENT_READ:
	    try
	    {
		om_tcp_read(module);
	    }
	    catch(e)
	    {
		io_err_handler(module, &e);
	    }
	    break;
        case NX_EVENT_WRITE:
	    //fallthrough
	case NX_EVENT_DATA_AVAILABLE:
	    try
	    {
		om_tcp_write(module);
	    }
	    catch(e)
	    {
		io_err_handler(module, &e);
	    }
	    break;
	case NX_EVENT_POLL:
	    if ( nx_module_get_status(module) == NX_MODULE_STATUS_RUNNING )
	    {
		nx_module_pollset_poll(module, FALSE);
	    }
	    break;
	default:
	    nx_panic("invalid event type: %d", event->type);
    }
}



NX_MODULE_DECLARATION nx_om_tcp_module =
{
    NX_MODULE_API_VERSION,
    NX_MODULE_TYPE_OUTPUT,
    NULL,			// capabilities
    om_tcp_config,		// config
    om_tcp_start,		// start
    om_tcp_stop, 		// stop
    NULL,			// pause
    NULL,			// resume
    om_tcp_init,		// init
    NULL,			// shutdown
    om_tcp_event,		// event
    NULL,			// info
    NULL,			// exports
};
