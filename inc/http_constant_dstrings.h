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
extern const dstring F_SLSH_F_SLSH;			// "//"
extern const dstring AST;					// "*"
extern const dstring AMP;					// "&"
extern const dstring QM;					// "?"
extern const dstring AT;					// "@"
extern const dstring PND;					// "#"
extern const dstring http_version_prefix;	// "HTTP/"

extern const dstring content_length_HKEY;	// "content-length"

extern const dstring transfer_encoding_HKEY;// "transfer-encoding"
extern const dstring chunked_te_HVAL;		// "chunked"

extern const dstring content_encoding_HKEY;	// "content-encoding"
extern const dstring accept_encoding_HKEY;	// "accept-encoding"
extern const dstring identity_ce_HVAL;		// "identity"
extern const dstring gzip_ce_HVAL;			// "gzip"
extern const dstring deflate_ce_HVAL;		// "deflate"
extern const dstring br_ce_HVAL;			// "brotli"
extern const dstring compress_ce_HVAL;		// "compress"

extern const dstring accept_HKEY;			// "accept"

extern const dstring cookie_HKEY;			// "cookie"

extern const dstring content_type_HKEY;		// "content-type"
extern const dstring form_url_encoded_ct_HVAL;// "application/x-www-form-urlencoded"
extern const dstring multipart_form_data_ct_HVAL;// "multipart/form-data"

extern const dstring content_disposition_HKEY;// "content-disposition"
extern const dstring form_data_cd_HVAL;		// "form-data"

extern const dstring LAST_CHUNK;			// "0\r\n\r\n"

// below are arrays of their prefix suffix match lengths
extern cy_uint const * const CRLF_spml;
extern cy_uint const * const SP_spml;
extern cy_uint const * const chunked_spml;

#endif