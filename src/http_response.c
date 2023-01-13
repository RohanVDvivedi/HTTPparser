#include<http_response.h>

void init_http_response(http_response* hr_p)
{
	init_dmap(&(hr_p->headers), 0);
}