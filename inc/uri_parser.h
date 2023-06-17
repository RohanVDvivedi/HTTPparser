#ifndef URI_PARSER_H
#define URI_PARSER_H

// scheme ":" [ "//" [ userinfo "@" ] host [ ":" port ] ] path [ "?" query ] [ "#" fragment ]

typedef struct uri uri;
struct uri
{
	// all the contents of uri will be POINT_DSTRs
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

#endif