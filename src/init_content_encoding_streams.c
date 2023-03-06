#include<init_content_encoding_streams.h>

#include<http_constant_dstrings.h>

#include<stdlib.h>

#include<zlib_readable_decompression_stream.h>
#include<zlib_writable_compression_stream.h>

int initialize_readable_content_decoding_stream(stacked_stream* sstrm, dmap* headers)
{
	if(is_empty_stacked_stream(sstrm, READ_STREAMS))
		return -1;

	int return_success = 0;

	stack encodings_stack;
	initialize_stack(&encodings_stack, 16);

	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in encodings_stack
			dstring const * encoding = NULL;
			if(	(compare_dstring(&value, &gzip_ce) == 0 && (encoding = &gzip_ce)) || 
				(compare_dstring(&value, &deflate_ce) == 0 && (encoding = &deflate_ce))	)
			{
				if(is_full_stack(&encodings_stack) && !expand_stack(&encodings_stack))
				{
					return_success = -1;
					break;
				}
				push_to_stack(&encodings_stack, encoding);
			}
			else if(compare_dstring(&value, &identity_ce))
				continue;
			else
			{
				return_success = -1;
				break;
			}
		}
	}

	if(return_success == -1)
	{
		deinitialize_stack(&encodings_stack);
		return return_success;
	}

	// init streams based on stack encodings and then push them on stacked_stream
	while(!is_empty_stack(&encodings_stack))
	{
		stream* strm = malloc(sizeof(stream));
		dstring const * const encoding = get_top_of_stack(&encodings_stack);
		pop_from_stack(&encodings_stack);
		if(encoding == &gzip_ce)
			initialize_stream_for_zlib_decompression2(strm, get_top_of_stacked_stream(sstrm, READ_STREAMS), 31);
		else if(encoding == &deflate_ce)
			initialize_stream_for_zlib_decompression2(strm, get_top_of_stacked_stream(sstrm, READ_STREAMS), -15);
		push_to_stacked_stream(sstrm, strm, READ_STREAMS);
	}

	deinitialize_stack(&encodings_stack);
	return return_success;
}

int initialize_writable_content_encoding_stream(stacked_stream* sstrm, dmap* headers)
{
	if(is_empty_stacked_stream(sstrm, WRITE_STREAMS))
		return -1;

	int return_success = 0;

	stack encodings_stack;
	initialize_stack(&encodings_stack, 16);

	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in encodings_stack
			dstring const * encoding = NULL;
			if(	(compare_dstring(&value, &gzip_ce) == 0 && (encoding = &gzip_ce)) || 
				(compare_dstring(&value, &deflate_ce) == 0 && (encoding = &deflate_ce))	)
			{
				if(is_full_stack(&encodings_stack) && !expand_stack(&encodings_stack))
				{
					return_success = -1;
					break;
				}
				push_to_stack(&encodings_stack, encoding);
			}
			else if(compare_dstring(&value, &identity_ce))
				continue;
			else
			{
				return_success = -1;
				break;
			}
		}
	}

	if(return_success == -1)
	{
		deinitialize_stack(&encodings_stack);
		return return_success;
	}

	// init streams based on stack encodings and then push them on stacked_stream
	while(!is_empty_stack(&encodings_stack))
	{
		stream* strm = malloc(sizeof(stream));
		dstring const * const encoding = get_top_of_stack(&encodings_stack);
		pop_from_stack(&encodings_stack);
		if(encoding == &gzip_ce)
			initialize_stream_for_zlib_compression2(strm, get_top_of_stacked_stream(sstrm, WRITE_STREAMS), Z_DEFAULT_COMPRESSION, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
		else if(encoding == &deflate_ce)
			initialize_stream_for_zlib_compression2(strm, get_top_of_stacked_stream(sstrm, WRITE_STREAMS), Z_DEFAULT_COMPRESSION, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
		push_to_stacked_stream(sstrm, strm, WRITE_STREAMS);
	}

	deinitialize_stack(&encodings_stack);
	return return_success;
}