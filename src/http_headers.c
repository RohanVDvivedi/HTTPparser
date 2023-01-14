#include<http_headers.h>

#include<stream_util.h>

#include<dstring.h>

int parse_http_headers(stream* rs, dmap* headers)
{
	// TODO
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