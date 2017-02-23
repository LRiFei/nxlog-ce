/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#include "../../../common/module.h"
#include "../../../common/event.h"
#include "../../../common/error_debug.h"
#include "../../../common/config_cache.h"
#include "../../../common/date.h"
#include "../../../core/ctx.h"

#include <windows.h>

#include "im_mseventlog.h"

#define NX_LOGMODULE NX_LOGMODULE_MODULE

#define NX_IM_MSEVENTLOG_READ_INTERVAL 1



static nx_expr_proc_t *lookup_convert_fields()
{
    nx_ctx_t *ctx;
    nx_module_t *module;
    int i;

    ctx = nx_ctx_get();

    for ( module = NX_DLIST_FIRST(ctx->modules);
	  module != NULL;
	  module = NX_DLIST_NEXT(module, link) )
    {
	if ( strcmp(module->dsoname, "xm_charconv") == 0 )
	{
	    break;
	}
    }
    
    if ( module == NULL )
    {
	return ( NULL );
    }

    ASSERT(module->decl->exports != NULL);
    ASSERT(module->decl->exports->num_proc > 0);

    for ( i = 0; i < module->decl->exports->num_proc; i++ )
    {
	if ( (strcmp(module->decl->exports->procs[i].name, "convert_fields") == 0) &&
	     (module->decl->exports->procs[i].num_arg == 2) ) 
	{
	    return ( &(module->decl->exports->procs[i]) );
	}
    }

    return ( NULL );
}



/* see this msdn link for code sample
 http://msdn.microsoft.com/en-us/library/bb427356%28v=vs.85%29.aspx
*/

#define mseventlog_error(fmt, args...) throw((apr_status_t) APR_FROM_OS_ERROR(GetLastError()), fmt, ## args)


static boolean parse_libs(char *dllIn, char *dllOut, uint32_t seqnum)
{
    int i = 0;
    int j = 0;

    while ( (seqnum > 0) && (dllIn[i] != '\0') )
    {
	if (dllIn[i] == ';')
	{
	    seqnum--;
	}
	i++;
    }

    if ( dllIn[i] == '\0' )
    {
	return ( FALSE );
    }
    while ( (dllIn[i] != '\0') && (dllIn[i] != ';') )
    {
	dllOut[j] = dllIn[i];
	j++;
	i++;
    }
    dllOut[j] = '\0';

    return ( TRUE );
}



static const char *get_dll_path(nx_module_t *module,
				const char *srcname,
				const char *source,
				const char *filetype,
				boolean *vistalog)
{
    const char *retval = NULL;
    char regkeybuf[IM_MSEVENTLOG_REG_BUFFER_LEN];
    char hashkey[IM_MSEVENTLOG_REG_BUFFER_LEN];
    char regvalbuf[IM_MSEVENTLOG_REG_BUFFER_LEN];
    DWORD regvalbufsize = IM_MSEVENTLOG_REG_BUFFER_LEN;
    char dll[IM_MSEVENTLOG_REG_BUFFER_LEN];
    nx_im_mseventlog_conf_t *imconf;
    HKEY regkey;
    DWORD regtype = 0;

    ASSERT(module != NULL);
    ASSERT(srcname != NULL);
    ASSERT(source != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    apr_snprintf(regkeybuf, sizeof(regkeybuf),
		 "SYSTEM\\CurrentControlSet\\Services\\EventLog\\%s\\%s",
		 srcname, source);
    apr_snprintf(hashkey, sizeof(hashkey), "%s|%s|%s",
		 srcname, source, filetype);

    retval = apr_hash_get(imconf->dll_names, hashkey, APR_HASH_KEY_STRING);
    if ( retval != NULL )
    {
	return ( retval );
    }

    if ( RegOpenKey(HKEY_LOCAL_MACHINE, regkeybuf, &regkey) == ERROR_SUCCESS )
    {
	if ( RegQueryValueEx(regkey, filetype, 0, &regtype,
			     (unsigned char *) regvalbuf, &regvalbufsize) == ERROR_SUCCESS )
	{
	    if ( regtype == REG_SZ )
	    {
		retval = apr_pstrdup(module->pool, regvalbuf);
		apr_hash_set(imconf->dll_names, apr_pstrdup(module->pool, hashkey),
			     APR_HASH_KEY_STRING, retval);
	    }
	    else
	    {
		if ( ExpandEnvironmentStrings(regvalbuf, dll, sizeof(dll)) > 0 )
		{
		    retval = apr_pstrdup(module->pool, dll);
		    apr_hash_set(imconf->dll_names, apr_pstrdup(module->pool, hashkey),
				 APR_HASH_KEY_STRING, retval);
		}
		else
		{
		    log_aprerror((apr_status_t) APR_FROM_OS_ERROR(GetLastError()),
				 "ExpandEnvironmentStrings() failed for %s", regvalbuf);
		}
	    }
	}
	else if ( RegQueryValueEx(regkey, "ProviderGuid", 0, &regtype,
			     (unsigned char *) regvalbuf, &regvalbufsize) == ERROR_SUCCESS )
	{
	    *vistalog = TRUE;
	}
	RegCloseKey(regkey);
    }
    //log_debug("got EventMessageFile: [%s]", retval);
    
    return ( retval );
}



static HANDLE get_dll_lib(nx_module_t *module, const char *dll)
{
    HANDLE lib = NULL;
    nx_im_mseventlog_conf_t *imconf;

    ASSERT(module != NULL);
    ASSERT(dll != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    lib = (HANDLE) apr_hash_get(imconf->dll_libs, dll, APR_HASH_KEY_STRING);
    if ( lib != NULL )
    {
	return ( lib );
    }
    lib = LoadLibraryEx(dll, NULL, DONT_RESOLVE_DLL_REFERENCES | LOAD_LIBRARY_AS_DATAFILE);
    if ( lib != NULL )
    {
	apr_hash_set(imconf->dll_libs, dll, APR_HASH_KEY_STRING, lib);
    }
    else
    {
	log_debug("LoadLibrary() failed for %s", dll);
//	log_aprerror(APR_FROM_OS_ERROR(GetLastError()), "LoadLibrary() failed for %s", dll);
    }
    return ( lib );
}



static void im_mseventlog_set_category_string(nx_module_t *module,
					      nx_logdata_t *logdata,
					      nx_eventlog_src_t *event_src,
					      const char *source,
					      DWORD category)
{
    const char *dll = NULL;
    boolean vistalog = FALSE;
    char dllparsed[IM_MSEVENTLOG_REG_BUFFER_LEN];
    uint32_t seqnum = 0;
    char ctgstring[512];
    uint32_t msgsize = 0;
    HANDLE lib;

    if ( category == 0 )
    {
	return;
    }

    dll = get_dll_path(module, event_src->name, source, "CategoryMessageFile", &vistalog);
    if ( dll == NULL )
    { // try again with eventlog name
	dll = get_dll_path(module, event_src->name, event_src->name,
			   "CategoryMessageFile", &vistalog);
    }

    if ( dll == NULL )
    {
	return;
    }

    while ( (parse_libs(dll, dllparsed, seqnum) == TRUE) )
    {
	if ( (lib = get_dll_lib(module, dllparsed)) != NULL )
	{
	    ctgstring[0] = '\0';
	    msgsize = FormatMessage(FORMAT_MESSAGE_FROM_HMODULE |
				    FORMAT_MESSAGE_FROM_SYSTEM |
				    FORMAT_MESSAGE_IGNORE_INSERTS |
				    FORMAT_MESSAGE_MAX_WIDTH_MASK,
				    lib,
				    category, 
				    /* languageid*/ 0,
				    (LPTSTR) ctgstring, sizeof(ctgstring), NULL);
	    if ( (msgsize > 0) && (ctgstring[0] != '\0') )
	    {
		nx_logdata_set_string(logdata, "Category", ctgstring);
/*
		log_info("Category: %s [EVENTID:%d CATEGORYNUM: %d %s/%s %s]",
			 ctgstring, eventid, category, event_src->name, source, dllparsed);
*/
		return;
	    }
	}
	seqnum++;
    }
    return;
}



static void convert_fields_to_utf8(nx_module_t *module, nx_logdata_t *logdata)
{
    nx_expr_eval_ctx_t eval_ctx;
    nx_im_mseventlog_conf_t *imconf;

    static nx_string_t utf8str;
    static nx_string_t lcstr;
    static nx_expr_t utf8strexpr;
    static nx_expr_t lcstrexpr;
    static boolean initialized = FALSE;
    static char localecharset[32];
    static char utf8[6] = { 'U', 'T', 'F', '-', '8', '\0' };
    static nx_expr_arg_list_t args;
    static nx_expr_arg_t arg1;
    static nx_expr_arg_t arg2;

    ASSERT(module != NULL);

    if ( initialized == FALSE )
    {
	lcstr.len = apr_snprintf(localecharset, sizeof(localecharset), "CP%u", GetACP());
	lcstr.buf = localecharset;

	lcstrexpr.type = NX_EXPR_TYPE_VALUE;
	lcstrexpr.rettype = NX_VALUE_TYPE_STRING;
	lcstrexpr.value.type = NX_VALUE_TYPE_STRING;
	lcstrexpr.value.string = &lcstr;
	lcstrexpr.value.defined = TRUE;
	arg1.expr = &lcstrexpr;

	utf8str.buf = utf8;
	utf8str.len = 5;

	utf8strexpr.type = NX_EXPR_TYPE_VALUE;
	utf8strexpr.rettype = NX_VALUE_TYPE_STRING;
	utf8strexpr.value.type = NX_VALUE_TYPE_STRING;
	utf8strexpr.value.string = &utf8str;
	utf8strexpr.value.defined = TRUE;
	arg2.expr = &utf8strexpr;

	NX_DLIST_INIT(&args, nx_expr_arg_t, link);
	NX_DLIST_INSERT_TAIL(&args, &arg1, link);
	NX_DLIST_INSERT_TAIL(&args, &arg2, link);

	initialized = TRUE;
    }
    eval_ctx.module = module;
    eval_ctx.logdata = logdata;

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    ASSERT(imconf->convert_fields->cb != NULL);
    imconf->convert_fields->cb(&eval_ctx, module, &args);
}



#define MAX_STRINGS 100

static nx_logdata_t *im_mseventlog_get_logdata(nx_module_t *module,
					       EVENTLOGRECORD *record,
					       nx_eventlog_src_t *event_src)
{
    nx_logdata_t *logdata;
    char *hostname;
    char *source;
    char *stringsptr;
    uint32_t i;
    char user[256];
    char domain[256];
    DWORD user_len = sizeof(user) - 1;
    DWORD domain_len = sizeof(domain) - 1;
    SID_NAME_USE account_type;
    HANDLE lib;
    const char *dll;
    char dllparsed[IM_MSEVENTLOG_REG_BUFFER_LEN];
    char *msg = NULL;
    uint32_t seqnum = 0;
    uint32_t msgsize = 0;
    boolean gotmsg = FALSE;
    char *strings[MAX_STRINGS];
    char tmpstr[30];
    nx_value_t *message;
    nx_loglevel_t loglevel = NX_LOGLEVEL_INFO;
    char eventidstr[32];
    boolean vistalog = FALSE;
    nx_im_mseventlog_conf_t *imconf;

    ASSERT(module != NULL);
    ASSERT(record != NULL);
    ASSERT(event_src != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

/* FOR TESTING
    if ( (int64_t) (record->EventID & 0x0000FFFF) != 1100 )
    {
	return ( NULL );
    }
*/    
    logdata = nx_logdata_new();

    source = (LPSTR) ((LPBYTE) record + sizeof(EVENTLOGRECORD));
    hostname = source + strlen(source) + 1;
    stringsptr = (LPSTR) ((LPBYTE) record + record->StringOffset);

    nx_date_to_iso(tmpstr, sizeof(tmpstr), apr_time_from_sec(record->TimeGenerated));
    nx_string_append(logdata->raw_event, tmpstr, -1);
    nx_string_append(logdata->raw_event, " ", 1);
    nx_logdata_set_datetime(logdata, "EventTime", apr_time_from_sec(record->TimeGenerated));
    nx_logdata_set_datetime(logdata, "EventTimeWritten", apr_time_from_sec(record->TimeWritten));

    if ( (hostname != NULL) && (strlen(hostname) > 0) )
    {
	nx_string_append(logdata->raw_event, hostname, -1);
	nx_string_append(logdata->raw_event, " ", 1);
	nx_logdata_set_string(logdata, "Hostname", hostname);
    }

    switch ( record->EventType )
    {
	case EVENTLOG_ERROR_TYPE:
	    nx_string_append(logdata->raw_event, "ERROR", -1);
	    nx_logdata_set_string(logdata, "EventType", "ERROR");
	    loglevel = NX_LOGLEVEL_ERROR;
	    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
	    break;
	case EVENTLOG_AUDIT_FAILURE:
	    nx_string_append(logdata->raw_event, "AUDIT_FAILURE", -1);
	    nx_logdata_set_string(logdata, "EventType", "AUDIT_FAILURE");
	    loglevel = NX_LOGLEVEL_ERROR;
	    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
	    break;
	case EVENTLOG_AUDIT_SUCCESS:
	    nx_string_append(logdata->raw_event, "AUDIT_SUCCESS", -1);
	    nx_logdata_set_string(logdata, "EventType", "AUDIT_SUCCESS");
	    loglevel = NX_LOGLEVEL_INFO;
	    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
	    break;
	case EVENTLOG_WARNING_TYPE:
	    nx_string_append(logdata->raw_event, "WARNING", -1);
	    nx_logdata_set_string(logdata, "EventType", "WARNING");
	    loglevel = NX_LOGLEVEL_WARNING;
	    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
	    break;
	default:
	    //log_debug("unknown eventtype: %d", record->EventType);
	    // fallthrough
	case EVENTLOG_INFORMATION_TYPE:
	    nx_string_append(logdata->raw_event, "INFO", -1);
	    nx_logdata_set_string(logdata, "EventType", "INFO");
	    loglevel = NX_LOGLEVEL_INFO;
	    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
	    break;
    }
    nx_logdata_set_string(logdata, "Severity", nx_loglevel_to_string(loglevel));

    nx_string_append(logdata->raw_event, " ", 1);
/*
    nx_string_append(logdata->raw_event, "source:\"", -1);
    nx_string_append(logdata->raw_event, source, -1);
    nx_string_append(logdata->raw_event, "\" ", 2);
*/
    nx_logdata_set_string(logdata, "SourceName", source);
    nx_logdata_set_string(logdata, "FileName", event_src->name);
    nx_logdata_set_integer(logdata, "EventID", (int64_t) (record->EventID & 0x0000FFFF));
    nx_logdata_set_integer(logdata, "CategoryNumber", (int64_t) record->EventCategory);
    im_mseventlog_set_category_string(module, logdata, event_src, source, record->EventCategory);
    nx_logdata_set_integer(logdata, "RecordNumber", (int64_t) record->RecordNumber);

    apr_snprintf(eventidstr, sizeof(eventidstr), "%d", (int) (record->EventID & 0x0000FFFF));
    nx_string_append(logdata->raw_event, eventidstr, -1);
    nx_string_append(logdata->raw_event, " ", 1);

    if ( (record->UserSidLength > 0) && (record->UserSidOffset > 0) )
    {
	ASSERT((record->UserSidOffset + record->UserSidLength) < record->Length);
	user[0] = '\0';
	domain[0] = '\0';

	if ( LookupAccountSid(NULL, (SID *)((LPSTR) record + record->UserSidOffset),
			      user, &user_len, domain, &domain_len, &account_type) != 0 )
	{
	    if ( domain[0] != '\0' )
	    {
		nx_logdata_set_string(logdata, "Domain", domain);
		nx_string_append(logdata->raw_event, domain, -1);
	    }
	    if ( user[0] != '\0' )
	    {
		if ( domain[0] != '\0' )
		{
		    nx_string_append(logdata->raw_event, "\\", 1);
		}
		nx_logdata_set_string(logdata, "AccountName", user);
		nx_string_append(logdata->raw_event, user, -1);
		nx_string_append(logdata->raw_event, " ", 1);
	    }

	    switch ( account_type )
	    {
		case SidTypeUser:
		    nx_logdata_set_string(logdata, "AccountType", "User");
		    break;
		case SidTypeGroup:
		    nx_logdata_set_string(logdata, "AccountType", "Group");
		    break;
		case SidTypeDomain:
		    nx_logdata_set_string(logdata, "AccountType", "Domain");
		    break;
		case SidTypeAlias:
		    nx_logdata_set_string(logdata, "AccountType", "Alias");
		    break;
		case SidTypeWellKnownGroup:
		    nx_logdata_set_string(logdata, "AccountType", "Well Known Group");
		    break;
		case SidTypeDeletedAccount:
		    nx_logdata_set_string(logdata, "AccountType", "Deleted Account");
		    break;
		case SidTypeInvalid:
		    nx_logdata_set_string(logdata, "AccountType", "Invalid");
		    break;
		case SidTypeUnknown:
		    nx_logdata_set_string(logdata, "AccountType", "Unknown");
		    break;
		case SidTypeComputer:
		    nx_logdata_set_string(logdata, "AccountType", "Computer");
		    break;
		default:
		    nx_logdata_set_string(logdata, "AccountType", "Unknown");
		    break;
	    }
	}
    }


    for ( i = 0; i < record->NumStrings; i++ )
    {
	if ( i >= MAX_STRINGS - 1 )
	{
	    break;
	}
	//log_info("strings[%d]: [%s]", i, stringsptr);
	strings[i] = stringsptr;
	stringsptr += strlen(stringsptr) + 1;
    }
    strings[i] = NULL;

    message = nx_value_new_string("");

    //log_info("numstrings: %d, stringsptr: %d", record->NumStrings, stringsptr);

    // Load messages from dlls
    dll = get_dll_path(module, event_src->name, source, "EventMessageFile", &vistalog);

    if ( dll != NULL )
    {
	//log_info("parse libs:[%s]", dll);
	while ( (parse_libs(dll, dllparsed, seqnum) == TRUE) && (gotmsg != TRUE) )
	{
	    //log_info("dll: [%s]", dllparsed);

	    msg = NULL;
	    if ( (lib = get_dll_lib(module, dllparsed)) != NULL )
	    {
		msgsize = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_HMODULE |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_ARGUMENT_ARRAY |
					FORMAT_MESSAGE_MAX_WIDTH_MASK,
					lib,
					record->EventID, 
					/* languageid*/ 0,
					(LPTSTR) &msg, 512, strings);

		// FIXME: resolve parameter insertion strings as in
		// http://msdn.microsoft.com/en-us/library/bb427356%28v=vs.85%29.aspx

		if ( (msg != NULL) && (msgsize > 0) )
		{
		    gotmsg = TRUE;
		    nx_string_append(logdata->raw_event, msg, (int) msgsize);
		    nx_string_append(message->string, msg, (int) msgsize);
		    //log_info("append: [%s]", msg);
		}
		if ( msg != NULL )
		{
		    LocalFree((HANDLE) msg);
		}
		else
		{
		    log_debug("msg is NULL");
		}
		if ( msgsize == 0 )
		{
		    log_debug("FormatMessage() failed for EventID %"APR_INT64_T_FMT,
			      (int64_t) (record->EventID & 0x0000FFFF));
/*
		    log_aprerror(APR_FROM_OS_ERROR(GetLastError()), "FormatMessage() failed for EventID %"APR_INT64_T_FMT,
				 (int64_t) (record->EventID & 0x0000FFFF));
*/
		}
	    }
	    else
	    {
		char notfound[128];
		int len;

		len = apr_snprintf(notfound, sizeof(notfound),
				   "The description for EventID %"APR_INT64_T_FMT" from source %s cannot be found.",
				   (int64_t) (record->EventID & 0x0000FFFF), source);
		    
		log_debug("lib is NULL");
		nx_string_append(logdata->raw_event, notfound, len);
		nx_string_append(message->string, notfound, len);
		gotmsg = TRUE;
	    }
	    seqnum++;
	}
    }
    else
    {
	log_debug("dll is NULL");
	if ( vistalog == TRUE )
	{
	    char notfound[256];
	    int len;

	    len = apr_snprintf(notfound, sizeof(notfound),
			       "The description for EventID %"APR_INT64_T_FMT" from source %s cannot be read by im_mseventlog because this does not support the newer WIN2008/Vista EventLog API. Consider using the im_msvistalog module instead.",
			       (int64_t) (record->EventID & 0x0000FFFF), source);
	    
	    nx_string_append(logdata->raw_event, notfound, len);
	    nx_string_append(message->string, notfound, len);
	    gotmsg = TRUE;
	}
    }

    if ( gotmsg != TRUE )
    {
	for ( i = 0; i < MAX_STRINGS; i++ )
	{
	    if ( strings[i] != NULL )
	    {
		if ( i > 0 )
		{
		    nx_string_append(logdata->raw_event, " ", 1);
		    nx_string_append(message->string, " ", 1);
		    gotmsg = TRUE;
		}
		nx_string_append(logdata->raw_event, strings[i], -1);
		nx_string_append(message->string, strings[i], -1);
		gotmsg = TRUE;
	    }
	    else
	    {
		break;
	    }
	    if ( strings[i][0] == '\0' )
	    {
		break;
	    }
	}
    }

    if ( gotmsg == TRUE )
    {
	nx_logdata_append_field_value(logdata, "Message", message);
    }
    else
    {
	nx_value_free(message);
    }

    if ( imconf->convert_fields != NULL )
    {
	convert_fields_to_utf8(module, logdata);
    }

    return ( logdata );
}


// got an error, close eventlog
static void im_mseventlog_error_close_src(nx_eventlog_src_t *event_src) 
{
    ASSERT(event_src != NULL);

    if ( event_src->handle != NULL )
    {
	CloseEventLog(event_src->handle);
	event_src->handle = NULL;
    }

    if ( event_src->blacklist_interval == 0 )
    {
	event_src->blacklist_interval = 1;
    }
    else
    {
	event_src->blacklist_interval *= 2;
	if ( event_src->blacklist_interval > 60*10 ) 
	{ // limit at 10 minutes
	    event_src->blacklist_interval = 60*10;
	}
    }
    event_src->blacklist_until = apr_time_now() + event_src->blacklist_interval * APR_USEC_PER_SEC;
}



static void im_mseventlog_set_readfromcurrent(nx_eventlog_src_t *event_src)
{
    DWORD oldest;
    DWORD record_cnt;

    ASSERT(event_src->handle != NULL);

    if ( (GetOldestEventLogRecord(event_src->handle, &oldest) == TRUE) &&
	 (GetNumberOfEventLogRecords(event_src->handle, &record_cnt) == TRUE) )
    {
	log_debug("setting readfromcurrent for %s, oldest is %d, number of events is %d",
		  event_src->name, (int) oldest, (int) record_cnt);
	if ( (oldest + record_cnt > 0) && (oldest + record_cnt <= event_src->rec_num) )
	{
	    // we are at the last record or past it, seek would fail for this
	    // so we seek to the previous and ignore it on read
	    log_debug("already at the last");
	    event_src->rec_num = oldest + record_cnt - 1;
	    event_src->ignorefirstread = TRUE;
	}
	event_src->seek = TRUE;
    }
    else
    {
	log_aprerror((apr_status_t) APR_FROM_OS_ERROR(GetLastError()),
		     "im_mseventlog_set_readfromcurrent() failed");
	im_mseventlog_error_close_src(event_src);
    }
}



static void im_mseventlog_set_readfromlast(nx_eventlog_src_t *event_src)
{
    DWORD oldest;
    DWORD record_cnt;

    ASSERT(event_src->handle != NULL);

    if ( (GetOldestEventLogRecord(event_src->handle, &oldest) == TRUE) &&
	 (GetNumberOfEventLogRecords(event_src->handle, &record_cnt) == TRUE) )
    {
	log_debug("setting readfromlast for %s, oldest is %d, number of events is %d",
		  event_src->name, (int) oldest, (int) record_cnt);
	if ( oldest + record_cnt > 0 )
	{
	    event_src->rec_num = oldest + record_cnt - 1;
	    event_src->ignorefirstread = TRUE;
	    event_src->seek = TRUE;
	}
    }
    else
    {
	log_aprerror((apr_status_t) APR_FROM_OS_ERROR(GetLastError()),
		     "im_mseventlog_set_readfromlast() failed");
	im_mseventlog_error_close_src(event_src);
    }
}



static void im_mseventlog_open_src(nx_module_t *module, nx_eventlog_src_t *event_src) 
{
    nx_im_mseventlog_conf_t *imconf;

    ASSERT(event_src != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    ASSERT(event_src->name != NULL);
    log_debug("opening %s eventlog", event_src->name);

    if ( event_src->handle != NULL )
    {
	CloseEventLog(event_src->handle);
	event_src->handle = NULL;
    }

    if ( (event_src->handle = OpenEventLog(NULL, event_src->name)) == NULL )
    {
	DWORD errorcode = GetLastError();
	
	im_mseventlog_error_close_src(event_src); // set/increase blacklist timeout
	log_aprerror((apr_status_t) APR_FROM_OS_ERROR(errorcode),
		     "Failed to open %s EventLog, will retry after %d seconds",
		     event_src->name, event_src->blacklist_interval);
    }
    else
    { // Opened OK
	if ( event_src->blacklist_interval != 0 )
	{
	    log_info("Successfully reopened %s EventLog", event_src->name);
	}
    }
	
    if ( event_src->handle != NULL )
    {
	if ( (imconf->savepos == TRUE) && (event_src->rec_num == 0) )
	{
	    char key[256];
	    uint64_t tmp = 0;
	
	    if ( apr_snprintf(key, sizeof(key), "%s_rec_num", event_src->name) < ((int) sizeof(key)) - 1 )
	    {
		if ( nx_config_cache_get_int(module->name, key, (int64_t *) &tmp) == TRUE )
		{
		    event_src->rec_num = (DWORD) tmp;
		    log_debug("got rec_num for %s: %d", key, (int) event_src->rec_num);
		    if ( event_src->rec_num > 0 )
		    {
			im_mseventlog_set_readfromcurrent(event_src);
		    }
		}
		else
		{
		    log_debug("no value found in cache for %s", key);
		}
	    }
	    else
	    {
		log_error("event source name too long: [%s]", event_src->name);
	    }
	}

	if ( (event_src->rec_num == 0) && (imconf->readfromlast == TRUE) )
	{
	    im_mseventlog_set_readfromlast(event_src);
	}
    }
}



#define IM_MSEVENTLOG_NUM_READ 50

static void im_mseventlog_read(nx_module_t *module)
{
    nx_event_t *event;
    nx_im_mseventlog_conf_t *imconf;
    nx_logdata_t *logdata;
    int evcnt = 0;
    DWORD next;
    EVENTLOGRECORD *record;
    DWORD flags;

    ASSERT(module != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;
    imconf->event = NULL;

    if ( nx_module_get_status(module) != NX_MODULE_STATUS_RUNNING )
    {
	log_debug("module %s not running, not reading any more data", module->name);
	return;
    }

    if ( imconf->currsrc == NULL )
    {
	imconf->currsrc = NX_DLIST_FIRST(imconf->event_sources);
    }
    for ( evcnt = 0; evcnt < IM_MSEVENTLOG_NUM_READ; )
    {
	if ( nx_module_get_status(module) != NX_MODULE_STATUS_RUNNING )
	{
	    break;
	}
	if ( imconf->currsrc == NULL )
	{ // end of eventlog sources list
	    break;
	}
	//log_debug("reading from %s", event_src->name);
	
	if ( (imconf->currsrc->handle == NULL) &&
	     (apr_time_now() > imconf->currsrc->blacklist_until) )
	{ // reopen eventlog
	    im_mseventlog_open_src(module, imconf->currsrc);
	}

	if ( imconf->currsrc->handle == NULL )
	{ // reopen failed, go to next source
	    imconf->currsrc = NX_DLIST_NEXT(imconf->currsrc, link);
	    continue;
	}
	if ( imconf->bytes == 0 )
	{
	    if ( imconf->currsrc->seek == TRUE )
	    {
		log_debug("resuming %s eventlog read from record number %d",
			  imconf->currsrc->name, (int) imconf->currsrc->rec_num);
		flags = EVENTLOG_FORWARDS_READ | EVENTLOG_SEEK_READ;
		imconf->currsrc->seek = FALSE;
	    }
	    else
	    {
		flags = EVENTLOG_FORWARDS_READ | EVENTLOG_SEQUENTIAL_READ;
	    }
   
	    if ( ReadEventLog(imconf->currsrc->handle, flags, imconf->currsrc->rec_num,
			      imconf->buffer, IM_MSEVENTLOG_BUFFER_LEN,
			      &(imconf->bytes), &next) == 0 )
	    {
		DWORD errorcode = GetLastError();
		if ( errorcode == ERROR_HANDLE_EOF )
		{
		    log_debug("got EOF for '%s'", imconf->currsrc->name);
		    imconf->currsrc = NX_DLIST_NEXT(imconf->currsrc, link);
		    imconf->offs = 0;
		    imconf->bytes = 0;
		    continue;
		}
		else if ( errorcode == ERROR_IO_PENDING )
		{ /* ERROR_IO_PENDING: errorcode: 997: Overlapped I/O operation is in progress.
		     ignore this error and try again later, we assume it is similar to EAGAIN */
		    log_debug("got ERROR_IO_PENDING for '%s'", imconf->currsrc->name);
		    imconf->currsrc = NX_DLIST_NEXT(imconf->currsrc, link);
		    imconf->offs = 0;
		    imconf->bytes = 0;
		    continue;
		}
		else if ( errorcode == ERROR_INVALID_PARAMETER )
		{ 
		    const char *warnmsg = "ReadFromLast is FALSE and will try to restart from the beginning. This might result in duplicated logs.";
 
		    if ( imconf->readfromlast == TRUE )
		    {
			warnmsg = "ReadFromLast is TRUE and will try to restart from the last position. This might result in uncollected logs.";
		    }
		    imconf->offs = 0;
		    imconf->bytes = 0;
		    imconf->currsrc->rec_num = 0;
		    im_mseventlog_error_close_src(imconf->currsrc);
		    log_warn("got ERROR_INVALID_PARAMETER (errorcode: %d) for the %s log, "
			     "will try to reopen in %d sec. %s", (int) errorcode, imconf->currsrc->name,
			     imconf->currsrc->blacklist_interval, warnmsg);
		}
		else if ( (errorcode == ERROR_ACCESS_DENIED) || (errorcode == RPC_S_UNKNOWN_IF) )
		{ // The eventlog subsystem may not be available and there can be other transient errors
		    const char *errcodestr = "ACCESS_DENIED";
		    
		    if ( errorcode == RPC_S_UNKNOWN_IF )
		    {
			errcodestr = "UNKNOWN_IF";
		    }
		    imconf->offs = 0;
		    imconf->bytes = 0;
		    im_mseventlog_error_close_src(imconf->currsrc);
		    log_warn("the eventlog service is possibly unavailable, " 
			     "got %s (errorcode: %d) for the %s log, will try to reopen in %d sec.",
			     errcodestr, (int) errorcode, imconf->currsrc->name,
			     imconf->currsrc->blacklist_interval);
		}
		else 
		{
		    // Try to reopen on other errors such as ERROR_INVALID_HANDLE
		    char errmsg[200];
		    const char *warnmsg = "ReadFromLast is FALSE and will try to restart from the beginning. This might result in duplicated logs.";
 
		    if ( imconf->readfromlast == TRUE )
		    {
			warnmsg = "ReadFromLast is TRUE and will try to restart from the last position. This might result in uncollected logs.";
		    }

		    apr_strerror((apr_status_t) APR_FROM_OS_ERROR(errorcode), errmsg, sizeof(errmsg));
		    imconf->offs = 0;
		    imconf->bytes = 0;
		    imconf->currsrc->rec_num = 0;
		    im_mseventlog_error_close_src(imconf->currsrc);
		    log_warn("ReadEventLog() failed for offset %d for the %s log, (errorcode: %d, reason: %s). %s",
			     (int) imconf->currsrc->rec_num, imconf->currsrc->name, 
			     (int) errorcode, errmsg, warnmsg);
		}
	    }
	    else
	    { // ReadEventLog succeeded, clear any blacklisting
		imconf->currsrc->blacklist_until = 0;
		imconf->currsrc->blacklist_interval = 0;
	    }
	}

	if ( imconf->bytes > 0 )
	{
	    ASSERT(imconf->offs < imconf->bytes);
	    record = (EVENTLOGRECORD *) (imconf->buffer + imconf->offs);

	    imconf->offs += record->Length;
	    ASSERT(imconf->offs <= imconf->bytes);

	    if ( imconf->currsrc->ignorefirstread == TRUE )
	    {
		imconf->currsrc->ignorefirstread = FALSE;
	    }
	    else
	    {
		logdata = im_mseventlog_get_logdata(module, record, imconf->currsrc);
		if ( logdata != NULL )
		{
		    nx_module_add_logdata_input(module, NULL, logdata);
		}
		evcnt++;
	    }
	    imconf->currsrc->rec_num = record->RecordNumber + 1;

	    if ( imconf->offs >= imconf->bytes )
	    {
		imconf->offs = 0;
		imconf->bytes = 0;
	    }
	}
    }

    log_debug("read %d eventlog entries", evcnt);

    if ( nx_module_get_status(module) == NX_MODULE_STATUS_RUNNING )
    {
	event = nx_event_new();
	imconf->event = event;
	event->module = module;
	event->type = NX_EVENT_READ;
	if ( evcnt < IM_MSEVENTLOG_NUM_READ )
	{
	    event->delayed = TRUE;
	    event->time = apr_time_now() + APR_USEC_PER_SEC * NX_IM_MSEVENTLOG_READ_INTERVAL;
	}
	else
	{
	    event->delayed = FALSE; // Still got events to read
	}
	event->priority = module->priority;
	nx_event_add(event);
    }
}



static void im_mseventlog_init(nx_module_t *module)
{
    nx_im_mseventlog_conf_t *imconf;

    ASSERT(module->config != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    imconf->dll_names = apr_hash_make(module->pool);
    imconf->dll_libs = apr_hash_make(module->pool);

}



static void im_mseventlog_config(nx_module_t *module)
{
    const nx_directive_t *curr;
    nx_im_mseventlog_conf_t *imconf;
    nx_eventlog_src_t *event_src;
    int num_source, i;
    const char *sources[NX_MODULE_MAX_FIELDS];

    ASSERT(module->directives != NULL);
    curr = module->directives;

    imconf = apr_pcalloc(module->pool, sizeof(nx_im_mseventlog_conf_t));
    module->config = imconf;

    while ( curr != NULL )
    {
	if ( nx_module_common_keyword(curr->directive) == TRUE )
	{
	}
	else if ( strcasecmp(curr->directive, "savepos") == 0 )
	{
	}
	else if ( strcasecmp(curr->directive, "ReadFromLast") == 0 )
	{
	}
	else if ( strcasecmp(curr->directive, "sources") == 0 )
	{
	    if ( imconf->event_sources == NULL )
	    {
		imconf->event_sources = apr_pcalloc(module->pool, sizeof(nx_eventlog_sources_t));
	    }
	    num_source = nx_module_parse_fields(sources, curr->args);
	    for ( i = 0; i < num_source; i++ )
	    {
		event_src = apr_pcalloc(module->pool, sizeof(nx_eventlog_src_t));
		event_src->name = apr_pstrdup(module->pool, sources[i]);
		NX_DLIST_INSERT_TAIL(imconf->event_sources, event_src, link);
	    }
	}
	else if ( strcasecmp(curr->directive, "UTF8") == 0 )
	{
	    nx_cfg_get_boolean(module->directives, "UTF8", &(imconf->utf8));
	    if ( imconf->utf8 == TRUE )
	    {
		if ( (imconf->convert_fields = lookup_convert_fields()) == NULL )
		{
		    nx_conf_error(curr, "xm_charconv needs to be declared for UTF-8 string support");
		}
	    }
	}
	else
	{
	    nx_conf_error(curr, "invalid keyword: %s", curr->directive);
	}
	curr = curr->next;
    }

    imconf->savepos = TRUE;
    nx_cfg_get_boolean(module->directives, "savepos", &(imconf->savepos));

    imconf->readfromlast = TRUE;
    nx_cfg_get_boolean(module->directives, "ReadFromLast", &(imconf->readfromlast));

    if ( nx_cfg_get_value(module->directives, "UTF8") == NULL )
    { // UTF8 not set explicitly
	if ( (imconf->convert_fields = lookup_convert_fields()) == NULL )
	{
	    log_debug("xm_charconv not loaded, im_mseventlog strings will be in locale charset");
	    imconf->utf8 = FALSE;
	}
	else
	{
	    imconf->utf8 = TRUE;
	}
    }
}



static void im_mseventlog_start(nx_module_t *module)
{
    nx_im_mseventlog_conf_t *imconf;
    nx_event_t *event;
    nx_eventlog_src_t *event_src;
    HKEY reg;
    DWORD num_log = 0;
    char regvalbuf[IM_MSEVENTLOG_REG_BUFFER_LEN];

    ASSERT(module->config != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    if ( imconf->event_sources == NULL )
    {
	imconf->event_sources = apr_pcalloc(module->pool, sizeof(nx_eventlog_sources_t));

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE,
			  IM_MSEVENTLOG_REGISTRY_PATH,
			  0, KEY_READ, &reg) == ERROR_SUCCESS )
	{
	    while ( RegEnumKey(reg, num_log++, regvalbuf, sizeof(regvalbuf)) == ERROR_SUCCESS )
	    {
		event_src = apr_pcalloc(module->pool, sizeof(nx_eventlog_src_t));
		event_src->name = apr_pstrdup(module->pool, regvalbuf);
		NX_DLIST_INSERT_TAIL(imconf->event_sources, event_src, link);
		im_mseventlog_open_src(module, event_src);
		log_debug("added eventlog source '%s'", event_src->name);
	    }
	    RegCloseKey(reg);
	}
	else
	{
	    mseventlog_error("Failed to get registry values from "IM_MSEVENTLOG_REGISTRY_PATH);
	}
    }
    else
    { // using the Sources directive
	for ( event_src = NX_DLIST_FIRST(imconf->event_sources);
	      event_src != NULL;
	      event_src = NX_DLIST_NEXT(event_src, link) )
	{
	    im_mseventlog_open_src(module, event_src);
	}
    }

    ASSERT(imconf->event == NULL);
    event = nx_event_new();
    imconf->event = event;
    event->module = module;
    event->delayed = FALSE; 
    event->type = NX_EVENT_READ;
    event->priority = module->priority;
    nx_event_add(event);
}



static void im_mseventlog_stop(nx_module_t *module)
{
    nx_im_mseventlog_conf_t *imconf;
    nx_eventlog_src_t *event_src;
    char key[256];

    ASSERT(module != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    if ( imconf->savepos == TRUE )
    {
	for ( event_src = NX_DLIST_FIRST(imconf->event_sources);
	      event_src != NULL;
	      event_src = NX_DLIST_NEXT(event_src, link) )
	{
	    if ( apr_snprintf(key, sizeof(key), "%s_rec_num", event_src->name) < ((int) sizeof(key)) - 1 )
	    {
		nx_config_cache_set_int(module->name, key, (int64_t) event_src->rec_num);
		log_debug("set rec_num for %s: %lld", key, (int64_t) event_src->rec_num);
	    }
	    else
	    {
		log_error("event source name too long: [%s]", event_src->name);
	    }
	    if ( event_src->handle != NULL )
	    {
		CloseEventLog(event_src->handle);
		event_src->handle = NULL;
	    }
	}
    }

    imconf->event = NULL;
}



static void im_mseventlog_pause(nx_module_t *module)
{
    nx_im_mseventlog_conf_t *imconf;

    ASSERT(module != NULL);
    ASSERT(module->config != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    if ( imconf->event != NULL )
    {
	nx_event_remove(imconf->event);
	nx_event_free(imconf->event);
	imconf->event = NULL;
    }
}



static void im_mseventlog_resume(nx_module_t *module)
{
    nx_im_mseventlog_conf_t *imconf;
    nx_event_t *event;

    ASSERT(module != NULL);
    ASSERT(module->config != NULL);

    imconf = (nx_im_mseventlog_conf_t *) module->config;

    if ( imconf->event != NULL )
    {
	nx_event_remove(imconf->event);
	nx_event_free(imconf->event);
	imconf->event = NULL;
    }
    event = nx_event_new();
    imconf->event = event;
    event->module = module;
    event->delayed = FALSE;
    event->type = NX_EVENT_READ;
    event->priority = module->priority;
    nx_event_add(event);
}



static void im_mseventlog_event(nx_module_t *module, nx_event_t *event)
{
    ASSERT(event != NULL);

    switch ( event->type )
    {
	case NX_EVENT_READ:
	    im_mseventlog_read(module);
	    break;
	default:
	    nx_panic("invalid event type: %d", event->type);
    }
}



NX_MODULE_DECLARATION nx_im_mseventlog_module =
{
    NX_MODULE_API_VERSION,
    NX_MODULE_TYPE_INPUT,
    NULL,			// capabilities
    im_mseventlog_config,	// config
    im_mseventlog_start,	// start
    im_mseventlog_stop, 	// stop
    im_mseventlog_pause,	// pause
    im_mseventlog_resume,	// resume
    im_mseventlog_init,		// init
    NULL,			// shutdown
    im_mseventlog_event,	// event
    NULL,			// info
    NULL,			// exports
};
