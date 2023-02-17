#ifndef FILE_HANDLING_UTIL_H
#define FILE_HANDLING_UTIL_H

#include<dstring.h>

// it returns POINT_DSTR (pointing to content of file_path) hence you do not need to deinit the return value
// in absence of '.' chanracter an empty dstring is returned
// "main/test.txt" => "txt"
const dstring get_extension_from_file_path(const dstring* file_path);

// it returns a POINT_DSTR
// the return value of this function can be used to set the content-type header of the http response
const dstring get_mimetype_from_file_extension(const dstring* ext);

#endif