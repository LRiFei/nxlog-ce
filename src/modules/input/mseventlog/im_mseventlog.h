/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#ifndef __NX_IM_MSEVENTLOG_H
#define __NX_IM_MSEVENTLOG_H

//#include <limits.h>

#include "../../../common/types.h"
#include "../../../common/dlist.h"
#include "../../../common/expr.h"

#define IM_MSEVENTLOG_REGISTRY_PATH "SYSTEM\\CurrentControlSet\\Services\\Eventlog"
#define IM_MSEVENTLOG_REG_BUFFER_LEN 1024
#define IM_MSEVENTLOG_BUFFER_LEN (511*1024)

typedef struct nx_eventlog_src_t
{
    NX_DLIST_ENTRY(nx_eventlog_src_t) link;
    HANDLE 		handle;  ///< NULL if couldn't open or need to reopen
    const char 		*name;
    DWORD 		rec_num; ///< for savepos
    boolean 		seek;
    apr_time_t		blacklist_until; ///< ignore this file until this time
    int			blacklist_interval; ///< seconds to blacklist the file, increased on failure  
    boolean 		ignorefirstread;
} nx_eventlog_src_t;

typedef struct nx_eventlog_sources_t nx_eventlog_sources_t;
NX_DLIST_HEAD(nx_eventlog_sources_t, nx_eventlog_src_t);


typedef struct nx_im_mseventlog_conf_t
{
    boolean savepos;
    boolean readfromlast;
    nx_event_t *event;
    uint32_t offs; ///< offset in buffer
    DWORD bytes; ///< 0 if buffer is empty 
    char buffer[IM_MSEVENTLOG_BUFFER_LEN];
    nx_eventlog_sources_t *event_sources;
    apr_hash_t *dll_names;
    apr_hash_t *dll_libs;
    nx_eventlog_src_t *currsrc;
    nx_expr_proc_t *convert_fields;
    boolean utf8;
} nx_im_mseventlog_conf_t;

#endif	/* __NX_IM_MSEVENTLOG_H */
