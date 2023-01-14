#include<http_headers.h>

#include<stream_util.h>

#include<dstring.h>

int parse_http_headers(stream* rs, dmap* headers)
{
	int error = 0;

	dstring CRLF = get_literal_cstring("\r\n");
	unsigned int CRLF_spml[3];
	get_prefix_suffix_match_lengths(&CRLF, CRLF_spml);

	dstring SPCL = get_literal_cstring(": ");

	while(1)
	{
		dstring header = read_dstring_until_from_stream(rs, &CRLF, CRLF_spml, 2048, &error);
		if(error || is_empty_dstring(&header))
			return -1;

		if(compare_dstring(&header, &CRLF) == 0)
			return 0;

		discard_chars_dstring(&header, get_char_count_dstring(&header) - 2, get_char_count_dstring(&header) - 1);

		dstring header_key;
		dstring header_value = split_dstring(&header, &SPCL, &header_key);

		insert_in_dmap(headers, &header_key, &header_value);

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