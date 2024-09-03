#include<http_response.h>

int init_http_response_head(http_response_head* hr_p)
{
	hr_p->version = (http_version){1,1};
	if(!init_dmap(&(hr_p->headers), 1))
		return 0;
	return 1;
}

void deinit_http_response_head(http_response_head* hr_p)
{
	deinit_dmap(&(hr_p->headers));
}

#include<http_response_line.h>
#include<http_headers.h>

int parse_http_response_head(stream* rs, http_response_head* hr_p)
{
	int error = parse_http_response_line(rs, hr_p);
	if(error)
		return error;

	error = parse_http_headers(rs, &(hr_p->headers));
	if(error)
		return error;

	return HTTP_NO_ERROR;
}

int serialize_http_response_head(stream* ws, const http_response_head* hr_p)
{
	int error = serialize_http_response_line(ws, hr_p);
	if(error)
		return error;

	error = serialize_http_headers(ws, &(hr_p->headers));
	if(error)
		return error;

	return HTTP_NO_ERROR;
}

#include<http_header_util.h>
#include<http_constant_dstrings.h>

int init_http_response_head_from_http_request_head(http_response_head* hrp_p, const http_request_head* hrq_p, int status, unsigned long long int content_length_val)
{
	if(!init_http_response_head(hrp_p))
		return 0;
	hrp_p->version = hrq_p->version;
	hrp_p->status = status;
	if(content_length_val == TRANSFER_CHUNKED)
	{
		if(!insert_in_dmap(&(hrp_p->headers), &transfer_encoding_HKEY, &chunked_te_HVAL))
			goto FAILED;
		const dstring* content_encoding_val = find_acceptable_content_encoding_for_http_response_body(hrq_p);
		if(content_encoding_val && !insert_in_dmap(&(hrp_p->headers), &content_encoding_HKEY, content_encoding_val))
			goto FAILED;
	}
	else
	{
		if((!insert_formatted_in_dmap(&(hrp_p->headers), &content_length_HKEY, "%llu", content_length_val))
		 || (!insert_in_dmap(&(hrp_p->headers), &content_encoding_HKEY, &identity_ce_HVAL)))
			goto FAILED;
	}
	return 1;

	FAILED:;
	deinit_http_response_head(hrp_p);
	return 0;
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