#include<http_response.h>

void init_http_response_head(http_response_head* hr_p)
{
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_response_head(http_response_head* hr_p)
{
	deinit_dmap(&(hr_p->headers));
}

#include<http_response_line.h>
#include<http_headers.h>

int parse_http_response_head(stream* rs, http_response_head* hr_p)
{
	int error = 0;

	error = parse_http_response_line(rs, hr_p);
	if(error)
		return -1;

	error = parse_http_headers(rs, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

int serialize_http_response_head(stream* ws, const http_response_head* hr_p)
{
	int error = 0;

	error = serialize_http_response_line(ws, hr_p);
	if(error)
		return -1;

	error = serialize_http_headers(ws, &(hr_p->headers));
	if(error)
		return -1;

	return 0;
}

#include<http_constant_dstrings.h>

const dstring* find_acceptable_content_encoding_for_response(const http_request_head* hrq_p)
{
	const dstring* result_encoding = NULL;

	int none_seen = 1;

	for_each_equals_in_dmap(accept_encoding_entry, &(hrq_p->headers), &accept_encoding)
	{
		for_each_split_by_delim(value, &(accept_encoding_entry->value), &CM)
		{
			none_seen = 0;
			if(compare_dstring(&value, &gzip_ce) == 0)
				result_encoding = &gzip_ce;
			else if(compare_dstring(&value, &deflate_ce) == 0)
				result_encoding = &deflate_ce;
			else if(0 == compare_dstring(&value, &identity_ce))
				result_encoding = &deflate_ce;
			else
				continue;

			if(result_encoding != NULL)
				break;
		}
	}

	if(none_seen)
		return &identity_ce;

	return result_encoding;
}

void init_http_response_head_from_http_request_head(http_request_head* hrq_p, const http_response_head* hrp_p, int status)
{

}

#include<stdio.h>

void print_http_response_head(const http_response_head* hr_p)
{
	printf("version = %d.%d\n", hr_p->version.major, hr_p->version.minor);
	printf("status = %d\n", hr_p->status);
	printf("headers = \n");
	for_each_in_dmap(e, &(hr_p->headers))
		printf("\t<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
}