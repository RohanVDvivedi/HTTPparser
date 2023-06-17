#include<uri_parser.h>

void init_uri(uri* uri_p)
{
	init_empty_dstring(&(uri_p->scheme), 0);
	init_empty_dstring(&(uri_p->userinfo), 0);
	init_empty_dstring(&(uri_p->host), 0);
	init_empty_dstring(&(uri_p->port), 0);
	init_empty_dstring(&(uri_p->path), 0);
	init_empty_dstring(&(uri_p->query), 0);
	init_empty_dstring(&(uri_p->fragment), 0);
}

int parse_uri(uri* uri_p, const dstring* uri_val);

void deinit_uri(uri* uri_p)
{
	deinit_dstring(&(uri_p->scheme));
	deinit_dstring(&(uri_p->userinfo));
	deinit_dstring(&(uri_p->host));
	deinit_dstring(&(uri_p->port));
	deinit_dstring(&(uri_p->path));
	deinit_dstring(&(uri_p->query));
	deinit_dstring(&(uri_p->fragment));
}