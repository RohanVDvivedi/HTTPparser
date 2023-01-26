#ifndef HTTP_CONSTANT_DSTRINGS_H
#define HTTP_CONSTANT_DSTRINGS_H

#include<dstring.h>

extern const dstring CRLF;					// "\r\n"
extern const dstring SP;					// " "
extern const dstring CL;					// ":"
extern const dstring CLSP;					// ": "
extern const dstring http_version_prefix;	// "HTTP/"

// below are arrays of their prefix suffix match lengths
extern unsigned int const * const CRLF_spml;
extern unsigned int const * const SP_spml;

#endif