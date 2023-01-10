#ifndef HTTP_RESPONSE_LINE_H
#define HTTP_RESPONSE_LINE_H

#include<stream.h>

#include<http_response.h>

// returns -1 for error and 0 for success
int parse_http_response_line(stream* rs, http_response* hr_p);

// returns -1 for error and 0 for success
int serialize_http_response_line(stream* ws, const http_response* hr_p);

#endif