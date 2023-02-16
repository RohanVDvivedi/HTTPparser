#include<file_handling_util.h>

#include<stdio.h>

int main()
{
	dstring file_path = get_dstring_pointing_to_literal_cstring("/main/test.xhtml");
	dstring file_ext = get_extension_from_file_path(&file_path);
	printf(printf_dstring_format "\n", printf_dstring_params(&file_ext));

	file_path = get_dstring_pointing_to_literal_cstring("/main/test_xhtml");
	file_ext = get_extension_from_file_path(&file_path);
	printf(printf_dstring_format "\n", printf_dstring_params(&file_ext));
}