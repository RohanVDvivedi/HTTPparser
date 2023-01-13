#include<http_request.h>

void init_http_request(http_request* hr_p)
{
	init_empty_dstring(&(hr_p->path), 0);
	init_dmap(&(hr_p->path_params), 0);
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_request(http_request* hr_p)
{
	deinit_dstring(&(hr_p->path));
	deinit_dmap(&(hr_p->path_params));
	deinit_dmap(&(hr_p->headers));
}