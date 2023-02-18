#include<http_constant_dstrings.h>

const dstring CRLF = get_dstring_pointing_to_literal_cstring("\r\n");

const dstring SP = get_dstring_pointing_to_literal_cstring(" ");

const dstring CL = get_dstring_pointing_to_literal_cstring(":");

const dstring CLSP = get_dstring_pointing_to_literal_cstring(": ");

const dstring SCL = get_dstring_pointing_to_literal_cstring(";");

const dstring CM = get_dstring_pointing_to_literal_cstring(",");

const dstring EQ = get_dstring_pointing_to_literal_cstring("=");

const dstring F_SLSH = get_dstring_pointing_to_literal_cstring("/");

const dstring AST = get_dstring_pointing_to_literal_cstring("*");

const dstring AMP = get_dstring_pointing_to_literal_cstring("&");

const dstring QM = get_dstring_pointing_to_literal_cstring("?");

const dstring http_version_prefix = get_dstring_pointing_to_literal_cstring("HTTP/");

const dstring content_length = get_dstring_pointing_to_literal_cstring("content-length");

const dstring transfer_encoding = get_dstring_pointing_to_literal_cstring("transfer-encoding");

const dstring chunked = get_dstring_pointing_to_literal_cstring("chunked");

const dstring LAST_CHUNK = get_dstring_pointing_to_literal_cstring("0\r\n\r\n");

const unsigned int ZERO_spml[64];

unsigned int const * const CRLF_spml = ZERO_spml;

unsigned int const * const SP_spml = ZERO_spml;

unsigned int const * const chunked_spml = ZERO_spml;