#include<http_version.h>

#include<string.h>
#include<stdio.h>

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

const char* http_version_prefix = "HTTP/";

static int parse_unsigned_int(stream* rs, unsigned int* u)
{
	char byte;
	unsigned int byte_read = 0;
	int error = 0;

	*u = 0;
	unsigned int total_bytes_read = 0;

	while(1)
	{
		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(error != 0)
			return -1;

		if(byte_read > 0)
		{
			if(('0' <= byte) && (byte <= '9'))
			{
				if(total_bytes_read > 0 && (*u) == 0 && byte == '0')
					return -1;
				total_bytes_read += byte_read;
				(*u) = ((*u) * 10) + (byte - '0');
			}
			else
			{
				unread_from_stream(rs, &byte, 1);
				break;
			}
		}
		else
			break;
	}

	if(total_bytes_read == 0)
		return -1;

	return 0;
}

int parse_http_version(stream* rs, http_version* v)
{
	char byte;
	unsigned int byte_read = 0;
	int error = 0;

	unsigned int prefix_bytes_match = 0;
	while(1)
	{
		if(http_version_prefix[prefix_bytes_match] == '\0')
			break;

		byte_read = read_from_stream(rs, &byte, 1, &error);
		if(byte_read == 0 || error != 0)
			return -1;

		if(byte != http_version_prefix[prefix_bytes_match++])
			return -1;
	}

	error = parse_unsigned_int(rs, &(v->major));
	if(error)
		return -1;

	byte_read = read_from_stream(rs, &byte, 1, &error);
	if(byte_read == 0 || error != 0)
		return -1;
	if(byte != '.')
		return -1;

	error = parse_unsigned_int(rs, &(v->minor));
	if(error)
		return -1;

	return 0;
}

static int serialize_unsigned_int(stream* ws, const unsigned int* u)
{
	int error = 0;

	char unsigned_int_string[33] = {};
	unsigned int unsigned_int_string_size = snprintf(unsigned_int_string, 32, "%u", (*u));

	write_to_stream(ws, unsigned_int_string, unsigned_int_string_size, &error);
	if(error)
		return -1;

	return 0;
}

int serialize_http_version(stream* ws, const http_version* v)
{
	int error = 0;

	write_to_stream(ws, http_version_prefix, strlen(http_version_prefix), &error);
	if(error)
		return -1;

	error = serialize_unsigned_int(ws, &(v->major));
	if(error)
		return -1;

	static const char DOT = '.';
	write_to_stream(ws, &DOT, 1, &error);
	if(error)
		return -1;

	error = serialize_unsigned_int(ws, &(v->minor));
	if(error)
		return -1;

	return 0;
}