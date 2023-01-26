#include<http_constant_dstrings.h>

const dstring CRLF = get_dstring_pointing_to_literal_cstring("\r\n");

const dstring SP = get_dstring_pointing_to_literal_cstring(" ");

const dstring CL = get_dstring_pointing_to_literal_cstring(":");

const dstring CLSP = get_dstring_pointing_to_literal_cstring(": ");

const dstring http_version_prefix = get_dstring_pointing_to_literal_cstring("HTTP/");

unsigned int ZERO_spml[64];

extern const unsigned int* CRLF_spml = ZERO_spml;