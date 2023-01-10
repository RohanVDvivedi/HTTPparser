#ifndef HTTP_REQUEST_LINE_H
#define HTTP_REQUEST_LINE_H

#include<stream.h>

#include<http_request.h>

// returns -1 for error and 0 for success
int parse_http_request_line(stream* rs, http_request* hr_p);

// returns -1 for error and 0 for success
int serialize_http_request_line(stream* ws, const http_request* hr_p);

#endif