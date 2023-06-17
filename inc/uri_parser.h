#ifndef URI_PARSER_H
#define URI_PARSER_H

#include<dstring.h>

// scheme ":" [ "//" [ userinfo "@" ] host [ ":" port ] ] path [ "?" query ] [ "#" fragment ]

typedef struct uri uri;
struct uri
{
	dstring scheme;
	dstring userinfo;
	dstring host;
	dstring port;
	dstring path;
	dstring query;
	dstring fragment;
};

void init_uri(uri* uri_p);

int parse_uri(uri* uri_p, const dstring* uri_val);

void deinit_uri(uri* uri_p);

#endif