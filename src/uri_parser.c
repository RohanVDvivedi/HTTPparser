#include<uri_parser.h>

#include<http_constant_dstrings.h>
#include<cutlery_math.h>

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
	cy_uint scheme_end_pos = contains_dstring_RK(&uri_val_pdstr, &CL);
	if(scheme_end_pos == INVALID_INDEX)
		return -1;

	// add scheme to scheme
	concatenate_dstring(&(uri_p->scheme), &get_dstring_pointing_to(get_byte_array_dstring(uri_val), scheme_end_pos));

	// discard "scheme:"
	discard_chars_from_front_dstring(&uri_val_pdstr, scheme_end_pos + 1);

	if(is_prefix_of_dstring(&uri_val_pdstr, &F_SLSH_F_SLSH))
	{
		// discard "//"
		discard_chars_from_front_dstring(&uri_val_pdstr, scheme_end_pos);

		cy_uint authority_end_pos = contains_dstring_RK(&uri_val_pdstr, &F_SLSH);
		if(authority_end_pos == INVALID_INDEX)
			return -1;

		// this will contain "userinfo @ host : port"
		dstring authority = get_dstring_pointing_to(get_byte_array_dstring(&uri_val_pdstr), authority_end_pos);

		// extract userinfo, host and port in this scope
		{
			// position of delimeters
			cy_uint at_pos = contains_dstring_RK(&authority, &AT);
			cy_uint cl_pos = contains_dstring_RK(&authority, &CL);

			if(at_pos != INVALID_INDEX)
				concatenate_dstring(&(uri_p->userinfo), &get_dstring_pointing_to(get_byte_array_dstring(&authority), at_pos));

			cy_uint host_start = (at_pos == INVALID_INDEX) ? 0 : (at_pos + 1);
			cy_uint host_end = (cl_pos == INVALID_INDEX) ? get_char_count_dstring(&authority) : cl_pos;
			concatenate_dstring(&(uri_p->host), &get_dstring_pointing_to(get_byte_array_dstring(&authority) + host_start, host_end - host_start));

			if(cl_pos != INVALID_INDEX)
				concatenate_dstring(&(uri_p->port), &get_dstring_pointing_to(get_byte_array_dstring(&authority) + cl_pos + 1, get_char_count_dstring(&authority) - cl_pos - 1));
		}

		// discard all of authority, after this call uri_val_pdstr only contains "path ? query # fragment"
		discard_chars_from_front_dstring(&uri_val_pdstr, authority_end_pos);
	}

	// next is path, a mandatory field
	cy_uint qm_pos = contains_dstring_RK(&uri_val_pdstr, &QM);
	cy_uint hs_pos = contains_dstring_RK(&uri_val_pdstr, &PND);

	// calculate path_end_position
	cy_uint path_end_pos = min(min(qm_pos, hs_pos), get_char_count_dstring(&uri_val_pdstr));
	concatenate_dstring(&(uri_p->path), &get_dstring_pointing_to(get_byte_array_dstring(&uri_val_pdstr), path_end_pos));

	// ? exists implies a query exists
	if(qm_pos != INVALID_INDEX)
	{
		cy_uint query_end_pos = min(hs_pos, get_char_count_dstring(&uri_val_pdstr));
		concatenate_dstring(&(uri_p->query), &get_dstring_pointing_to(get_byte_array_dstring(&uri_val_pdstr) + qm_pos + 1, query_end_pos - qm_pos - 1));
	}

	// # exists implies a fragment exists
	if(hs_pos != INVALID_INDEX)
	{
		cy_uint fragment_end_pos = get_char_count_dstring(&uri_val_pdstr);
		concatenate_dstring(&(uri_p->fragment), &get_dstring_pointing_to(get_byte_array_dstring(&uri_val_pdstr) + hs_pos + 1, fragment_end_pos - hs_pos - 1));
	}

	return 0;
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