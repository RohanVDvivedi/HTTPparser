#ifndef HTTP_VERSION_H
#define HTTP_VERSION_H

#include<stream.h>

typedef struct http_version http_version;
struct http_version
{
	unsigned int major;
	unsigned int minor;
};

extern const http_version valid_http_versions[];

int are_identical_http_versions(const http_version* v1, const http_version* v2);

int is_valid_http_version(const http_version* vsn);

// returns -1 for error, 0 for success
int parse_http_version(stream* rs, http_version* v);

// returns -1 for error, 0 for success
int serialize_http_version(stream* ws, const http_version* v);

#endif