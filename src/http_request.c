#include<http_request.h>

void init_http_request(http_request_head* hr_p)
{
	init_empty_dstring(&(hr_p->path), 0);
	init_dmap(&(hr_p->path_params), 0);
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_request(http_request_head* hr_p)
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