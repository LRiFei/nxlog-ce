/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#include "../../../common/module.h"
#include "../../../common/error_debug.h"
#include "gelf.h"

#include "zlib.h"

#define NX_LOGMODULE NX_LOGMODULE_MODULE


static void xm_gelf_writer(nx_module_output_t *output,
			   void *data UNUSED)
{
    nx_gelf_ctx_t *gelfctx;
    nx_string_t *gelfstr;
    z_stream strm;

    ASSERT(output != NULL);

    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;

    ASSERT(deflateInit(&strm, Z_DEFAULT_COMPRESSION) == Z_OK);

    gelfctx = (nx_gelf_ctx_t *) nx_module_output_data_get(output, "gelfctx");
    if ( gelfctx == NULL )
    {
	gelfctx = apr_pcalloc(output->pool, sizeof(nx_gelf_ctx_t));
	nx_module_output_data_set(output, "gelfctx", gelfctx);
    }

    gelfctx->logdata = output->logdata;

    gelfstr = nx_logdata_to_gelf(gelfctx);
    //log_info(gelfstr->buf);

    strm.next_in = (unsigned char *) gelfstr->buf;
    strm.avail_in = gelfstr->len;
    strm.next_out = (unsigned char *) output->buf;
    strm.avail_out = (uInt) output->bufsize;
    ASSERT(deflate(&strm, Z_FINISH) == Z_STREAM_END);
    output->buflen = output->bufsize - strm.avail_out;
    output->bufstart = 0;
    ASSERT(strm.avail_in == 0);
    deflateEnd(&strm);
    nx_string_free(gelfstr);
}



static void xm_gelf_config(nx_module_t *module)
{
    const nx_directive_t *curr;

    nx_module_output_func_register(NULL, "gelf", &xm_gelf_writer, NULL);

    curr = module->directives;

    while ( curr != NULL )
    {
	if ( nx_module_common_keyword(curr->directive) == TRUE )
	{
	}
	else
	{
	    nx_conf_error(curr, "invalid keyword: %s", curr->directive);
	}

	curr = curr->next;
    }
}


NX_MODULE_DECLARATION nx_xm_gelf_module =
{
    NX_MODULE_API_VERSION,
    NX_MODULE_TYPE_EXTENSION,
    NULL,			// capabilities
    xm_gelf_config,		// config
    NULL,			// start
    NULL,	 		// stop
    NULL,			// pause
    NULL,			// resume
    NULL,			// init
    NULL,			// shutdown
    NULL,			// event
    NULL,			// info
    NULL,			//exports
};
