#ifndef URI_PARSER_H
#define URI_PARSER_H

#include<dstring.h>

// scheme ":" [ "//" [ userinfo "@" ] host [ ":" port ] ] path [ "?" query ] [ "#" fragment ]

typedef struct uri uri;
struct uri
{
	// all the contents of uri will be POINT_DSTRs
	// the the get_byte_array_dstring of any of them returns NULL, then they are absent in the uri
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