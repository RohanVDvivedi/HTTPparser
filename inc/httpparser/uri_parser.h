#ifndef URI_PARSER_H
#define URI_PARSER_H

#include<cutlery/dstring.h>

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

int init_uri(uri* uri_p);

#define URI_NO_ERROR          0
#define URI_PARSER_ERROR     -1
#define URI_ALLOCATION_ERROR -2

// returns int value, suggesting error, as mentioned above
int parse_uri(uri* uri_p, const dstring* uri_val);

void deinit_uri(uri* uri_p);

#endif