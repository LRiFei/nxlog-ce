/*
 * This file is part of the nxlog log collector tool.
 * See the file LICENSE in the source root for licensing terms.
 * Website: http://nxlog.org
 * Author: Botond Botyanszki <botond.botyanszki@nxlog.org>
 */

#include "module.h"
#include "serialize.h"
#include "exception.h"

#define NX_LOGMODULE NX_LOGMODULE_MODULE

nx_logdata_t *nx_module_input_func_linereader(nx_module_input_t *input,
					      void *data UNUSED)
{
    int i;
    nx_logdata_t *retval = NULL;
    int len;
    nx_logdata_t *incomplete_logdata;

    ASSERT(input != NULL);
    ASSERT(input->buflen >= 0);

    if ( input->buflen == 0 )
    {
	return ( NULL );
    }
    incomplete_logdata = input->ctx;
    if ( incomplete_logdata == NULL )
    {
	if ( (input->buf[input->bufstart + 0] == NX_LOGDATA_BINARY_HEADER[0]) &&
	     (input->buflen >= 4) &&
	     (input->buf[input->bufstart + 1] == NX_LOGDATA_BINARY_HEADER[1]) && 
	     (input->buf[input->bufstart + 2] == NX_LOGDATA_BINARY_HEADER[2]) &&
	     (input->buf[input->bufstart + 3] == NX_LOGDATA_BINARY_HEADER[3]) ) //possible NX_LOGDATA_BINARY_HEADER
	{
	    return ( nx_module_input_func_binaryreader(input, NULL) );
	}
	else
	{ // treat as normal textbased line
	    boolean foundcr = FALSE;

	    for ( i = 0; i < input->buflen; i++ )
	    {
		if ( (input->buf[input->bufstart + i] == APR_ASCII_CR) ||
		     (input->buf[input->bufstart + i] == APR_ASCII_LF) )
		{
		    foundcr = TRUE;
		    break;
		}
	    }
	    
	    len = i;
	    if ( foundcr == TRUE )
	    {
		retval = nx_logdata_new_logline(input->buf + input->bufstart, len);
		ASSERT(len <= input->buflen);
		input->buflen -= len;
		while ( (input->buflen > 0) && ((input->buf[input->bufstart + i] == APR_ASCII_CR) ||
						(input->buf[input->bufstart + i] == APR_ASCII_LF)) )
		{
		    i++;
		    (input->buflen)--;
		}

		if ( input->buf[input->bufstart + i] == '\0' )
		{ // this is a hack against utf-16le double byte linebreak
		    i++;
		}
	    }
	    else
	    { // not found, partial read
		incomplete_logdata = nx_logdata_new_logline(input->buf + input->bufstart, len);
		input->ctx = (void *) incomplete_logdata;
		ASSERT(len <= input->buflen);
		input->buflen -= len;
	    }
	    input->bufstart += i;
	}
    }
    else
    { // we have a partial read in incomplete_logdata
	boolean foundcr = FALSE;
	
	log_debug("appending to incomplete_logdata");
	for ( i = 0; i < input->buflen; i++ )
	{
	    if ( (input->buf[input->bufstart + i] == APR_ASCII_CR) ||
		 (input->buf[input->bufstart + i] == APR_ASCII_LF) )
	    {
		foundcr = TRUE;
		break;
	    }
	}
	
	len = i;
	if ( len > 0 )
	{
	    nx_logdata_append_logline(incomplete_logdata, input->buf + input->bufstart, len);
	    ASSERT(len <= input->buflen);
	    input->buflen -= len;
	}
	if ( foundcr == TRUE )
	{
	    retval = incomplete_logdata;
	    input->ctx = NULL;
	    while ( (input->buflen > 0) && ((input->buf[input->bufstart + i] == APR_ASCII_CR) ||
					    (input->buf[input->bufstart + i] == APR_ASCII_LF)) )
	    {
		i++;
		(input->buflen)--;
	    }
	    if ( input->buf[input->bufstart + i] == '\0' )
	    { // this is a hack against utf-16le double byte linebreak
		i++;
	    }
	}
	input->bufstart += i;
    }

    return ( retval );
}



nx_logdata_t *nx_module_input_func_dgramreader(nx_module_input_t *input,
					       void *data UNUSED)
{
    nx_logdata_t *retval = NULL;

    ASSERT(input != NULL);
    ASSERT(input->buflen >= 0);
    ASSERT(input->ctx == NULL);
    ASSERT(input->bufstart == 0);

    if ( input->buflen == 0 )
    {
	return ( NULL );
    }

    if ( input->buf[input->bufstart] < 10 )
    {
	retval = nx_module_input_func_binaryreader(input, NULL);
	input->buflen = 0;
	input->bufstart = 0;
	return ( retval );
    }
    if ( input->bufsize > input->buflen )
    {
	input->buf[input->buflen] = '\0';
	retval = nx_logdata_new_logline(input->buf, (int) strlen(input->buf));
    }
    else
    {
	retval = nx_logdata_new_logline(input->buf, input->buflen);
    }
    input->buflen = 0;
    input->bufstart = 0;

    return ( retval );
}



nx_logdata_t *nx_module_input_func_binaryreader(nx_module_input_t *input,
						void *data UNUSED)
{
    nx_logdata_t *retval = NULL;
    apr_size_t len;
    apr_uint32_t datalen;
    nx_exception_t e;

    ASSERT(input != NULL);
    ASSERT(input->buflen >= 0);
    ASSERT(input->ctx == NULL);

    if ( input->buflen < 8 )
    {
	if ( input->bufstart + input->buflen == input->bufsize )
	{
	    memmove(input->buf, input->buf + input->bufstart, (size_t) input->buflen);
	    input->bufstart = 0;
	}
	return ( NULL );
    }

    if ( memcmp(input->buf + input->bufstart, NX_LOGDATA_BINARY_HEADER, 4) != 0 )
    {
	log_error("binary header not found in data received from %s, is input really binary?",
		  nx_module_input_name_get(input));
	for ( ; input->buflen > 0; (input->buflen)--, (input->bufstart)++ )
	{
	    if ( (input->buflen >= 4) &&
		 (memcmp(input->buf + input->bufstart, NX_LOGDATA_BINARY_HEADER, 4) == 0) )
	    {
		break;
	    }
	}

	if ( input->buflen == 0 )
	{
	    return ( NULL );
	}
    }

    datalen = nx_int32_from_le(input->buf + input->bufstart + 4);
    //log_info("datalen: %d", datalen);

    if ( datalen + 8 > NX_OUTPUT_BUFSIZE )
    {
	log_error("logdata is larger (%d) than OUTPUT_BUFSIZE, discarding", datalen);
	input->bufstart = 0;
	input->buflen = 0;
	return ( NULL );
    }

    if ( (int) datalen + 8 <= input->buflen )
    {
	//log_info("logdata (size: %d) all in buffer, reading from %d", (int) datalen + 8, input->bufstart);
	input->bufstart += 8;
	input->buflen -= 8;

	try
	{
	    retval = nx_logdata_from_membuf(input->buf + input->bufstart,
					    (apr_size_t) input->buflen, &len);
	    input->bufstart += (int) len;
	    ASSERT(len <= (apr_size_t) input->buflen);
	    input->buflen -= (int) len;
	}
	catch(e)
	{
	    log_exception_msg(e, "failed to read binary logdata, discarding");
	    for ( ; input->buflen > 0; (input->buflen)--, (input->bufstart)++ )
	    {
		if ( (input->buflen >= 4) &&
		     (memcmp(input->buf + input->bufstart, NX_LOGDATA_BINARY_HEADER, 4) == 0) )
		{
		    break;
		}
	    }
	}
    }
    else
    {
	log_debug("logdata (%d) larger than buffer (%d)", datalen + 8, input->buflen);
	if ( input->bufstart > 0 )
	{   // possible partial buffer
	    memmove(input->buf, input->buf + input->bufstart, (size_t) input->buflen);
	    input->bufstart = 0;
	}
    }
    return ( retval );
}
