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
	int error = 0;

	// skip reading the http version prefix
	{
		unsigned int http_version_prefix_read = skip_dstring_from_stream(rs, &http_version_prefix, &error);
		if(http_version_prefix_read == 0 || error)
			return -1;
	}

	// parse version major
	{
		uint64_t v_major;
		unsigned int v_major_bytes = read_uint64_from_stream(rs, DECIMAL, &v_major, &error);
		if(v_major > 100)
			return -1;
		if(v_major_bytes == 0 || error)
			return -1;
		v->major = v_major;
	}

	char byte;
	unsigned int byte_read = read_from_stream(rs, &byte, 1, &error);
	if(byte_read == 0 || error != 0 || byte != '.')
		return -1;

	// parse version minor
	{
		uint64_t v_minor;
		unsigned int v_minor_bytes = read_uint64_from_stream(rs, DECIMAL, &v_minor, &error);
		if(v_minor > 1000)
			return -1;
		if(v_minor_bytes == 0 || error)
			return -1;
		v->minor = v_minor;
	}

	return 0;
}

int serialize_http_version(stream* ws, const http_version* v)
{
	if(!write_to_stream_formatted(ws, printf_dstring_format"%d.%d", printf_dstring_params(&http_version_prefix), v->major, v->minor))
		return -1;

	return 0;
}