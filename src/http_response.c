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

#include<stdio.h>
#include<stdint.h>

#include<http_header_util.h>
#include<http_constant_dstrings.h>

void init_http_response_head_from_http_request_head(http_response_head* hrp_p, const http_request_head* hrq_p, int status, size_t content_length_val)
{
	init_http_response_head(hrp_p);
	hrp_p->version = hrq_p->version;
	hrp_p->status = status;
	if(content_length_val == TRANSFER_CHUNKED)
	{
		insert_in_dmap(&(hrp_p->headers), &transfer_encoding_HKEY, &chunked_te_HVAL);
		const dstring* content_encoding_val = find_acceptable_content_encoding_for_http_response_body(hrq_p);
		if(content_encoding_val)
			insert_in_dmap(&(hrp_p->headers), &content_encoding_HKEY, content_encoding_val);
	}
	else
	{
		insert_formatted_in_dmap(&(hrp_p->headers), &content_length_HKEY, "%zu", content_length_val);
		insert_in_dmap(&(hrp_p->headers), &content_encoding_HKEY, &identity_ce_HVAL);
	}
}

void print_http_response_head(const http_response_head* hr_p)
{
	printf("version = %d.%d\n", hr_p->version.major, hr_p->version.minor);
	printf("status = %d\n", hr_p->status);
	printf("headers = \n");
	for_each_in_dmap(e, &(hr_p->headers))
		printf("\t<" printf_dstring_format "> -> <" printf_dstring_format ">\n", printf_dstring_params(&(e->key)), printf_dstring_params(&(e->value)));
}