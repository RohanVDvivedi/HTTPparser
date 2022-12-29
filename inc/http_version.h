#ifndef HTTP_VERSION_H
#define HTTP_VERSION_H

typedef struct http_version http_version;
struct http_version
{
	int major;
	int minor;
};

extern const http_version valid_http_versions[];

int are_identical_http_versions(const http_version* v1, const http_version* v2);

int is_valid_http_version(const http_version* vsn);

#endif