#include<http_headers.h>

#include<http_constant_dstrings.h>

#include<stream_util.h>

#include<dstring.h>

int parse_http_headers(stream* rs, dmap* headers)
{
	int stream_error = 0;

	while(1)
	{
		// read from stream until CRLF
		dstring header = read_until_dstring_from_stream(rs, &CRLF, CRLF_spml, min(2048, MAX_UNREAD_BYTES_COUNT/2), &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
		if(is_empty_dstring(&header))
		{
			deinit_dstring(&header);
			return HTTP_PARSER_ERROR;
		}

		// if we read just CRLF then it is end of headers and start of body
		if(compare_dstring(&header, &CRLF) == 0)
		{
			deinit_dstring(&header);
			return HTTP_NO_ERROR;
		}

		// discard the last 2 bytes from header string, which must be CRLF
		discard_chars_from_back_dstring(&header, get_char_count_dstring(&CRLF));

		// split header into its corrsponding key and value
		dstring header_key;
		dstring header_value = split_dstring(&header, &CL, &header_key);
		// note: header_key and header_value are POINT_DSTR, hence we need not deinit them

		// header key must not be empty
		if(is_empty_dstring(&header_key))
		{
			deinit_dstring(&header);
			return HTTP_PARSER_ERROR;
		}

		// trim header value of any spaces in the front and back
		trim_dstring(&header_value);

		// insert then both in the headers dmap
		if(!insert_in_dmap(headers, &header_key, &header_value))
		{
			deinit_dstring(&header);
			return HTTP_ALLOCATION_ERROR;
		}

		// deinitialize all dstrings you generated, again header_key and header_value need not be deinit-ed
		deinit_dstring(&header);
	}

	return HTTP_NO_ERROR;
}

int serialize_http_headers(stream* ws, const dmap* headers)
{
	int stream_error = HTTP_NO_ERROR;

	for_each_in_dmap(e, headers)
	{
		// key
		write_dstring_to_stream(ws, &(e->key), &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;

		// ": "
		write_dstring_to_stream(ws, &CLSP, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;

		// value
		write_dstring_to_stream(ws, &(e->value), &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;

		// "\r\n"
		write_dstring_to_stream(ws, &CRLF, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
	}

	write_dstring_to_stream(ws, &CRLF, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	return HTTP_NO_ERROR;
}