/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#ifdef WIN32

#include <unistd.h>
#include <apr_getopt.h>

#include <apr_portable.h>

#include "../common/error_debug.h"
#include "../common/event.h"
#include "../common/context.h"
#include "../common/alloc.h"
#include "nxlog.h"
#include "modules.h"
#include "router.h"
#include "ctx.h"
#include "core.h"

#define NX_LOGMODULE NX_LOGMODULE_CORE

static void WINAPI nx_win32_svc_main(DWORD argc, LPTSTR * argv);
extern void nx_logger_disable_foreground();

static boolean do_install = FALSE;
static boolean do_uninstall = FALSE;
static SERVICE_STATUS svc_status;
static SERVICE_STATUS_HANDLE svc_status_handle = NULL;
static nxlog_t nxlog;
static apr_os_thread_t _nxlog_initializer = 0;

#define nx_win32_error(fmt, args...) throw(APR_FROM_OS_ERROR(GetLastError()), fmt, ##args)


/* Start service dispatcher */
static void nx_win32_svc_dispatch()
{
    static SERVICE_TABLE_ENTRY svc_dispatch_table[] = 
    {
	{ "nxlog", nx_win32_svc_main },
	{ NULL, NULL }
    };

    if ( StartServiceCtrlDispatcher(svc_dispatch_table) == FALSE )
    {
	nx_win32_error("Cannot start service dispatcher");
    }
}



static void nx_win32_svc_start(DWORD argc, LPCTSTR *argv)
{
    SC_HANDLE service_manager = NULL;
    SC_HANDLE service_handle = NULL;
    SERVICE_STATUS status;
    nx_exception_t e;

    // Connect to service manager
    service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_manager == NULL )
    {
	nx_win32_error("Cannot initialize access to the service manager");
    }
    try
    {
	service_handle = OpenService(service_manager, "nxlog", SERVICE_ALL_ACCESS);
	if ( service_handle == NULL )
	{
	    nx_win32_error("Couldn't open nxlog service");
	}
	else
	{
	    if ( QueryServiceStatus(service_handle, &status) )
	    {
		if ( status.dwCurrentState != SERVICE_RUNNING )
		{
		    log_info("Service not running - starting service...");
		    if ( StartService(service_handle, argc, argv) == 0 )
		    {
			nx_win32_error("Failed to start nxlog service");
		    }
		}
		else
		{
		    log_error("Service is already running");
		}
	    }
	    
	    // Close connection to service
	    CloseServiceHandle(service_handle);
	}
	// Close connection to service manager
	CloseServiceHandle(service_manager);
    }
    catch(e)
    {
	if ( service_handle != NULL )
	{
	    CloseServiceHandle(service_handle);

	}
	if ( service_manager != NULL )
	{
	    CloseServiceHandle(service_manager);

	}
	rethrow(e);
    }
}



static void nx_win32_svc_install()
{
    SC_HANDLE service_manager;
    SC_HANDLE new_service = NULL;
    HKEY regkey;
    uint32_t regtype = 0;
    char regvalbuf[1024];
    uint32_t regvalbufsize = 1024;
    char servicename[1024];

    // Connect to service manager
    service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_manager == NULL )
    {
	nx_win32_error("Cannot initialize access to the service manager");
    }

    //FIXME use nxlog.ctx.user to invoke service in CreateService

    // default in case registry lookup would fail
    apr_cpystrn(servicename, 
		"\"c:\\Program Files\\nxlog\\nxlog.exe\" -c \"c:\\Program Files\\nxlog\\nxlog.conf\"",
		sizeof(servicename));

    if ( RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\nxlog", &regkey) == ERROR_SUCCESS )
    {
	if ( RegQueryValueEx(regkey, "installdir", 0, &regtype,
			     (unsigned char *) regvalbuf, &regvalbufsize) == ERROR_SUCCESS )
	{
	    if ( regtype == REG_SZ )
	    {
		apr_snprintf(servicename, sizeof(servicename), "\"%snxlog.exe\"", regvalbuf);
	    }
	}
	RegCloseKey(regkey);
    }

    // Install the new service
    // TODO: add load order dependency on the eventlog service
    new_service = CreateService(service_manager, "nxlog", "nxlog",
				SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
				SERVICE_AUTO_START, SERVICE_ERROR_IGNORE,
				servicename, NULL, NULL,
				"eventlog\0", NULL, NULL);
    if ( new_service == NULL )
    {
	nx_win32_error("Couldn't create service");
    }
    else
    {
	CloseServiceHandle(new_service);
	log_info("Service successfully installed");
    }

    // Close the connection to service manager
    CloseServiceHandle(service_manager);
}



static void nx_win32_svc_uninstall()
{
    SC_HANDLE service_manager = NULL;
    SC_HANDLE service_handle = NULL;
    SERVICE_STATUS query_status;
    nx_exception_t e;

    // Connect to service manager
    service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_manager == NULL )
    {
	nx_win32_error("Cannot initialize access to the service manager");
    }

    try
    {
	// Connect to service
	service_handle = OpenService(service_manager, "nxlog", SERVICE_ALL_ACCESS | DELETE);
	if ( service_handle == NULL )
	{
	    nx_win32_error("Couldn't open nxlog service");
	}
	else
	{
	    // Check that the service is stopped
	    if ( QueryServiceStatus(service_handle, &query_status) &&
		 (query_status.dwCurrentState == SERVICE_RUNNING) )
	    {
		throw_msg("Service is running, please stop it first.");
	    }
	    else
	    { // we can remove
		if ( DeleteService(service_handle) == FALSE )
		{
		    nx_win32_error("Couldn't delete service");
		}
		else
		{
		    log_info("Service successfully uninstalled");
		}
	    }
	    // Close connection to service
	    CloseServiceHandle(service_handle);
	}
	// Close connection to service manager
	CloseServiceHandle(service_manager);
    }
    catch(e)
    {
	if ( service_handle != NULL )
	{
	    CloseServiceHandle(service_handle);

	}
	if ( service_manager != NULL )
	{
	    CloseServiceHandle(service_manager);

	}
	rethrow(e);
    }
}



/* Callback for service changes */
static void WINAPI nx_win32_svc_change(DWORD cmd)
{
    // Only STOP command is handled
    switch ( cmd )
    {
	case SERVICE_CONTROL_SHUTDOWN:
	    log_warn("received a system shutdown request");
	    // fallthrough
	case SERVICE_CONTROL_STOP:
	    log_warn("stopping nxlog service");
	    nxlog.terminate_request = TRUE;
	    // Wait until service stops
	    while ( svc_status.dwCurrentState != SERVICE_STOPPED )
	    {
		Sleep(500);
	    }
	    break;
	case SERVICE_CONTROL_INTERROGATE:
	    // ignore
	    log_debug("ignoring unsupported service change request: SERVICE_CONTROL_INTERROGATE");
	    break;
	case SERVICE_CONTROL_PAUSE:
	    log_warn("ignoring unsupported service change request: SERVICE_CONTROL_PAUSE");
	    break;
	case SERVICE_CONTROL_CONTINUE:
	    log_warn("ignoring unsupported service change request: SERVICE_CONTROL_CONTINUE");
	    break;
	case SERVICE_CONTROL_PARAMCHANGE:
	    log_warn("ignoring unsupported service change request: SERVICE_CONTROL_PARAMCHANGE");
	    break;
	case 200:
	    nxlog_dump_info();
    	    break;
	case 201:
	{
	    nx_ctx_t *ctx;

	    log_info("got user control signal, switching to debug loglevel");
	    ctx = nx_ctx_get();
	    ctx->loglevel = NX_LOGLEVEL_DEBUG;
	}
	break;
	default:
	    log_warn("ignoring unsupported service change request (%d)", cmd);
    }
}



static void nxlog_exit_function()
{
    static boolean exited = FALSE;

    if ( exited == TRUE )
    {
	return;
    }
    exited = TRUE;

    if ( _nxlog_initializer == 0 )
    {
	return;
    }

    if ( nxlog.pid != (int) getpid() )
    {
	return;
    }

    nx_ctx_free(nxlog.ctx);
    apr_pool_destroy(nxlog.pool);
    apr_terminate();
}



static void print_usage()
{
    printf(PACKAGE "-" VERSION_STRING "\n"
	   " usage: "
	   " nxlog.exe [OPTIONS]\n"
	   "   [-h] print help\n"
	   "   [-f] run in foreground, otherwise try to start the nxlog service\n"
	   "   [-c conffile] specify an alternate config file\n"
	   "   [-i] install service available to service manager\n"
	   "   [-u] uninstall service\n"
	   "   [-s] stop running nxlog service\n"
	   "   [-v] verify configuration file syntax\n"
	   );
//	   "   [-r] reload configuration of a running instance\n"
}



static void set_configfile()
{
    HKEY regkey;
    uint32_t regtype = 0;
    char regvalbuf[1024];
    uint32_t regvalbufsize = 1024;

    if ( nxlog.cfgfile != NULL )
    {
	return;
    }

    if ( RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\nxlog", &regkey) == ERROR_SUCCESS )
    {
	if ( RegQueryValueEx(regkey, "installdir", 0, &regtype,
			     (unsigned char *) regvalbuf, &regvalbufsize) == ERROR_SUCCESS )
	{
	    if ( regtype == REG_SZ )
	    {
		nxlog.cfgfile = apr_psprintf(nxlog.pool, "%sconf\\nxlog.conf", regvalbuf);
	    }
	}
	RegCloseKey(regkey);
    }

    if ( nxlog.cfgfile == NULL )
    {
	nxlog.cfgfile = apr_pstrdup(nxlog.pool, NX_CONFIGFILE);
    }
}



static void parse_cmd_line(int argc, const char * const *argv)
{
    const char *opt_arg;
    apr_status_t rv;
    apr_getopt_t *opt;
    int ch;

    static const apr_getopt_option_t options[] = {
	{ "help", 'h', 0, "print help" }, 
	{ "foreground", 'f', 0, "run in foreground" },
	{ "stop", 's', 0, "stop a running instance" },
//	{ "reload", 'r', 0, "reload configuration of a running instance" },
	{ "conf", 'c', 1, "configuration file" }, 
	{ "verify", 'v', 0, "verify configuration file syntax" },
	{ "install", 'i', 0, "install service available to service manager" },
	{ "uninstall", 'u', 0, "uninstall service" },
	{ NULL, 0, 1, NULL }, 
    };
	
    apr_getopt_init(&opt, nxlog.pool, argc, argv);
    while ( (rv = apr_getopt_long(opt, options, &ch, &opt_arg)) == APR_SUCCESS )
    {
	switch ( ch )
	{
	    case 'c':	/* configuration file */
		nxlog.cfgfile = apr_pstrdup(nxlog.pool, opt_arg);
		break;
	    case 'f':	/* foreground */
		nxlog.foreground = TRUE;
		break;
	    case 'h':	/* help */
		print_usage();
		exit(-1);
	    case 's':	/* stop */
		nxlog.do_stop = TRUE;
		break;
/*
	    case 'r':	// reload
		nxlog.do_restart = TRUE;
		break;
*/
	    case 'v':	/* verify */
		nxlog.verify_conf = TRUE;
		nxlog.ctx->ignoreerrors = FALSE;
		break;
	    case 'i':	/* install */
		do_install = TRUE;
		break;
	    case 'u':	/* uninstall */
		do_uninstall = TRUE;
		break;
	    default:
		print_usage();
		exit(-1);
	}
    }

    set_configfile();

    if ( (rv != APR_SUCCESS) && (rv != APR_EOF) )
    {
        throw(rv, "Could not parse options");
    }
}



static void WINAPI nx_win32_svc_main(DWORD argc, LPTSTR *argv)
{
    nx_context_t thread_context;
    nx_exception_t e;

    if ( _nxlog_initializer == 0 )
    { // running from service manager
	ASSERT(nx_init(&argc, &argv, NULL) == TRUE);

	nxlog_init(&nxlog);
	nx_logger_disable_foreground();
    }
    else if ( _nxlog_initializer != apr_os_thread_current() )
    {
	// service dispatcher runs in a new thread, we need
	// to initialize the exception context.
	_nxlog_initializer = apr_os_thread_current();
	memset(&thread_context, 0, sizeof(nx_context_t));
	init_exception_context(&thread_context.exception_context);
	apr_threadkey_private_set(&thread_context, nx_get_context_key());
    }

    log_debug("nx_win32_svc_main");

    try
    {
	// read config cache
	nx_config_cache_read();
	log_debug("nxlog cache read");

	// load DSO and read and verify module config
	nx_ctx_config_modules(nxlog.ctx);
	log_debug("nxlog config OK");

	// initialize modules
	nx_ctx_init_modules(nxlog.ctx);
    
	// initialize log routes
	nx_ctx_init_routes(nxlog.ctx);
	nx_ctx_init_jobs(nxlog.ctx);
	
	nx_ctx_restore_queues(nxlog.ctx);
    
	// setup threadpool
	nxlog_create_threads(&nxlog);

	// start modules
	nx_ctx_start_modules(nxlog.ctx);

	if ( nxlog.foreground != TRUE )
	{
	    // register to service manager
	    svc_status_handle = RegisterServiceCtrlHandler("nxlog", nx_win32_svc_change);
	    if ( svc_status_handle == 0 )
	    {
		nx_win32_error("RegisterServiceCtrlHandler() failed, couldn't register the service control handler");
	    }

	    // Signal to svc manager that we are running
	    svc_status.dwWin32ExitCode = 0;
	    svc_status.dwServiceSpecificExitCode = 0;
	    svc_status.dwCheckPoint = 0;
	    svc_status.dwWaitHint = 0; 
	    svc_status.dwServiceType = SERVICE_WIN32;
	    svc_status.dwCurrentState = SERVICE_RUNNING;
	    // Do we need to support SERVICE_ACCEPT_PRESHUTDOWN ?
	    svc_status.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	    if ( SetServiceStatus(svc_status_handle, &svc_status) == FALSE )
	    {
		nx_win32_error("Cannot send start service status update");
	    }
	}
    
	log_info(PACKAGE"-"VERSION_STRING" started");
    }
    catch(e)
    {
	log_exception(e);
	log_error("exiting...");
	svc_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(svc_status_handle, &svc_status);
	exit(e.code);
    }

    // mainloop
    nxlog_mainloop(&nxlog, FALSE);
    
    nxlog_shutdown(&nxlog);

    if ( nxlog.foreground != TRUE )
    {
	// Signal back that we are stopped
	svc_status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(svc_status_handle, &svc_status);
	log_debug("service stopped");
    }

    nxlog_exit_function();
}



static void nx_win32_svc_stop()
{
    SC_HANDLE service_manager = NULL;
    SC_HANDLE service_handle = NULL;
    SERVICE_STATUS status;
    nx_exception_t e;

    // Connect to service manager
    service_manager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
    if ( service_manager == NULL )
    {
	nx_win32_error("Cannot initialize access to the service manager");
    }
    try
    {
	service_handle = OpenService(service_manager, "nxlog", SERVICE_ALL_ACCESS);
	if ( service_handle == NULL )
	{
	    nx_win32_error("Couldn't open nxlog service");
	}
	else
	{
	    if ( QueryServiceStatus(service_handle, &status) )
	    {
		if ( status.dwCurrentState != SERVICE_STOPPED )
		{
		    log_info("Service currenty active - stopping service...");
		    if ( !ControlService(service_handle, SERVICE_CONTROL_STOP, &status) )
		    {
			nx_win32_error("Couldn't stop service");
		    }
		    else
		    {
			Sleep(500);
		    }
		}
		else
		{
		    log_error("Service is already stopped");
		}
	    }
	    // Close connection to service
	    CloseServiceHandle(service_handle);
	}
	// Close connection to service manager
	CloseServiceHandle(service_manager);
    }
    catch(e)
    {
	if ( service_handle != NULL )
	{
	    CloseServiceHandle(service_handle);

	}
	if ( service_manager != NULL )
	{
	    CloseServiceHandle(service_manager);

	}
	rethrow(e);
    }
}



/**
 * The whole thing starts here if called directly
 */

int main(int argc, const char * const *argv, const char * const *env)
{
    DWORD dispmode;
    nx_exception_t e;

    ASSERT(nx_init(&argc, &argv, &env) == TRUE);

    nxlog_init(&nxlog);
    _nxlog_initializer = apr_os_thread_current();

    try
    {
	// read cmd line
	parse_cmd_line(argc, argv);
/*
	// reload switch?
	if ( nxlog.do_restart == TRUE )
	{
	    //FIXME restart service; use custom control code?
	}
*/
	// stop switch?
	if ( nxlog.do_stop == TRUE )
	{
	    nx_win32_svc_stop();
	    exit(0);
	}

	if ( do_install == TRUE )
	{
	    nx_win32_svc_install();
	    exit(0);
	}

	if ( do_uninstall == TRUE )
	{
	    nx_win32_svc_uninstall();
	    exit(0);
	}

	// load and parse config
	nx_ctx_parse_cfg(nxlog.ctx, nxlog.cfgfile);

	if ( nxlog.ctx->spooldir != NULL )
	{
	    CHECKERR_MSG(apr_filepath_set(nxlog.ctx->spooldir, nxlog.pool),
		     "Couldn't change to SpoolDir '%s'", nxlog.ctx->spooldir);
	}

	nx_ctx_init_logging(nxlog.ctx);

	if ( nxlog.ctx->rootdir != NULL )
	{
	    throw_msg("RootDir not supported on windows");
	}

	if ( nxlog.verify_conf == TRUE )
	{
	    // load DSO and read and verify module config
	    nx_ctx_config_modules(nxlog.ctx);
	    nx_ctx_init_routes(nxlog.ctx);
	    log_info("configuration OK");
	    exit(0);
	}
	
	if ( nxlog.foreground == TRUE )
	{
	    nx_win32_svc_main(argc, argv);
	}
	else
	{
	    // detect wheter we are invoked by SVM or from the console
	    if ( GetConsoleDisplayMode(&dispmode) == 0 )
	    {
		nx_win32_svc_dispatch();
	    }
	    else
	    {
		nx_win32_svc_start(argc, argv);
	    }
	}
    }
    catch(e)
    {
	apr_file_t *f = NULL;
	apr_pool_t *pool;
	HKEY regkey;
	uint32_t regtype = 0;
	char regvalbuf[1024];
	uint32_t regvalbufsize = 1024;
	const char *logfile = "c:\\nxlog-service-error.txt";

	pool = nx_pool_create_core();

	// TODO: log to eventlog in addition?

	if ( RegOpenKey(HKEY_LOCAL_MACHINE, "SOFTWARE\\nxlog", &regkey) == ERROR_SUCCESS )
	{
	    if ( RegQueryValueEx(regkey, "installdir", 0, &regtype,
				 (unsigned char *) regvalbuf, &regvalbufsize) == ERROR_SUCCESS )
	    {
		if ( regtype == REG_SZ )
		{
		    logfile = apr_psprintf(nxlog.pool, "%sdata\\nxlog.log", regvalbuf);
		}
	    }
	    RegCloseKey(regkey);
	}
	apr_file_open(&f, logfile, APR_READ | APR_WRITE | APR_CREATE | APR_APPEND,
		      APR_OS_DEFAULT, pool);
	// FXIME: check status code 
	if ( f != NULL )
	{
	    nx_string_t *tmpstr;
	    int i;
	    char errmsg[1024];

	    tmpstr = nx_string_new();

	    for ( i = ((int) e.num_throw) - 1; i >= 0; i-- )
	    {
		if ( tmpstr->len > 0 )
		{
		    nx_string_append(tmpstr, NX_LINEFEED, -1);
		}
		nx_string_append(tmpstr, e.msgbuf + e.throwlist[i].msgoffs, -1);
	    }
	    nx_string_append(tmpstr, NX_LINEFEED, -1);
	    if ( e.code != APR_SUCCESS )
	    {
		apr_strerror(e.code, errmsg, sizeof(errmsg));
		nx_string_append(tmpstr, errmsg, -1);
		nx_string_append(tmpstr, NX_LINEFEED, -1);
	    }
	    apr_file_printf(f, "nxlog failed to start: %s\r\n", tmpstr->buf);
	    apr_file_close(f);
	    nx_string_free(tmpstr);
	}
	apr_pool_destroy(pool);
	log_exception(e);
	exit(e.code);
    }

    return ( 0 );
}

#endif /* WIN32 */
