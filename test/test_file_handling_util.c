#include<file_handling_util.h>

#include<stdio.h>

int main()
{
	dstring file_path = get_dstring_pointing_to_literal_cstring("/main/test..mpeg");
	dstring file_ext = get_extension_from_file_path(&file_path);
	printf(printf_dstring_format "\n", printf_dstring_params(&file_ext));
	dstring mimetype = get_mimetype_from_file_extension(&file_ext);
	printf(printf_dstring_format "\n", printf_dstring_params(&mimetype));

	printf("\n");

	file_path = get_dstring_pointing_to_literal_cstring("/main/test_xhtml");
	file_ext = get_extension_from_file_path(&file_path);
	printf(printf_dstring_format "\n", printf_dstring_params(&file_ext));
	mimetype = get_mimetype_from_file_extension(&file_ext);
	printf(printf_dstring_format "\n", printf_dstring_params(&mimetype));

	dstring accept       = get_dstring_pointing_to_literal_cstring("application/*");
	dstring content_type = get_dstring_pointing_to_literal_cstring("text/plain");
	printf(printf_dstring_format " == " printf_dstring_format " => %d\n", printf_dstring_params(&accept), printf_dstring_params(&content_type), match_accept_to_content_type(&content_type, &accept));
}