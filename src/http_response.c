#include<http_response.h>

void init_http_response(http_response* hr_p)
{
	init_dmap(&(hr_p->headers), 1);
}

void deinit_http_response(http_response* hr_p)
{
	deinit_dmap(&(hr_p->headers));
}