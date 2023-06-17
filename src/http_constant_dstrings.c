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

const dstring AT = get_dstring_pointing_to_literal_cstring("@");

const dstring PND = get_dstring_pointing_to_literal_cstring("#");

const dstring http_version_prefix = get_dstring_pointing_to_literal_cstring("HTTP/");

const dstring content_length_HKEY = get_dstring_pointing_to_literal_cstring("content-length");

const dstring transfer_encoding_HKEY = get_dstring_pointing_to_literal_cstring("transfer-encoding");

const dstring chunked_te_HVAL = get_dstring_pointing_to_literal_cstring("chunked");

const dstring content_encoding_HKEY = get_dstring_pointing_to_literal_cstring("content-encoding");

const dstring accept_encoding_HKEY = get_dstring_pointing_to_literal_cstring("accept-encoding");

const dstring identity_ce_HVAL = get_dstring_pointing_to_literal_cstring("identity");

const dstring gzip_ce_HVAL = get_dstring_pointing_to_literal_cstring("gzip");

const dstring deflate_ce_HVAL = get_dstring_pointing_to_literal_cstring("deflate");

const dstring br_ce_HVAL = get_dstring_pointing_to_literal_cstring("br");

const dstring compress_ce_HVAL = get_dstring_pointing_to_literal_cstring("compress");

const dstring accept_HKEY = get_dstring_pointing_to_literal_cstring("accept");

const dstring cookie_HKEY = get_dstring_pointing_to_literal_cstring("cookie");

const dstring content_type_HKEY = get_dstring_pointing_to_literal_cstring("content-type");

const dstring form_url_encoded_ct_HVAL = get_dstring_pointing_to_literal_cstring("application/x-www-form-urlencoded");

const dstring multipart_form_data_ct_HVAL = get_dstring_pointing_to_literal_cstring("multipart/form-data");

const dstring content_disposition_HKEY = get_dstring_pointing_to_literal_cstring("content-disposition");

const dstring form_data_cd_HVAL = get_dstring_pointing_to_literal_cstring("form-data");

const dstring LAST_CHUNK = get_dstring_pointing_to_literal_cstring("0\r\n\r\n");

const cy_uint ZERO_spml[64];

cy_uint const * const CRLF_spml = ZERO_spml;

cy_uint const * const SP_spml = ZERO_spml;

cy_uint const * const chunked_spml = ZERO_spml;