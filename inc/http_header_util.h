#ifndef HTTP_HEADER_UTIL_H
#define HTTP_HEADER_UTIL_H

#include<dstring.h>

typedef struct acceptable_value acceptable_value;
struct acceptable_value
{
	dstring value;
	double q_value;
};

// returns -1 for error and 0 fr success
int parse_acceptable_value(const dstring* singlular_header_value, acceptable_value* av_p);

#include<http_request.h>
#include<http_response.h>

const dstring* find_acceptable_content_encoding_for_http_response_body(const http_request_head* hrq_p);

// check if a given content type is acceptable
int check_content_type_acceptable(const dstring* content_type, const http_request_head* hrq_p);

// return < 0 for an error, else a value > 0 for the number of streams initialized in the stacked_stream
int intialize_http_body_and_encoding_streams_for_writing(stacked_stream* sstrm, stream* raw_strm, const dmap* headers);
int intialize_http_body_and_encoding_streams_for_writing(stacked_stream* sstrm, stream* raw_strm, const dmap* headers);

#endif