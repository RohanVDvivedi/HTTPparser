#ifndef HTTP_PATH_AND_PATH_PARAMS_H
#define HTTP_PATH_AND_PATH_PARAMS_H

#include<http_request.h>

// returns -1 for error and 0 for success
int parse_http_path_and_path_params(stream* rs, http_request* hr_p);

// returns -1 for error and 0 for success
int serialize_http_path_and_path_params(stream* ws, const http_request* hr_p);

// key and value will be initialized by this function, you only need to deinit_dstring them
// and after this call they will contain the next key value pair, unless there is an error
int parse_url_encoded_param(stream* rs, dstring* key, dstring* value);

// this function never returns -1, hence no error
int parse_url_encoded_params(stream* rs, dmap* params);

// in the below functions set the is_first_param flag if this is the first param being written to the stream
int serialize_url_encoded_param(stream* ws, const dstring* key, const dstring* value, int is_first_param);

int serialize_url_encoded_params(stream* ws, const dmap* params);

#endif