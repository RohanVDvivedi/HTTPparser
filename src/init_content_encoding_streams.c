#include<init_content_encoding_streams.h>

#include<http_constant_dstrings.h>

int initialize_readable_content_decoding_stream(stream* strm, stream* underlying_stream, dmap* headers)
{
	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in array
		}
	}

	// init streams based on array and push them on stack
}

int initialize_writable_content_encoding_stream(stream* strm, stream* underlying_stream, dmap* headers)
{
	for_each_equals_in_dmap(content_encoding_entry, headers, &content_encoding)
	{
		for_each_split_by_delim(value, &(content_encoding_entry->value), &CM)
		{
			// compare it each with br, deflate, gzip, compress and identity and put them in array
		}
	}

	// init streams based on array and push them on stack
}