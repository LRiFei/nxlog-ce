/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#ifndef __NX_IM_MSVISTALOG_H
#define __NX_IM_MSVISTALOG_H

#include "../../../common/types.h"
#include "../../../common/dlist.h"

#define IM_MSVISTALOG_DEFAULT_POLL_INTERVAL 1
#define IM_MSVISTALOG_BATCH_SIZE 50
#define IM_MSVISTALOG_RENDER_BUFFER_SIZE (1024 * 42) /* 42k */

/// severity levels
#define WINEVENT_AUDIT_TYPE            0
#define WINEVENT_CRITICAL_TYPE         1
#define WINEVENT_ERROR_TYPE            2
#define WINEVENT_WARNING_TYPE          3
#define WINEVENT_INFORMATION_TYPE      4
#define WINEVENT_VERBOSE_TYPE          5

#define WINEVENT_AUDIT_FAILURE 0x10000000000000LL
#define WINEVENT_AUDIT_SUCCESS 0x20000000000000LL

typedef struct nx_msvistalog_publisher_t
{
    NX_DLIST_ENTRY(nx_msvistalog_publisher_t) link;
    EVT_HANDLE handle;
} nx_msvistalog_publisher_t;

typedef struct nx_msvistalog_publishers_t nx_msvistalog_publishers_t;
NX_DLIST_HEAD(nx_msvistalog_publishers_t, nx_msvistalog_publisher_t);

typedef struct nx_im_msvistalog_conf_t
{
    boolean		savepos;
    nx_event_t		*event;
    const char		*query;
    const char		*channel;
    LPCWSTR		_query;
    LPCWSTR		_path;
    boolean		readfromlast;
    EVT_HANDLE		waithandle;
    EVT_HANDLE		eventarray[IM_MSVISTALOG_BATCH_SIZE];
    EVT_HANDLE		subscription;
    EVT_HANDLE		last_event;
    EVT_HANDLE		renderer_system;
    PEVT_VARIANT	renderbuf;
    DWORD		renderbufsize;
    EVT_HANDLE		renderer_user;
    PEVT_VARIANT	renderbuf_user;
    DWORD		renderbufsize_user;
    EVT_HANDLE		session;
    float		poll_interval;
    apr_pool_t		*publisherpool;
    apr_hash_t		*publisherhash;
    nx_msvistalog_publishers_t *publisherlist;
} nx_im_msvistalog_conf_t;

#endif	/* __NX_IM_MSVISTALOG_H */
