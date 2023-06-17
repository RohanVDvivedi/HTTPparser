#include<uri_parser.h>

void init_uri(uri* uri_p)
{
	init_empty_dstring(&(uri_p->scheme), 0);
	init_empty_dstring(&(uri_p->userinfo), 0);
	init_empty_dstring(&(uri_p->host), 0);
	init_empty_dstring(&(uri_p->port), 0);
	init_empty_dstring(&(uri_p->path), 0);
	init_empty_dstring(&(uri_p->query), 0);
	init_empty_dstring(&(uri_p->fragment), 0);
}

int parse_uri(uri* uri_p, const dstring* uri_val)
{
	dstring uri_val_pdstr = get_dstring_pointing_to_dstring(uri_val);

	// get the position of ":" in the uri_val_pdstr
	cy_uint scheme_end_pos = contains_dstring_RK(uri_val_pdstr, &CL);
	if(scheme_end_pos == INVALID_INDEX)
		return -1;

	// add scheme to scheme
	concatenate_dstring(&(uri_p->scheme), &get_dstring_pointing_to(get_byte_array_dstring(uri_val), scheme_end_pos));

	// discard scheme :
	discard_chars_from_front_dstring(&uri_val_pdstr, scheme_end_pos);

	if(is_prefix_of_dstring(&uri_val_pdstr, ))
}

void deinit_uri(uri* uri_p)
{
	deinit_dstring(&(uri_p->scheme));
	deinit_dstring(&(uri_p->userinfo));
	deinit_dstring(&(uri_p->host));
	deinit_dstring(&(uri_p->port));
	deinit_dstring(&(uri_p->path));
	deinit_dstring(&(uri_p->query));
	deinit_dstring(&(uri_p->fragment));
}