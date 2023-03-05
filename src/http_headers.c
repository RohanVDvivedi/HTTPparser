#include<http_headers.h>

#include<http_constant_dstrings.h>

#include<stream_util.h>

#include<dstring.h>

int parse_http_headers(stream* rs, dmap* headers)
{
	int error = 0;

	while(1)
	{
		// read from stream until CRLF
		dstring header = read_until_dstring_from_stream(rs, &CRLF, CRLF_spml, 2048, &error);
		if(error || is_empty_dstring(&header))
			return -1;

		// if we read just CRLF then it is end of headers and start of body
		if(compare_dstring(&header, &CRLF) == 0)
			return 0;

		// discard the last 2 bytes from header string, which must be CRLF
		discard_chars_from_back_dstring(&header, get_char_count_dstring(&CRLF));

		// split header into its corrsponding key and value
		dstring header_key;
		dstring header_value = split_dstring(&header, &CL, &header_key);

		// header key must not be empty
		if(is_empty_dstring(&header_key))
		{
			deinit_dstring(&header_key);
			deinit_dstring(&header_value);
			deinit_dstring(&header);
			return -1;
		}

		// trim header value of any spaces in the front and back
		trim_dstring(&header_value);

		// insert then both in the headers dmap
		insert_in_dmap(headers, &header_key, &header_value);

		// deinitialize all dstrings you generated

		deinit_dstring(&header_key);
		deinit_dstring(&header_value);

		deinit_dstring(&header);
	}

	return 0;
}

int serialize_http_headers(stream* ws, const dmap* headers)
{
	int error = 0;

	for_each_in_dmap(e, headers)
	{
		write_to_stream_formatted(ws, &error, printf_dstring_format printf_dstring_format printf_dstring_format printf_dstring_format, printf_dstring_params(&(e->key)), printf_dstring_params(&CLSP), printf_dstring_params(&(e->value)), printf_dstring_params(&CRLF));
		if(error)
			return -1;
	}

	write_dstring_to_stream(ws, &CRLF, &error);
	if(error)
		return -1;

	return 0;
}