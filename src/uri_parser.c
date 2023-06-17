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