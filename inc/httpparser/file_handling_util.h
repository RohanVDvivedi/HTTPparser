#ifndef FILE_HANDLING_UTIL_H
#define FILE_HANDLING_UTIL_H

#include<cutlery/dstring.h>
#include<httpparser/dmap.h>

// it returns POINT_DSTR (pointing to content of file_path) hence you do not need to deinit the return value
// in absence of '.' chanracter an empty dstring is returned
// "main/test.txt" => "txt"
dstring get_extension_from_file_path(const dstring* file_path);

// it returns a POINT_DSTR
// the return value of this function can be used to set the content-type header of the http response
dstring get_mimetype_from_file_extension(const dstring* ext);

// checks for a match between content_type header of the response and accept header of the request
// accept values may be
//	<MIME_type>/<MIME_subtype>
//	<MIME_type>/*
//	*/*
int match_accept_to_content_type(const dstring* content_type, const dstring* accept);

// path_regex can be any complete path dstring that does not contain a '*' directory
// or it can contain '*' in the directory but nor a '*' directory
// "/rohan/*/dvivedi" == "/rohan/vipulkumar/dvivedi" => 1
// "/rohan/*abcd/dvivedi" == "/rohan/vipulkumar/dvivedi" => 0
// "/rohan/*abcd/dvivedi" == "/rohan/*abcd/dvivedi" => 1
int match_path_with_path_regex(const dstring* path, const dstring* path_regex);

#endif