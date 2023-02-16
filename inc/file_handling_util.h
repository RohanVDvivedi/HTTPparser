#ifndef FILE_HANDLING_UTIL_H
#define FILE_HANDLING_UTIL_H

#include<dstring.h>

// it returns POINT_DSTR (pointing to content of file_path) hence you do not need to deinit the return value
dstring get_extension_from_file_path(const dstring* file_path);

#endif