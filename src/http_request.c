#include<httpparser/http_request.h>

int init_http_request_head(http_request_head* hr_p)
{
	hr_p->version = (http_version){1,1};
	if(!init_empty_dstring(&(hr_p->path), 0))
		return 0;
	if(!init_dmap(&(hr_p->path_params), 0))
	{
		deinit_dstring(&(hr_p->path));
		return 0;
	}
	if(!init_dmap(&(hr_p->headers), 1))
	{
		deinit_dmap(&(hr_p->path_params));
		deinit_dstring(&(hr_p->path));
		return 0;
	}
	return 1;
}

#include<httpparser/uri_parser.h>
#include<httpparser/http_constant_dstrings.h>
#include<httpparser/http_path_and_path_params.h>

int init_http_request_head_from_uri(http_request_head* hr_p, const dstring* uri_str)
{
	if(!init_http_request_head(hr_p))
		return 0;
	uri uriv;
	if(!init_uri(&uriv))
	{
		deinit_http_request_head(hr_p);
		return 0;
	}

	int parse_error = parse_uri(&uriv, uri_str);
	if(parse_error)
		goto ERROR;

	if(!is_empty_dstring(&(uriv.host)) && !insert_in_dmap(&(hr_p->headers), &get_dstring_pointing_to_literal_cstring("host"), &(uriv.host)))
		goto ERROR;

	if(!is_empty_dstring(&(uriv.path)))
	{
		int error = uri_to_dstring_format(&(uriv.path), &(hr_p->path));
		if(error)
			goto ERROR;
	}
	else
	{
		if(!concatenate_dstring(&(hr_p->path), &F_SLSH))
			goto ERROR;
	}

	if(!is_empty_dstring(&(uriv.query)))
	{
		for_each_split_by_delim(key_value, &(uriv.query), &AMP)
		{
			dstring key;
			dstring value = split_dstring(&key_value, &EQ, &key);

			// malformed uri
			if(get_byte_array_dstring(&value) == NULL)
				goto ERROR;

			// empty key is error prone
			if(is_empty_dstring(&key))
				continue;

			dstring key_;
			if(!init_empty_dstring(&key_, get_char_count_dstring(&key)))
				goto ERROR;

			dstring value_;
			if(!init_empty_dstring(&value_, get_char_count_dstring(&value)))
			{
				deinit_dstring(&key_);
				goto ERROR;
			}

			// if we fail converting them to dstring_format OR if the insert fails, then we fail
			if(uri_to_dstring_format(&key, &key_) || uri_to_dstring_format(&value, &value_) || !insert_in_dmap(&(hr_p->path_params), &key_, &value_))
			{
				deinit_dstring(&key_);
				deinit_dstring(&value_);
				goto ERROR;
			}

			deinit_dstring(&key_);
			deinit_dstring(&value_);
		}
	}

	deinit_uri(&uriv);
	return 1;

	ERROR :;
	deinit_uri(&uriv);
	deinit_http_request_head(hr_p);
	return 0;
}

void deinit_http_request_head(http_request_head* hr_p)
{
	deinit_dstring(&(hr_p->path));
	deinit_dmap(&(hr_p->path_params));
	deinit_dmap(&(hr_p->headers));
}

#include<httpparser/http_request_line.h>
#include<httpparser/http_headers.h>

int parse_http_request_head(stream* rs, http_request_head* hr_p)
{
	int error = parse_http_request_line(rs, hr_p);
	if(error)
		return error;

	error = parse_http_headers(rs, &(hr_p->headers));
	if(error)
		return error;

	return HTTP_NO_ERROR;
}

int serialize_http_request_head(stream* ws, const http_request_head* hr_p)
{
	int error = serialize_http_request_line(ws, hr_p);
	if(error)
		return error;

	error = serialize_http_headers(ws, &(hr_p->headers));
	if(error)
		return error;

	return HTTP_NO_ERROR;
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