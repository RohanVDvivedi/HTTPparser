#include<uri_parser.h>

void init_uri(uri* uri_p)
{
	uri_p->scheme = get_dstring_pointing_to(NULL, 0);
	uri_p->userinfo = get_dstring_pointing_to(NULL, 0);
	uri_p->host = get_dstring_pointing_to(NULL, 0);
	uri_p->port = get_dstring_pointing_to(NULL, 0);
	uri_p->path = get_dstring_pointing_to(NULL, 0);
	uri_p->query = get_dstring_pointing_to(NULL, 0);
	uri_p->fragment = get_dstring_pointing_to(NULL, 0);
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