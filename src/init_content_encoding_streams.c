#include<httpparser/init_content_encoding_streams.h>

#include<httpparser/http_constant_dstrings.h>

#include<stdlib.h>

#include<connman/zlib_readable_decompression_stream.h>
#include<connman/zlib_writable_compression_stream.h>

int initialize_readable_content_decoding_stream(stacked_stream* sstrm, const dmap* headers)
{
	if(is_empty_stacked_stream(sstrm, READ_STREAMS))
		return -1;

	int return_success = 0;

	arraylist encodings_stack;
	if(!initialize_arraylist(&encodings_stack, 16))
		return -2;

	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding_HKEY)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in encodings_stack
			dstring const * encoding = NULL;
			if(	(compare_dstring(&value, &gzip_ce_HVAL) == 0 && (encoding = &gzip_ce_HVAL)) || 
				(compare_dstring(&value, &deflate_ce_HVAL) == 0 && (encoding = &deflate_ce_HVAL))	)
			{
				if(is_full_arraylist(&encodings_stack) && !expand_arraylist(&encodings_stack))
				{
					return_success = -2;
					break;
				}
				push_back_to_arraylist(&encodings_stack, encoding);
			}
			else if(0 == compare_dstring(&value, &identity_ce_HVAL))
				continue;
			else
			{
				return_success = -1;
				break;
			}
		}
	}

	if(return_success < 0)
	{
		deinitialize_arraylist(&encodings_stack);
		return return_success;
	}

	int streams_pushed = 0;

	// init streams based on stack encodings and then push them on stacked_stream
	while(!is_empty_arraylist(&encodings_stack))
	{
		stream* strm = malloc(sizeof(stream));
		if(strm == NULL)
		{
			return_success = -2;
			break;
		}
		dstring const * const encoding = get_back_of_arraylist(&encodings_stack);
		pop_back_from_arraylist(&encodings_stack);
		if(encoding == &gzip_ce_HVAL)
		{
			if(!initialize_stream_for_zlib_decompression2(strm, get_top_of_stacked_stream(sstrm, READ_STREAMS), 31))
			{
				free(strm);
				return_success = -3;
				break;
			}
		}
		else if(encoding == &deflate_ce_HVAL)
		{
			if(!initialize_stream_for_zlib_decompression2(strm, get_top_of_stacked_stream(sstrm, READ_STREAMS), -15))
			{
				free(strm);
				return_success = -3;
				break;
			}
		}
		if(!push_to_stacked_stream(sstrm, strm, READ_STREAMS))
		{
			return_success = -2;
			break;
		}
		streams_pushed++;
	}

	deinitialize_arraylist(&encodings_stack);

	if(return_success >= 0)
		return return_success;

	for(int i = 0; i < streams_pushed; i++)
	{
		stream* strm = get_top_of_stacked_stream(sstrm, READ_STREAMS);
		pop_from_stacked_stream(sstrm, READ_STREAMS);
		deinitialize_stream(strm);
		free(strm);
	}

	return return_success;
}

int initialize_writable_content_encoding_stream(stacked_stream* sstrm, const dmap* headers)
{
	if(is_empty_stacked_stream(sstrm, WRITE_STREAMS))
		return -1;

	int return_success = 0;

	arraylist encodings_stack;
	initialize_arraylist(&encodings_stack, 16);

	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding_HKEY)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in encodings_stack
			dstring const * encoding = NULL;
			if(	(compare_dstring(&value, &gzip_ce_HVAL) == 0 && (encoding = &gzip_ce_HVAL)) || 
				(compare_dstring(&value, &deflate_ce_HVAL) == 0 && (encoding = &deflate_ce_HVAL))	)
			{
				if(is_full_arraylist(&encodings_stack) && !expand_arraylist(&encodings_stack))
				{
					return_success = -2;
					break;
				}
				push_back_to_arraylist(&encodings_stack, encoding);
			}
			else if(0 == compare_dstring(&value, &identity_ce_HVAL))
				continue;
			else
			{
				return_success = -1;
				break;
			}
		}
	}

	if(return_success < 0)
	{
		deinitialize_arraylist(&encodings_stack);
		return return_success;
	}

	int streams_pushed = 0;

	// init streams based on stack encodings and then push them on stacked_stream
	while(!is_empty_arraylist(&encodings_stack))
	{
		stream* strm = malloc(sizeof(stream));
		if(strm == NULL)
		{
			return_success = -2;
			break;
		}
		dstring const * const encoding = get_back_of_arraylist(&encodings_stack);
		pop_back_from_arraylist(&encodings_stack);
		if(encoding == &gzip_ce_HVAL)
		{
			if(!initialize_stream_for_zlib_compression2(strm, get_top_of_stacked_stream(sstrm, WRITE_STREAMS), Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY))
			{
				free(strm);
				return_success = -3;
				break;
			}
		}
		else if(encoding == &deflate_ce_HVAL)
		{
			if(!initialize_stream_for_zlib_compression2(strm, get_top_of_stacked_stream(sstrm, WRITE_STREAMS), Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY))
			{
				free(strm);
				return_success = -3;
				break;
			}
		}
		if(!push_to_stacked_stream(sstrm, strm, WRITE_STREAMS))
		{
			return_success = -2;
			break;
		}
		streams_pushed++;
	}

	deinitialize_arraylist(&encodings_stack);

	if(return_success >= 0)
		return return_success;

	for(int i = 0; i < streams_pushed; i++)
	{
		stream* strm = get_top_of_stacked_stream(sstrm, WRITE_STREAMS);
		pop_from_stacked_stream(sstrm, WRITE_STREAMS);
		deinitialize_stream(strm);
		free(strm);
	}

	return return_success;
}