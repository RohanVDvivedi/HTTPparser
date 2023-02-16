#include<file_handling_util.h>

dstring get_extension_from_file_path(const dstring* file_path)
{
	unsigned int ext_len = 0;

	const char* file_path_str = get_byte_array_dstring(file_path);
	unsigned int file_path_len = get_char_count_dstring(file_path);

	while(ext_len < file_path_len && file_path_str[file_path_len - 1 - ext_len] != '.')
		ext_len++;

	if(ext_len == file_path_len)
		return get_dstring_pointing_to(NULL, 0);

	return get_dstring_pointing_to(file_path_str + file_path_len - ext_len, ext_len);
}