#ifndef HTTP_HEADERS_H
#define HTTP_HEADERS_H

#include<dmap.h>

// returns -1 for error and 0 for success
int parse_http_headers(stream* rs, dmap* headers);

// returns -1 for error and 0 for success
int serialize_http_headers(stream* ws, const dmap* headers);

#endif