#include<http_headers.h>

#include<stream_util.h>

#include<dstring.h>

#include<http_constant_dstrings.h>

int parse_http_headers(stream* rs, dmap* headers)
{
	int error = 0;

	unsigned int CRLF_spml[3];
	get_prefix_suffix_match_lengths(&CRLF, CRLF_spml);

	while(1)
	{
		// read from stream until CRLF
		dstring header = read_dstring_until_from_stream(rs, &CRLF, CRLF_spml, 2048, &error);
		if(error || is_empty_dstring(&header))
			return -1;

		// if we read just CRLF then it is end of headers and start of body
		if(compare_dstring(&header, &CRLF) == 0)
			return 0;

		// discard the last 2 bytes from header string, which must be CRLF
		discard_chars_dstring(&header, get_char_count_dstring(&header) - 2, get_char_count_dstring(&header) - 1);

		// split header into its corrsponding key and value
		dstring header_key;
		dstring header_value = split_dstring(&header, &SPCL, &header_key);

		// header key must not be empty
		if(is_empty_dstring(&header_key))
		{
			deinit_dstring(&header_key);
			deinit_dstring(&header_value);
			deinit_dstring(&header);
			return -1;
		}

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

	for(const dmap_entry* e = get_first_of_in_hashmap(headers, FIRST_OF_HASHMAP); e != NULL; e = get_next_of_in_hashmap(headers, e, ANY_IN_HASHMAP))
		write_to_stream_formatted(ws, printf_dstring_format ": " printf_dstring_format "\r\n", &error, printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));

	static const char* CRLF = "\r\n";
	write_to_stream(ws, CRLF, 2, &error);
	if(error)
		return -1;

	return 0;
}