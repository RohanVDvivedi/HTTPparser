#include<http_version.h>

#include<http_constant_dstrings.h>

#include<stream_util.h>

#include<string.h>
#include<stdio.h>
#include<stdint.h>

const http_version valid_http_versions[] = 
{
	{1,0},
	{1,1}
};

int are_identical_http_versions(const http_version* v1, const http_version* v2)
{
	return (v1->major == v2->major) && (v1->minor == v2->minor);
}

int is_valid_http_version(const http_version* vsn)
{
	for(int i = 0; i < sizeof(valid_http_versions) / sizeof(http_version); i++)
	{
		if(are_identical_http_versions(valid_http_versions + i, vsn))
			return 1;
	}
	return 0;
}

int parse_http_version(stream* rs, http_version* v)
{
	int stream_error = 0;

	// skip reading the http version prefix
	size_t http_version_prefix_read = skip_dstring_from_stream(rs, &http_version_prefix, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(http_version_prefix_read == 0)
		return HTTP_PARSER_ERROR;

	// parse version major
	{
		unsigned long long int v_major;
		size_t v_major_bytes = read_unsigned_long_long_int_from_stream(rs, DECIMAL, &v_major, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
		if(v_major_bytes == 0 || v_major > 100)
			return HTTP_PARSER_ERROR;
		v->major = v_major;
	}

	char byte;
	size_t byte_read = read_from_stream(rs, &byte, 1, &stream_error);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;
	if(byte_read == 0 || byte != '.')
		return HTTP_PARSER_ERROR;

	// parse version minor
	{
		unsigned long long int v_minor;
		size_t v_minor_bytes = read_unsigned_long_long_int_from_stream(rs, DECIMAL, &v_minor, &stream_error);
		if(stream_error)
			return HTTP_ERROR_IN_STREAM;
		if(v_minor_bytes == 0 || v_minor > 100)
			return HTTP_PARSER_ERROR;
		v->minor = v_minor;
	}

	return HTTP_NO_ERROR;
}

int serialize_http_version(stream* ws, const http_version* v)
{
	int stream_error = 0;
	write_to_stream_formatted(ws, &stream_error, printf_dstring_format"%d.%d", printf_dstring_params(&http_version_prefix), v->major, v->minor);
	if(stream_error)
		return HTTP_ERROR_IN_STREAM;

	return HTTP_NO_ERROR;
}