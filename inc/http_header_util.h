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

#endif