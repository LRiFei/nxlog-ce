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
#include "../../../common/alloc.h"
#include "../core/ctx.h"

#ifndef HAVE_WINEVT_H
 // This is a hack to let it build with MINGW32-3.4.5+MSYS
# include "winevt.h"
#endif

#include <Sddl.h>

#include "im_msvistalog.h"

#define NX_LOGMODULE NX_LOGMODULE_MODULE


#define msvistalog_throw(fmt, args...) throw((apr_status_t) APR_FROM_OS_ERROR(GetLastError()), fmt, ## args)
#define msvistalog_error(fmt, args...) log_aprerror((apr_status_t) APR_FROM_OS_ERROR(GetLastError()), fmt, ## args)

static nx_string_t *nx_logdata_set_string_from_wide(nx_logdata_t *logdata,
						    const char *key,
						    LPCWSTR value)
{
    nx_value_t *val;
    int size;

    size = WideCharToMultiByte(CP_UTF8, 0, value, -1, NULL, 0, NULL, NULL);
    if ( size == 0 )
    {
	msvistalog_throw("couldn't convert wide char to utf-8");
    }
    val = nx_value_new(NX_VALUE_TYPE_STRING);
    val->string = nx_string_new_size((size_t) size);
    if ( WideCharToMultiByte(CP_UTF8, 0, value, -1, val->string->buf,
			     (int) val->string->bufsize, NULL, NULL) == 0 )
    {
	msvistalog_throw("couldn't convert wide char to utf-8");
    }
    val->string->len = size - 1;

    //log_info("%s: %s", key, val->string->buf);
    nx_logdata_set_field_value(logdata, key, val);

    return ( val->string );
}



/*
 * This is a simple but efficient xml parser which can read data from
 * the security log in the format of <Data Name='KEY'>Value</Data>
 * The names of the fields cannot be read with EvtRendererContextUser,
 * so this xml parser is needed.
 */
// TODO: use TdhGetEventInformation() instead
// Alternatively use EvtRenderContextUser
static void im_msvistalog_parse_event_xml(nx_module_t *module,
					  EVT_HANDLE event,
					  nx_logdata_t *logdata)
{
    nx_im_msvistalog_conf_t *imconf;
    DWORD bufferused = 0;
    DWORD propertycount = 0;
    char *xmlstr = NULL;
    size_t size;
    nx_exception_t e;
    char *namestart;
    char *nameend;
    char *valstart;
    char *valend;

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    try
    {
	if ( EvtRender(NULL, event, EvtRenderEventXml, imconf->renderbufsize,
		       imconf->renderbuf, &bufferused, &propertycount) != TRUE )
	{
	    msvistalog_throw("Couldn't retrieve eventlog fields from xml, EvtRender() failed");
	}

	size = (size_t) bufferused * 3 + 1;
	xmlstr = malloc(size);
	ASSERT(xmlstr != NULL);
	
	if ( WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR) imconf->renderbuf, (int) bufferused,
				 xmlstr, (int) size, NULL, NULL) == 0 )
	{
	    msvistalog_throw("couldn't convert wide char to utf-8");
	}

	namestart = xmlstr;

	for ( ; ; )
	{
	    namestart = strstr(namestart, "<Data Name='");
	    if ( namestart == NULL )
	    {
		break;
	    }
	    namestart += sizeof("<Data Name='") - 1;

	    nameend = strstr(namestart, "'>");
	    if ( nameend == NULL )
	    {
		break;
	    }

	    valstart = nameend + sizeof("'>") - 1;
	    
	    valend = strstr(valstart, "</Data");
	    if ( valend == NULL )
	    {
		break;
	    }

	    *nameend = '\0';
	    *valend = '\0';

	    if ( (valend > valstart) && (nx_logdata_get_field(logdata, namestart) == NULL) )
	    {
		nx_logdata_set_string(logdata, namestart, valstart);
	    }
	    namestart = valend + sizeof("</Data") - 1;
	}
	free(xmlstr);
    }
    catch(e)
    {
	if ( xmlstr != NULL )
	{
	    free(xmlstr);
	    rethrow(e);
	}
    }
}


/*
 * Get the unformatted message from the event in case EvtFormatMessage fails
 */

static LPCWSTR im_msvistalog_get_userdata(nx_module_t *module,
					  EVT_HANDLE event)
{
    nx_im_msvistalog_conf_t *imconf;
    DWORD bufferused = 0;
    DWORD propertycount = 0;
    int i;

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    if ( EvtRender(imconf->renderer_user, event, EvtRenderEventValues,
		   imconf->renderbufsize_user,
		   imconf->renderbuf_user, &bufferused, &propertycount) != TRUE )
    {
	if ( ERROR_INSUFFICIENT_BUFFER == GetLastError() )
	{
	    ASSERT(bufferused > 0);
	    if ( bufferused >= 1024*1024*10 )
	    {
		throw_msg("excess size requirement (over 10Mb) by eventlog, skipping");
	    }
	    imconf->renderbuf_user = (PEVT_VARIANT) apr_palloc(module->pool, bufferused);
	    ASSERT(imconf->renderbuf_user != NULL);
	    imconf->renderbufsize_user = bufferused;
	    
	    if ( EvtRender(imconf->renderer_user, event, EvtRenderEventXml,
			   imconf->renderbufsize_user,
			   imconf->renderbuf_user, &bufferused, &propertycount) != TRUE )
	    {
		msvistalog_throw("Failed to retrieve eventlog user data");
	    }
	}
    }

    if ( (propertycount == 1) && 
	 (imconf->renderbuf_user[i].Type == EvtVarTypeString) )
    {
	return ( imconf->renderbuf_user[i].StringVal );
    }
    
    return ( NULL );
}



static nx_msvistalog_publisher_t *im_msvistalog_get_message(nx_module_t *module,
							    EVT_HANDLE event,
							    nx_logdata_t *logdata,
							    LPCWSTR publishername,
							    const char *sourcename,
							    int eventid)
{
    nx_im_msvistalog_conf_t *imconf;
    DWORD bufferused = 0;
    const nx_string_t *message = NULL;
    const char *errorstr = NULL;
    nx_msvistalog_publisher_t *publisher = NULL;
    DWORD errcode = 0;

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    publisher = apr_hash_get(imconf->publisherhash, sourcename, APR_HASH_KEY_STRING);
    if ( publisher == NULL )
    {
	publisher = apr_pcalloc(imconf->publisherpool, sizeof(nx_msvistalog_publisher_t));
	publisher->handle = EvtOpenPublisherMetadata(imconf->session, publishername, NULL, 0, 0);
	// set even if it is NULL
	apr_hash_set(imconf->publisherhash, apr_pstrdup(imconf->publisherpool, sourcename),
		     APR_HASH_KEY_STRING, publisher);
	NX_DLIST_INSERT_TAIL(imconf->publisherlist, publisher, link);
    }

    if ( publisher->handle == NULL )
    {
	errorstr = "failed to open publisher metadata";
    }

    if ( EvtFormatMessage(publisher->handle, event, 0, 0, NULL, EvtFormatMessageEvent,
			  imconf->renderbufsize, (LPWSTR) imconf->renderbuf, &bufferused) != TRUE )
    {
	LPCWSTR userdata;
	errcode = GetLastError();

	switch ( errcode )
	{
	    case 15029: //ERROR_EVT_UNRESOLVED_VALUE_INSERT
	    case 15030: //ERROR_EVT_UNRESOLVED_PARAMETER_INSERT
		userdata = im_msvistalog_get_userdata(module, event);
		if ( userdata != NULL )
		{
		    message = nx_logdata_set_string_from_wide(logdata, "Message", userdata);
		}
		break;
	    default:
		break;
	}
	//msvistalog_error("Couldn't format message for EventID %d", eventid);
    }
    else
    {
	message = nx_logdata_set_string_from_wide(logdata, "Message", (LPWSTR) imconf->renderbuf);
	//log_info("%s", message->buf);
    }

    if ( message != NULL )
    {
	nx_string_append(logdata->raw_event, message->buf, (int) message->len);
    }
    else
    {
	char msg[256];
	int len;
	char errmsg[512];

	if ( errcode != 0 )
	{
	    apr_strerror((int) APR_FROM_OS_ERROR(errcode), errmsg, sizeof(errmsg));
	    errorstr = errmsg;
	}

	if ( errorstr != NULL )
	{
	    len = apr_snprintf(msg, sizeof(msg), "[The description for EventID %d from source %s cannot be found: %s]",
			       eventid, sourcename, errorstr);
	}
	else
	{
	    len = apr_snprintf(msg, sizeof(msg), "[The description for EventID %d from source %s cannot be found",
			       eventid, sourcename);
	}
	nx_string_append(logdata->raw_event, msg, len);
    }

    return ( publisher );
}



static void im_msvistalog_event_to_logdata(nx_module_t *module,
					   EVT_HANDLE event)
{
    nx_logdata_t *logdata = NULL;
    nx_im_msvistalog_conf_t *imconf;
    DWORD bufferused = 0;
    DWORD propertycount = 0;
    nx_exception_t e;
    WCHAR wguid[50];
    DWORD EventID;
    char user[256];
    char domain[256];
    DWORD user_len = sizeof(user) - 1;
    DWORD domain_len = sizeof(domain) - 1;
    SID_NAME_USE account_type;
    LPCWSTR publishername = NULL;
    const nx_string_t *sourcename = NULL;
    int64_t level = WINEVENT_INFORMATION_TYPE;
    nx_loglevel_t loglevel = NX_LOGLEVEL_INFO;
    int64_t keywords = 0;
    char tmpstr[30];
    WCHAR tmpwstr[128];
    apr_time_t eventtime;
    nx_string_t *hostname = NULL;
    char eventidstr[32];
    nx_msvistalog_publisher_t *publisher;

    ASSERT(event != NULL);
    imconf = (nx_im_msvistalog_conf_t *) module->config;

    logdata = nx_logdata_new();

    try
    {
	// Render System data
	if ( EvtRender(imconf->renderer_system, event, EvtRenderEventValues, imconf->renderbufsize,
		       imconf->renderbuf, &bufferused, &propertycount) != TRUE )
	{
	    if ( ERROR_INSUFFICIENT_BUFFER == GetLastError() )
	    {
		ASSERT(bufferused > 0);
		if ( bufferused >= 1024*1024*10 )
		{
		    throw_msg("excess size requirement (over 10Mb) by eventlog, skipping");
		}
		imconf->renderbuf = (PEVT_VARIANT) apr_palloc(module->pool, bufferused);
		ASSERT(imconf->renderbuf != NULL);
		imconf->renderbufsize = bufferused;
	    
		if ( EvtRender(imconf->renderer_system, event, EvtRenderEventValues, imconf->renderbufsize,
			       imconf->renderbuf, &bufferused, &propertycount) != TRUE )
		{
		    msvistalog_throw("Failed to retrieve eventlog fields");
		}
	    }
	    else
	    {
		msvistalog_throw("Failed to retrieve eventlog fields");
	    }
	}

	// EventTime
	eventtime = (apr_time_t) (imconf->renderbuf[EvtSystemTimeCreated].FileTimeVal);
	eventtime = (DWORD)((imconf->renderbuf[EvtSystemTimeCreated].FileTimeVal >> 32) & 0xFFFFFFFF);
	eventtime = eventtime << 32;
	eventtime |= (DWORD)(imconf->renderbuf[EvtSystemTimeCreated].FileTimeVal & 0xFFFFFFFF);
	eventtime /= 10;    /* Convert from 100 nano-sec periods to micro-seconds. */
	eventtime -= APR_TIME_C(11644473600000000);  /* Convert from Windows epoch to Unix epoch */
	nx_logdata_set_datetime(logdata, "EventTime", eventtime);
	nx_date_to_iso(tmpstr, sizeof(tmpstr), eventtime);
	nx_string_append(logdata->raw_event, tmpstr, -1);
	nx_string_append(logdata->raw_event, " ", 1);

	if ( imconf->renderbuf[EvtSystemComputer].Type != EvtVarTypeNull )
	{
	    hostname = nx_logdata_set_string_from_wide(logdata, "Hostname",
						       imconf->renderbuf[EvtSystemComputer].StringVal);
	    if ( hostname->len == 0 )
	    {
		nx_string_free(hostname);
		hostname = NULL;
	    }
	    else
	    {
		nx_string_append(logdata->raw_event, hostname->buf, (int) hostname->len);
	    }
	}
	if ( hostname == NULL )
	{
	    const nx_string_t *hoststr;
	    
	    hoststr = nx_get_hostname();
	    nx_string_append(logdata->raw_event, hoststr->buf, (int) hoststr->len);
	}
	nx_string_append(logdata->raw_event, " ", 1);

	if ( imconf->renderbuf[EvtSystemKeywords].Type != EvtVarTypeNull )
	{
	    keywords = (int64_t) imconf->renderbuf[EvtSystemKeywords].UInt64Val;
	    nx_logdata_set_integer(logdata, "Keywords", keywords);
	}

	if ( imconf->renderbuf[EvtSystemLevel].Type != EvtVarTypeNull )
	{
	    level = (int64_t) imconf->renderbuf[EvtSystemLevel].ByteVal;
	}

	switch ( level )
	{
	    case WINEVENT_CRITICAL_TYPE:
		nx_string_append(logdata->raw_event, "CRITICAL", -1);
		nx_logdata_set_string(logdata, "EventType", "CRITICAL");
		loglevel = NX_LOGLEVEL_CRITICAL;
		nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		break;
	    case WINEVENT_ERROR_TYPE:
		nx_string_append(logdata->raw_event, "ERROR", -1);
		nx_logdata_set_string(logdata, "EventType", "ERROR");
		loglevel = NX_LOGLEVEL_ERROR;
		nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		break;
	    case WINEVENT_WARNING_TYPE:
		nx_string_append(logdata->raw_event, "WARNING", -1);
		nx_logdata_set_string(logdata, "EventType", "WARNING");
		loglevel = NX_LOGLEVEL_WARNING;
		nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		break;
	    case WINEVENT_VERBOSE_TYPE:
		nx_string_append(logdata->raw_event, "DEBUG", -1);
		nx_logdata_set_string(logdata, "EventType", "VERBOSE");
		loglevel = NX_LOGLEVEL_DEBUG;
		nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		break;
	    case WINEVENT_AUDIT_TYPE:
		if ( keywords & WINEVENT_AUDIT_FAILURE )
		{
		    nx_string_append(logdata->raw_event, "AUDIT_FAILURE", -1);
		    nx_logdata_set_string(logdata, "EventType", "AUDIT_FAILURE");
		    loglevel = NX_LOGLEVEL_ERROR;
		    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		    break;
		}
		else if ( keywords & WINEVENT_AUDIT_SUCCESS )
		{
		    nx_string_append(logdata->raw_event, "AUDIT_SUCCESS", -1);
		    nx_logdata_set_string(logdata, "EventType", "AUDIT_SUCCESS");
		    loglevel = NX_LOGLEVEL_INFO;
		    nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		    break;
		}
		// else fallthrough
	    default:
		//log_debug("unknown eventtype: %d", record->EventType);
		// fallthrough
	    case WINEVENT_INFORMATION_TYPE:
		nx_string_append(logdata->raw_event, "INFO", -1);
		nx_logdata_set_string(logdata, "EventType", "INFO");
		loglevel = NX_LOGLEVEL_INFO;
		nx_logdata_set_integer(logdata, "SeverityValue", (int64_t) loglevel);
		break;
	}
	nx_logdata_set_string(logdata, "Severity", nx_loglevel_to_string(loglevel));
	nx_string_append(logdata->raw_event, " ", 1);

	EventID = imconf->renderbuf[EvtSystemEventID].UInt16Val;
	/* The following is not needed
	if ( imconf->renderbuf[EvtSystemQualifiers].Type != EvtVarTypeNull )
	{
	    EventID = MAKELONG(imconf->renderbuf[EvtSystemEventID].UInt16Val,
			       imconf->renderbuf[EvtSystemQualifiers].UInt16Val);
	}
	*/
	nx_logdata_set_integer(logdata, "EventID", (int64_t) EventID);

	apr_snprintf(eventidstr, sizeof(eventidstr), "%d", (int) (EventID & 0x0000FFFF));
	nx_string_append(logdata->raw_event, eventidstr, -1);
	nx_string_append(logdata->raw_event, " ", 1);


/* For testing
	if ( EventID != 4624 ) throw_msg("other event");
	else log_info("Got event");
*/
	sourcename = nx_logdata_set_string_from_wide(logdata, "SourceName",
						     imconf->renderbuf[EvtSystemProviderName].StringVal);
	publishername = imconf->renderbuf[EvtSystemProviderName].StringVal;

	if ( imconf->renderbuf[EvtSystemProviderGuid].GuidVal != NULL )
	{
	    // this requires ole32.dll
	    StringFromGUID2(imconf->renderbuf[EvtSystemProviderGuid].GuidVal, wguid,
			    sizeof(wguid) / sizeof(WCHAR));
	    nx_logdata_set_string_from_wide(logdata, "ProviderGuid", wguid);
	}

	if ( imconf->renderbuf[EvtSystemVersion].Type != EvtVarTypeNull )
	{
	    nx_logdata_set_integer(logdata, "Version",
				   (int64_t) imconf->renderbuf[EvtSystemVersion].ByteVal);
	}

	if ( imconf->renderbuf[EvtSystemTask].Type != EvtVarTypeNull )
	{
	    nx_logdata_set_integer(logdata, "Task",
				   (int64_t) imconf->renderbuf[EvtSystemTask].UInt16Val);
	}

	if ( imconf->renderbuf[EvtSystemOpcode].Type != EvtVarTypeNull )
	{
	    nx_logdata_set_integer(logdata, "OpcodeValue",
				   (int64_t) imconf->renderbuf[EvtSystemOpcode].ByteVal);
	}

	nx_logdata_set_integer(logdata, "RecordNumber",
			       (int64_t) imconf->renderbuf[EvtSystemEventRecordId].UInt64Val);

	if ( imconf->renderbuf[EvtSystemActivityID].GuidVal != NULL )
	{
	    // this requires ole32.dll
	    StringFromGUID2(imconf->renderbuf[EvtSystemActivityID].GuidVal, wguid,
			    sizeof(wguid) / sizeof(WCHAR));
	    nx_logdata_set_string_from_wide(logdata, "ActivityID", wguid);
	}

	if ( imconf->renderbuf[EvtSystemRelatedActivityID].GuidVal != NULL )
	{
	    // this requires ole32.dll
	    StringFromGUID2(imconf->renderbuf[EvtSystemRelatedActivityID].GuidVal, wguid,
			    sizeof(wguid) / sizeof(WCHAR));
	    nx_logdata_set_string_from_wide(logdata, "RelatedActivityID", wguid);
	}

	nx_logdata_set_integer(logdata, "ProcessID",
			       (int64_t) imconf->renderbuf[EvtSystemProcessID].UInt32Val);

	nx_logdata_set_integer(logdata, "ThreadID",
			       (int64_t) imconf->renderbuf[EvtSystemThreadID].UInt32Val);

	if ( imconf->renderbuf[EvtSystemChannel].Type != EvtVarTypeNull )
	{
	    nx_logdata_set_string_from_wide(logdata, "Channel",
					    imconf->renderbuf[EvtSystemChannel].StringVal);
	}

	if ( imconf->renderbuf[EvtSystemUserID].Type != EvtVarTypeNull )
	{
	    LPTSTR sidstr = NULL;

	    if ( LookupAccountSid(NULL, imconf->renderbuf[EvtSystemUserID].SidVal,
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
	    }

	    if (ConvertSidToStringSid(imconf->renderbuf[EvtSystemUserID].SidVal, &sidstr))
	    {
		nx_logdata_set_string(logdata, "UserID", user);
		LocalFree(sidstr);
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

	publisher = im_msvistalog_get_message(module, event, logdata, publishername,
					      sourcename->buf, (int) EventID);
	if ( publisher->handle != NULL )
	{
	    // Look up Category
	    if ( EvtFormatMessage(publisher->handle, event,
				  imconf->renderbuf[EvtSystemTask].UInt16Val,
				  0, NULL, EvtFormatMessageTask,
				  sizeof(tmpwstr) / sizeof(WCHAR), tmpwstr, &bufferused) == TRUE )
	    {
		nx_logdata_set_string_from_wide(logdata, "Category", tmpwstr);
	    }
	    // Look up Opcode
	    if ( EvtFormatMessage(publisher->handle, event,
				  imconf->renderbuf[EvtSystemOpcode].ByteVal,
				  0, NULL, EvtFormatMessageOpcode,
				  sizeof(tmpwstr) / sizeof(WCHAR), tmpwstr, &bufferused) == TRUE )
	    {
		nx_logdata_set_string_from_wide(logdata, "Opcode", tmpwstr);
	    }
	}
	im_msvistalog_parse_event_xml(module, event, logdata);
    }
    catch(e)
    {
	log_exception(e);
	nx_logdata_free(logdata);
	return;
    }

    nx_module_add_logdata_input(module, NULL, logdata);
}



LPWSTR mbtowide(apr_pool_t *pool, const char *str)
{
    int size;
    LPWSTR retval = NULL;

    ASSERT(str != NULL);

    size = MultiByteToWideChar(0, 0, str, -1, NULL, 0);
    if ( size == 0 )
    {
	msvistalog_throw("_MultiByteToWideChar failed for '%s'", str);
    }
    retval = apr_palloc(pool, (apr_size_t) (size * sizeof(WCHAR)));
    if ( MultiByteToWideChar(0, 0, str, -1, retval, size) == 0 )
    {
	msvistalog_throw("MultiByteToWideChar failed for '%s'", str);
    }

    return ( retval );
}



const char *widetoutf8(apr_pool_t *pool, LPCWSTR str)
{
    int size;
    char *retval = NULL;

    ASSERT(str != NULL);

    size = WideCharToMultiByte(CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL);
    if ( size == 0 )
    {
	msvistalog_throw("couldn't convert wide char to utf-8");
    }

    retval = apr_palloc(pool, (apr_size_t) size);
    if ( WideCharToMultiByte(CP_UTF8, 0, str, -1, retval, size, NULL, NULL) == 0 )
    {
	msvistalog_throw("couldn't convert wide char to utf-8");
    }

    return ( retval );
}



static void im_msvistalog_read(nx_module_t *module)
{
    nx_event_t *event;
    nx_im_msvistalog_conf_t *imconf;
    DWORD evcnt = 0;
    int i;
    DWORD status;

    ASSERT(module != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;
    imconf->event = NULL;

    if ( nx_module_get_status(module) != NX_MODULE_STATUS_RUNNING )
    {
	log_debug("module %s not running, not reading any more data", module->name);
	return;
    }

    log_debug("im_msvistalog checking for new events...");

    memset(imconf->eventarray, 0, sizeof(imconf->eventarray));
    if ( EvtNext(imconf->subscription, IM_MSVISTALOG_BATCH_SIZE,
		 imconf->eventarray, 500, 0, &evcnt ) != TRUE )
    {
	switch ( status = GetLastError() )
	{
	    case ERROR_NO_MORE_ITEMS:
	    case ERROR_INVALID_OPERATION:
		ASSERT(evcnt == 0);
		break;
	    case ERROR_TIMEOUT:
		ASSERT(evcnt == 0);
		//msvistalog_error("EvtNext timeout");
		break;
	    case ERROR_INVALID_HANDLE:
		nx_module_stop_self(module);
		nx_module_start(module);
		SetLastError(status);
		msvistalog_throw("Couldn't read next event, invalid handle");
	    case ERROR_INVALID_DATA:
		SetLastError(status);
		msvistalog_throw("Couldn't read next event, corrupted eventlog?");
	    default:
		msvistalog_error("EvtNext failed with error %d", (int) status);
		nx_module_stop_self(module);
		nx_module_start(module);
		break;
	}
    }
    log_debug("im_msvistalog read %d events", (int) evcnt);

    if ( evcnt > 0 )
    {
	if ( imconf->last_event != NULL )
	{
	    EvtClose(imconf->last_event);
	}
	// save the last event for bookmarking it later in module_stop
	imconf->last_event = imconf->eventarray[evcnt - 1];
	for ( i = 0; i < (int) evcnt; i++ )
	{
	    if ( imconf->eventarray[i] != NULL )
	    {
		im_msvistalog_event_to_logdata(module, imconf->eventarray[i]);
		if ( i < (int) evcnt - 1 )
		{
		    EvtClose(imconf->eventarray[i]);
		}
	    }
	}
    }

    if ( nx_module_get_status(module) == NX_MODULE_STATUS_RUNNING )
    {
	event = nx_event_new();
	imconf->event = event;
	event->module = module;
	event->type = NX_EVENT_READ;
	if ( evcnt < IM_MSVISTALOG_BATCH_SIZE )
	{
	    event->delayed = TRUE;
	    event->time = apr_time_now() + (int) APR_USEC_PER_SEC * imconf->poll_interval;
	}
	else
	{
	    event->delayed = FALSE; // Still got events to read
	}
	event->priority = module->priority;
	nx_event_add(event);
    }
}



static void im_msvistalog_init(nx_module_t *module)
{
    nx_im_msvistalog_conf_t *imconf;

    ASSERT(module->config != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;
}



static void im_msvistalog_config(nx_module_t *module)
{
    const nx_directive_t *curr;
    nx_im_msvistalog_conf_t *imconf;

    ASSERT(module->directives != NULL);
    curr = module->directives;

    imconf = apr_pcalloc(module->pool, sizeof(nx_im_msvistalog_conf_t));
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
	else if ( strcasecmp(curr->directive, "Query") == 0 )
	{
	    if ( imconf->query != NULL )
	    {
		nx_conf_error(curr, "Query is already defined");
	    }
	    imconf->query = apr_pstrdup(module->pool, curr->args);
	}
	else if ( strcasecmp(curr->directive, "Channel") == 0 )
	{
	    if ( imconf->channel != NULL )
	    {
		nx_conf_error(curr, "Channel is already defined");
	    }
	    imconf->channel = apr_pstrdup(module->pool, curr->args);
	}
	else if ( strcasecmp(curr->directive, "PollInterval") == 0 )
	{
	    if ( sscanf(curr->args, "%f", &(imconf->poll_interval)) != 1 )
	    {
		nx_conf_error(curr, "invalid PollInterval: %s", curr->args);
            }
	}
	else
	{
	    nx_conf_error(curr, "invalid keyword: %s", curr->directive);
	}
	curr = curr->next;
    }
    if ( imconf->channel != NULL )
    {
	imconf->_path = mbtowide(module->pool, imconf->channel);
    }

    if ( imconf->query != NULL )
    {
	imconf->_query = mbtowide(module->pool, imconf->query);
    }

    if ( imconf->poll_interval == 0 )
    {
	imconf->poll_interval = IM_MSVISTALOG_DEFAULT_POLL_INTERVAL;
    }

    imconf->savepos = TRUE;
    nx_cfg_get_boolean(module->directives, "savepos", &(imconf->savepos));
    imconf->readfromlast = TRUE;
    nx_cfg_get_boolean(module->directives, "ReadFromLast", &(imconf->readfromlast));

    imconf->renderbuf = (PEVT_VARIANT) apr_palloc(module->pool, IM_MSVISTALOG_RENDER_BUFFER_SIZE); 
    imconf->renderbufsize = IM_MSVISTALOG_RENDER_BUFFER_SIZE;

    imconf->renderbuf_user = (PEVT_VARIANT) apr_palloc(module->pool, IM_MSVISTALOG_RENDER_BUFFER_SIZE); 
    imconf->renderbufsize_user = IM_MSVISTALOG_RENDER_BUFFER_SIZE;
}



static void dump_query_xml(const char *xml)
{
    nx_ctx_t *ctx;

    ctx = nx_ctx_get();
    if ( ctx->logfile != NULL )
    {
	apr_file_write_full(ctx->logfile, xml, strlen(xml), NULL);
    }
}



/**
 * There are still cases when EvtSubscribe will fail even if the source
 * isn't Analytic or Debug.
 * Instead of doing such heuristics we check directly by trying to subscribe
 * to that single source.
 */
static boolean _source_subscription_ok(EVT_HANDLE session,
				       nx_module_t *module,
				       const char *source)
{
    boolean retval = TRUE;
    EVT_HANDLE waithandle;
    EVT_HANDLE subscription;
    LPCWSTR _query;
    apr_pool_t *pool;
    const char *query = NULL;
    EVT_SUBSCRIBE_FLAGS flags;
    nx_im_msvistalog_conf_t *imconf;
	
    imconf = (nx_im_msvistalog_conf_t *) module->config;

    pool = nx_pool_create_child(module->pool);

    waithandle = CreateEvent(NULL, TRUE, TRUE, NULL);

    query = apr_pstrdup(pool, "<QueryList>");
    query = apr_pstrcat(pool, query, source, NULL);
    query = apr_pstrcat(pool, query, "</QueryList>", NULL);
    _query = mbtowide(pool, query);

    if ( imconf->readfromlast == TRUE )
    {
	flags = EvtSubscribeToFutureEvents;
    }
    else
    {
	flags = EvtSubscribeStartAtOldestRecord;
    }
    subscription = EvtSubscribe(session, waithandle, NULL, _query,
				NULL, NULL, NULL, flags);
    if ( subscription == NULL )
    { // failed to subscribe
	DWORD status;
	
	retval = FALSE;
	status = GetLastError();
	log_warn("ignoring source as it cannot be subscribed to (error code: %d): %s",
		 (int) status, source);
    }
    else
    {
	EvtClose(subscription);
    }
    EvtClose(waithandle);
    apr_pool_destroy(pool);

    return ( retval );
}



static const char *get_query_xml(EVT_HANDLE session, nx_module_t *module, apr_pool_t *pool)
{
    EVT_HANDLE channels = NULL;
    EVT_HANDLE channelcfg = NULL;
    WCHAR name[256];
    const char *nameutf8;
    DWORD used = 0;
    nx_exception_t e;
    boolean enabled;
    uint8_t propbuf[16];
    PEVT_VARIANT property = (PEVT_VARIANT) propbuf;
    EVT_CHANNEL_TYPE type;
    int query_id = 0;
    const char *tmpstr;
    const char *query;
    nx_im_msvistalog_conf_t *imconf;
    DWORD status;

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    // Event Selection
    // http://msdn.microsoft.com/en-us/library/aa385231.aspx
    query = apr_pstrdup(pool, "<QueryList>");

    try
    {
	if ( (channels = EvtOpenChannelEnum(session, 0)) == NULL )
	{
	    msvistalog_throw("Failed to query available channels");
	}

	for ( ; ; )
	{
	    if ( EvtNextChannelPath(channels, sizeof(name)/sizeof(WCHAR), name, &used) == TRUE )
	    {
		nameutf8 = widetoutf8(pool, name);
		if ( (channelcfg = EvtOpenChannelConfig(session, name, 0)) == NULL )
		{
		    // Security event log gives Access Denied when running under a service account
		    log_debug("EvtOpenChannelConfig failed for %s (errorcode: %d)",
			      nameutf8, (int) GetLastError());
		    tmpstr = apr_psprintf(pool, "<Query Id='%d'><Select Path='%s'>*</Select></Query>",
					  query_id, nameutf8);
		    query_id++;
		    
		    if ( _source_subscription_ok(session, module, tmpstr) == TRUE )
		    {
			query = apr_pstrcat(pool, query, tmpstr, NULL);
		    }
		}
		else
		{
		    if ( EvtGetChannelConfigProperty(channelcfg, EvtChannelConfigEnabled,
						     0, sizeof(propbuf), property, &used) != TRUE )
		    {
			msvistalog_throw("EvtGetChannelConfigProperty(Enabled) failed");
		    }
		    if ( property->BooleanVal == FALSE )
		    {
			enabled = FALSE;
		    }
		    else
		    {
			enabled = TRUE;
		    }
		    
		    if ( EvtGetChannelConfigProperty(channelcfg, EvtChannelConfigType,
						     0, sizeof(propbuf), property, &used) != TRUE )
		    {
			msvistalog_throw("EvtGetChannelConfigProperty(Type) failed");
		    }
		    type = (EVT_CHANNEL_TYPE) property->UInt32Val;

		    if ( (enabled == TRUE) &&
			 ((type == EvtChannelTypeAdmin) || (type == EvtChannelTypeOperational)) )
		    {
			tmpstr = apr_psprintf(pool, "<Query Id='%d'><Select Path='%s'>*</Select></Query>",
					      query_id, nameutf8);
			query_id++;

			if ( _source_subscription_ok(session, module, tmpstr) == TRUE )
			{
			    query = apr_pstrcat(pool, query, tmpstr, NULL);
			}
		    }
		}
		if ( channelcfg != NULL )
		{
		    EvtClose(channelcfg);
		}
	    }
	    else
	    {
		if ( GetLastError() == ERROR_NO_MORE_ITEMS )
		{
		    break;
		}
		else
		{
		    msvistalog_throw("EvtNextPublisherId failed");
		}
	    }
	}
    }
    catch(e)
    {
	if ( channels != NULL )
	{
	    EvtClose(channels);
	}
	if ( channelcfg != NULL )
	{
	    EvtClose(channelcfg);
	}
	rethrow(e);
    }
    if ( channels != NULL )
    {
	EvtClose(channels);
    }

    query = apr_pstrcat(pool, query, "</QueryList>", NULL);

    return ( query );
}



static void im_msvistalog_start(nx_module_t *module)
{
    nx_im_msvistalog_conf_t *imconf;
    nx_event_t *event;
    apr_pool_t *pool;
    const char *query = NULL;
    LPCWSTR _query;
    nx_exception_t e;
    EVT_HANDLE bookmark = NULL;
    boolean needseek = TRUE;

    ASSERT(module->config != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    pool = nx_pool_create_child(module->pool);
    
    log_debug("initiating eventlog collection process");

    try
    {
	if ( imconf->publisherpool == NULL )
	{
	    imconf->publisherpool = nx_pool_create_child(module->pool);
	    imconf->publisherlist = apr_pcalloc(imconf->publisherpool, sizeof(nx_msvistalog_publishers_t));
	    imconf->publisherhash = apr_hash_make(imconf->publisherpool);
	}

	if ( (imconf->query == NULL) && (imconf->channel == NULL) )
	{ // no parameters provided so we look up all available channels and build a query xml
	    _query = mbtowide(pool, get_query_xml(imconf->session, module, pool));
	}
	else
	{
	    _query = imconf->_query;
	    if ( imconf->query != NULL )
	    {
		log_debug("msvistalog query xml: %s", imconf->query);
	    }
	}
    
	imconf->waithandle = CreateEvent(NULL, TRUE, TRUE, NULL);
	if ( imconf->waithandle == NULL )
	{
	    msvistalog_throw("CreateEvent failed");
	}

	if ( imconf->savepos == TRUE )
	{ // load bookmark
	    const char *bookmarkxml = NULL;
	    LPCWSTR _bookmarkxml;

	    if ( nx_config_cache_get_string(module->name, "bookmark", &bookmarkxml) == TRUE )
	    {
		_bookmarkxml = mbtowide(pool, bookmarkxml);
		bookmark = EvtCreateBookmark(_bookmarkxml);
		if ( bookmark == NULL )
		{
		    msvistalog_error("failed to restore the saved position from bookmark xml: [%s]", bookmarkxml);
		}
		else
		{
		    imconf->subscription = EvtSubscribe(imconf->session,
							imconf->waithandle, imconf->_path, _query,
							bookmark, NULL, NULL,
							EvtSubscribeStartAfterBookmark);
		    if ( imconf->subscription == NULL )
		    {
			msvistalog_error("failed to subscribe to msvistalog events using bookmark");
		    }
		    else
		    {
			needseek = FALSE;
			log_debug("im_msvistalog subscribed successfully using a bookmark");
		    }
		}
	    }
	}
	if ( needseek == TRUE )
	{
	    EVT_SUBSCRIBE_FLAGS flags;

	    if ( imconf->readfromlast == TRUE )
	    {
		flags = EvtSubscribeToFutureEvents;
	    }
	    else
	    {
		flags = EvtSubscribeStartAtOldestRecord;
	    }
	    imconf->subscription = EvtSubscribe(imconf->session, 
						imconf->waithandle, imconf->_path, _query,
						NULL, NULL, NULL, flags);
	    if ( imconf->subscription == NULL )
	    {
		WCHAR _qerror[256];
		char qerror[256];
		DWORD bufused;
		DWORD status;

		switch ( status = GetLastError() )
		{
		    case 5: //ACCESS_DENIED
			msvistalog_throw("failed to subscribe to msvistalog events,"
					 "access denied [%d]", (int) status);
		    case 15009: //ERROR_EVT_SUBSCRIPTION_TO_DIRECT_CHANNEL
			msvistalog_throw("failed to subscribe to msvistalog events,"
					 "direct channel (analytic and debug) subscription is not supported [%d]", (int) status);
		    case 15007: //ERROR_EVT_CHANNEL_NOT_FOUND
			msvistalog_throw("failed to subscribe to msvistalog events,"
					 "the channel was not found [%d]", (int) status);
		    case 15001: //ERROR_EVT_INVALID_QUERY
		    default:
			memset(_qerror, 0, sizeof(_qerror));
			memset(_qerror, 0, sizeof(qerror));
			if ( query != NULL )
			{
			    dump_query_xml(query);
			}
			if ( EvtGetExtendedStatus(sizeof(_qerror), _qerror, &bufused) == ERROR_SUCCESS )
			{
			    if ( WideCharToMultiByte(CP_UTF8, 0, _qerror, -1, qerror,
						     sizeof(qerror), NULL, NULL) > 0 )
			    {
				SetLastError(status);
				throw_msg("failed to subscribe to msvistalog events,"
					  "the Query is invalid: %s [%d]", qerror,
					  (int) status);
			    }
			}
			SetLastError(status);
			msvistalog_throw("failed to subscribe to msvistalog events "
					 "[error: %d]", (int) status);
		}
	    }
	    log_debug("im_msvistalog subscribed successfully using a query/channel");
	}

	imconf->renderer_system = EvtCreateRenderContext(0, NULL, EvtRenderContextSystem);
	imconf->renderer_user = EvtCreateRenderContext(0, NULL, EvtRenderContextUser);
	if ( (imconf->renderer_system == NULL) || (imconf->renderer_user == NULL) )
	{
	    msvistalog_throw("EvtCreateRenderContext failed");
	}

	apr_pool_destroy(pool);
    }
    catch(e)
    {
	if ( bookmark != NULL )
	{
	    EvtClose(bookmark);
	}
	if ( imconf->renderer_system != NULL )
	{
	    EvtClose(imconf->renderer_system);
	}
	if ( imconf->renderer_user != NULL )
	{
	    EvtClose(imconf->renderer_user);
	}
	apr_pool_destroy(pool);
	rethrow(e);
    }

    ASSERT(imconf->event == NULL);
    event = nx_event_new();
    imconf->event = event;
    event->module = module;
    event->delayed = FALSE; 
    event->type = NX_EVENT_READ;
    event->priority = module->priority;
    nx_event_add(event);

    log_debug("im_msvistalog instance %s started", module->name);
}



static void im_msvistalog_stop(nx_module_t *module)
{
    nx_im_msvistalog_conf_t *imconf;
    EVT_HANDLE bookmark = NULL;
    DWORD size;
    LPWSTR _bookmarkxml = NULL;
    const char *bookmarkxml = NULL;
    nx_msvistalog_publisher_t *publisher;
    apr_pool_t *pool = NULL;

    ASSERT(module != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    if ( imconf->savepos == TRUE )
    {
	// save bookmark
	if ( imconf->last_event != NULL )
	{
	    bookmark = EvtCreateBookmark(NULL);
	    if ( bookmark == NULL )
	    {
		msvistalog_error("EvtCreateBookmark failed");
	    }
	    else
	    {
		if ( EvtUpdateBookmark(bookmark, imconf->last_event) != TRUE )
		{
		    msvistalog_error("EvtUpdateBookmark failed");
		}
		else
		{
		    if ( EvtRender(NULL, bookmark, EvtRenderBookmark, 0, NULL, &size, 0) != TRUE )
		    {
			if ( GetLastError() != ERROR_INSUFFICIENT_BUFFER )
			{
			    msvistalog_error("failed to calculate bookmarkxml size");
			}
			else
			{
			    pool = nx_pool_create_child(module->pool);
			    _bookmarkxml = apr_pcalloc(pool, (size_t) size);
			    ASSERT(_bookmarkxml != NULL);
			    if ( EvtRender(NULL, bookmark, EvtRenderBookmark, size,
					   _bookmarkxml, &size, 0) != TRUE )
			    {
				msvistalog_error("failed to render bookmark");
			    }
			    else
			    {
				bookmarkxml = widetoutf8(pool, _bookmarkxml);
				nx_config_cache_set_string(module->name, "bookmark", bookmarkxml);
			    }
			    apr_pool_destroy(pool);
			}
		    }
		}
	    }
	}
	if ( bookmark != NULL )
	{
	    EvtClose(bookmark);
	}
    }

    if ( imconf->renderer_system != NULL )
    {
	EvtClose(imconf->renderer_system);
    }

    if ( imconf->renderer_user != NULL )
    {
	EvtClose(imconf->renderer_user);
    }
    
    if ( imconf->last_event != NULL )
    {
	EvtClose(imconf->last_event);
    }

    for ( publisher = NX_DLIST_FIRST(imconf->publisherlist);
	  publisher != NULL;
	  publisher = NX_DLIST_NEXT(publisher, link) )
    {
	if ( publisher->handle != NULL )
	{
	    EvtClose(publisher->handle);
	}
    }

    if ( imconf->publisherpool != NULL )
    {
	apr_pool_destroy(imconf->publisherpool);
	imconf->publisherpool = NULL;
    }

    if ( imconf->subscription != NULL )
    {
	EvtClose(imconf->subscription);
    }
    if ( imconf->waithandle != NULL )
    {
	EvtClose(imconf->waithandle);
    }
    if ( imconf->session != NULL )
    {
	EvtClose(imconf->session);
    }
    imconf->event = NULL;
}



static void im_msvistalog_pause(nx_module_t *module)
{
    nx_im_msvistalog_conf_t *imconf;

    ASSERT(module != NULL);
    ASSERT(module->config != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;

    if ( imconf->event != NULL )
    {
	nx_event_remove(imconf->event);
	nx_event_free(imconf->event);
	imconf->event = NULL;
    }
}



static void im_msvistalog_resume(nx_module_t *module)
{
    nx_im_msvistalog_conf_t *imconf;
    nx_event_t *event;

    ASSERT(module != NULL);
    ASSERT(module->config != NULL);

    imconf = (nx_im_msvistalog_conf_t *) module->config;

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



static void im_msvistalog_event(nx_module_t *module, nx_event_t *event)
{
    ASSERT(event != NULL);

    switch ( event->type )
    {
	case NX_EVENT_READ:
	    im_msvistalog_read(module);
	    break;
	default:
	    nx_panic("invalid event type: %d", event->type);
    }
}



NX_MODULE_DECLARATION nx_im_msvistalog_module =
{
    NX_MODULE_API_VERSION,
    NX_MODULE_TYPE_INPUT,
    NULL,			// capabilities
    im_msvistalog_config,	// config
    im_msvistalog_start,	// start
    im_msvistalog_stop, 	// stop
    im_msvistalog_pause,	// pause
    im_msvistalog_resume,	// resume
    im_msvistalog_init,		// init
    NULL,			// shutdown
    im_msvistalog_event,	// event
    NULL,			// info
    NULL,			// exports
};
