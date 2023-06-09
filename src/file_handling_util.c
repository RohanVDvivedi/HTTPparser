#include<file_handling_util.h>

#include<http_constant_dstrings.h>

dstring get_extension_from_file_path(const dstring* file_path)
{
	cy_uint ext_len = 0;

	const char* file_path_str = get_byte_array_dstring(file_path);
	cy_uint file_path_len = get_char_count_dstring(file_path);

	while(ext_len < file_path_len && file_path_str[file_path_len - 1 - ext_len] != '.')
		ext_len++;

	if(ext_len == file_path_len)
		return get_dstring_pointing_to(NULL, 0);

	return get_dstring_pointing_to(file_path_str + file_path_len - ext_len, ext_len);
}

// a map from extension of files to their corresponding http mime types
char const * const ext_to_mimetype[][2] = {
	{	"aac",		"audio/aac"},
	{	"abw",		"application/x-abiword"},
	{	"arc",		"application/x-freearc"},
	{	"avif",		"image/avif"},
	{	"avi",		"video/x-msvideo"},
	{	"azw",		"application/vnd.amazon.ebook"},
	{	"bin",		"application/octet-stream"},
	{	"bmp",		"image/bmp"},
	{	"bz",		"application/x-bzip"},
	{	"bz2",		"application/x-bzip2"},
	{	"cda",		"application/x-cdf"},
	{	"csh",		"application/x-csh"},
	{	"css",		"text/css"},
	{	"csv",		"text/csv"},
	{	"doc",		"application/msword"},
	{	"docx",		"application/vnd.openxmlformats-officedocument.wordprocessingml.document"},
	{	"eot",		"application/vnd.ms-fontobject"},
	{	"epub",		"application/epub+zip"},
	{	"gz",		"application/gzip"},
	{	"gif",		"image/gif"},
	{	"htm",		"text/html"},
	{	"html",		"text/html"},
	{	"ico",		"image/vnd.microsoft.icon"},
	{	"ics",		"text/calendar"},
	{	"jar",		"application/java-archive"},
	{	"jpeg",		"image/jpeg"},
	{	"jpg",		"image/jpeg"},
	{	"js",		"text/javascript"},
	{	"json",		"application/json"},
	{	"jsonld",	"application/ld+json"},
	{	"mid",		"audio/midi"},
	{	"midi",		"audio/midi"},
	{	"mjs",		"text/javascript"},
	{	"mp3",		"audio/mpeg"},
	{	"mp4",		"video/mp4"},
	{	"mpeg",		"video/mpeg"},
	{	"mpkg",		"application/vnd.apple.installer+xml"},
	{	"odp",		"application/vnd.oasis.opendocument.presentation"},
	{	"ods",		"application/vnd.oasis.opendocument.spreadsheet"},
	{	"odt",		"application/vnd.oasis.opendocument.text"},
	{	"oga",		"audio/ogg"},
	{	"ogv",		"video/ogg"},
	{	"ogx",		"application/ogg"},
	{	"opus",		"audio/opus"},
	{	"otf",		"font/otf"},
	{	"png",		"image/png"},
	{	"pdf",		"application/pdf"},
	{	"php",		"application/x-httpd-php"},
	{	"ppt",		"application/vnd.ms-powerpoint"},
	{	"pptx",		"application/vnd.openxmlformats-officedocument.presentationml.presentation"},
	{	"rar",		"application/vnd.rar"},
	{	"rtf",		"application/rtf"},
	{	"sh",		"application/x-sh"},
	{	"svg",		"image/svg+xml"},
	{	"tar",		"application/x-tar"},
	{	"tif",		"image/tiff"},
	{	"tiff",		"image/tiff"},
	{	"ts",		"video/mp2t"},
	{	"ttf",		"font/ttf"},
	{	"txt",		"text/plain"},
	{	"vsd",		"application/vnd.visio"},
	{	"wav",		"audio/wav"},
	{	"weba",		"audio/webm"},
	{	"webm",		"video/webm"},
	{	"webp",		"image/webp"},
	{	"woff",		"font/woff"},
	{	"woff2",	"font/woff2"},
	{	"xhtml",	"application/xhtml+xml"},
	{	"xls",		"application/vnd.ms-excel"},
	{	"xlsx",		"application/vnd.openxmlformats-officedocument.spreadsheetml.sheet"},
	{	"xml",		"application/xml"},
	{	"xul",		"application/vnd.mozilla.xul+xml"},
	{	"zip",		"application/zip"},
	{	"3gp",		"video/3gpp"},
	{	"3g2",		"video/3gpp2"},
	{	"7z",		"application/x-7z-compressed"},
};

dstring default_mimetype = get_dstring_pointing_to_literal_cstring("application/octet-stream");

dstring get_mimetype_from_file_extension(const dstring* ext)
{
	if(is_empty_dstring(ext))
		return default_mimetype;

	for(int i = 0; i < sizeof(ext_to_mimetype)/sizeof(ext_to_mimetype[0]); i++)
	{
		dstring extn = get_dstring_pointing_to_cstring(ext_to_mimetype[i][0]);
		if(compare_dstring(ext, &extn) == 0)
			return get_dstring_pointing_to_cstring(ext_to_mimetype[i][1]);
	}

	return default_mimetype;
}

int match_accept_to_content_type(const dstring* content_type, const dstring* accept)
{
	// everything matches
	if(compare_dstring(accept, &get_dstring_pointing_to_literal_cstring("*/*")) == 0)
		return 1;

	dstring accept_mime_type;
	dstring accept_mime_subtype = split_dstring(accept, &F_SLSH, &accept_mime_type);

	dstring content_mime_type;
	dstring content_mime_subtype = split_dstring(content_type, &F_SLSH, &content_mime_type);

	if(compare_dstring(&accept_mime_type, &content_mime_type) != 0)
		return 0;

	if(compare_dstring(&accept_mime_subtype, &AST) == 0 || compare_dstring(&accept_mime_subtype, &content_mime_subtype) == 0)
		return 1;

	return 0;
}

int match_path_with_path_regex(const dstring* path, const dstring* path_regex)
{
	dstring dir_path;
	dstring rem_path = split_dstring(path, &F_SLSH, &dir_path);
	for_each_split_by_delim(dir_regex, path_regex, &F_SLSH)
	{
		// end of path, but the path_regex still has directories
		if(get_byte_array_dstring(&dir_path) == NULL)
			return 0;

		// if dir_regex is not "*" and not equal to dir_path, then return failure to match
		if(compare_dstring(&dir_regex, &AST) != 0 && compare_dstring(&dir_regex, &dir_path) != 0)
			return 0;

		rem_path = split_dstring(&rem_path, &F_SLSH, &dir_path);
	}

	if(get_byte_array_dstring(&dir_path) != NULL)
		return 0;

	return 1;
}