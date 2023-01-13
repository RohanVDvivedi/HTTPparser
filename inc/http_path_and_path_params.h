#ifndef HTTP_PATH_AND_PATH_PARAMS_H
#define HTTP_PATH_AND_PATH_PARAMS_H

#include<http_request.h>

// returns -1 for error and 0 for success
int parse_http_path_and_path_params(stream* rs, http_request* hr_p);

// returns -1 for error and 0 for success
int serialize_http_path_and_path_params(stream* ws, const http_request* hr_p);

#endif