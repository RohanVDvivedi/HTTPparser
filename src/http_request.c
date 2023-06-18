#include<http_request.h>

void init_http_request_head(http_request_head* hr_p)
{
	hr_p->version = (http_version){1,1};
	init_empty_dstring(&(hr_p->path), 0);
	init_dmap(&(hr_p->path_params), 0);
	init_dmap(&(hr_p->headers), 1);
}

#include<uri_parser.h>
#include<http_constant_dstrings.h>

int init_http_request_head_from_uri(http_request_head* hr_p, const dstring* uri_str)
{
	init_http_request_head(hr_p);

	uri uriv;
	init_uri(&uriv);
	int parse_error = parse_uri(&uriv, uri_str);

	if(parse_error)
		return -1;

	if(!is_empty_dstring(&(uriv.host)))
		insert_literal_cstrings_in_dmap(&(hr_p->headers), "host", &(uriv.host));

	if(!is_empty_dstring(&(uriv.path)))
		concatenate_dstring(&(hr_p->path), &(uriv.path));

	if(!is_empty_dstring(&(uriv.query)))
	{
		for_each_split_by_delim(key_value, &(uriv.query), &AMP)
		{
			dstring key;
			dstring value = split_dstring(&value, &EQ, &key);

			// malfomed uri
			if(get_byte_array_dstring(&value) == NULL)
				return -1;

			// empty key is error prone
			if(is_empty_dstring(&key))
				continue;

			insert_in_dmap(&(hr_p->path_params), &key, &value);
		}
	}

	deinit_uri(&uriv);
}

void deinit_http_request_head(http_request_head* hr_p)
{
	deinit_dstring(&(hr_p->path));
	deinit_dmap(&(hr_p->path_params));
	deinit_dmap(&(hr_p->headers));
}

#include<http_request_line.h>
#include<http_headers.h>

int parse_http_request_head(stream* rs, http_request_head* hr_p)
{
	int error = 0;

	error = parse_http_request_line(rs, hr_p);
	if(error)
		return -1;

	error = parse_http_headers(rs, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

int serialize_http_request_head(stream* ws, const http_request_head* hr_p)
{
	int error = 0;

	error = serialize_http_request_line(ws, hr_p);
	if(error)
		return -1;

	error = serialize_http_headers(ws, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

#include<stdio.h>

void print_http_request_head(const http_request_head* hr_p)
{
	printf("method = %s\n", http_method_strings[hr_p->method]);
	printf("path = " printf_dstring_format "\n",  printf_dstring_params(&(hr_p->path)));
	printf("path_params = \n");
	for_each_in_dmap(e, &(hr_p->path_params))
		printf("\t<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
	printf("version = %d.%d\n", hr_p->version.major, hr_p->version.minor);
	printf("headers = \n");
	for_each_in_dmap(e, &(hr_p->headers))
		printf("\t<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
}