#include<http_version.h>

const http_version valid_http_versions[] = 
{
	{1,0},
	{1,1},
	{2,0},
	{3,0}
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