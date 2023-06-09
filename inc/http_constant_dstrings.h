#ifndef HTTP_CONSTANT_DSTRINGS_H
#define HTTP_CONSTANT_DSTRINGS_H

#include<dstring.h>

extern const dstring CRLF;					// "\r\n"
extern const dstring SP;					// " "
extern const dstring CL;					// ":"
extern const dstring CLSP;					// ": "
extern const dstring SCL;					// ";"
extern const dstring CM;					// ","
extern const dstring EQ;					// "="
extern const dstring F_SLSH;				// "/"
extern const dstring AST;					// "*"
extern const dstring AMP;					// "&"
extern const dstring QM;					// "?"
extern const dstring http_version_prefix;	// "HTTP/"

extern const dstring content_length;		// "content-length"

extern const dstring transfer_encoding;		// "transfer-encoding"
extern const dstring chunked;				// "chunked"

extern const dstring content_encoding;		// "content-encoding"
extern const dstring accept_encoding;		// "accept-encoding"
extern const dstring identity_ce;			// "identity"
extern const dstring gzip_ce;				// "gzip"
extern const dstring deflate_ce;			// "deflate"
extern const dstring br_ce;					// "brotli"
extern const dstring compress_ce;			// "compress"

extern const dstring accept_http_header_key;// "accept"

extern const dstring LAST_CHUNK;			// "0\r\n\r\n"

// below are arrays of their prefix suffix match lengths
extern cy_uint const * const CRLF_spml;
extern cy_uint const * const SP_spml;
extern cy_uint const * const chunked_spml;

#endif