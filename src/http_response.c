#include<http_response.h>

void init_http_response(http_response* hr_p)
{
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_response(http_response* hr_p)
{
	deinit_dmap(&(hr_p->headers));
}

#include<http_response_line.h>
#include<http_headers.h>

int parse_http_response_head(stream* rs, http_response* hr_p)
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

int serialize_http_request_head(stream* ws, const http_response* hr_p)
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