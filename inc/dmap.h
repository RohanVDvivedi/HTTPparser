#ifndef DMAP_H
#define DMAP_H

#include<dstring.h>
#include<hashmap.h>

typedef struct dmap_entry dmap_entry;
struct dmap_entry
{
	dstring key;
	dstring value;

	// this is the node that will be used by the dmap
	rbhnode embed_node;
};

typedef hashmap dmap;

int init_dmap(dmap* dmap_p, int ignore_case_for_key);

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key);

// inserts a new dmap_entry in dmap with the given key and returns the inserted dmap_entry
// if an entry with the given key already exists then, no insertion will be performed and the existing entry will be returned
dmap_entry* insert_in_dmap(dmap* dmap_p, const dstring* key, const dstring* value);

// just like printf it inserts a formatted value corresponding to a key
dmap_entry* insert_formatted_in_dmap(dmap* dmap_p, const dstring* key, const char* value_format, ...);

#define insert_cstrings_in_dmap(dmap_p, key, value) insert_in_dmap(dmap_p, &get_dstring_pointing_to_cstring(key), &get_dstring_pointing_to_cstring(value))
#define insert_literal_cstrings_in_dmap(dmap_p, key, value) insert_in_dmap(dmap_p, &get_dstring_pointing_to_literal_cstring(key), &get_dstring_pointing_to_literal_cstring(value))

int delete_from_dmap(dmap* dmap_p, const dmap_entry* dmap_entry_p);

#include<stream.h>

void print_dmap(stream* ws, const dmap* dmap_p);

void deinit_dmap(dmap* dmap_p);

// below are 2 macros to iterate over all or some of the elements of the dmap

#define for_each_in_dmap(entry_p, dmap_p) for(const dmap_entry* entry_p = get_first_of_in_hashmap(dmap_p, FIRST_OF_HASHMAP); entry_p != NULL; entry_p = get_next_of_in_hashmap(dmap_p, entry_p, ANY_IN_HASHMAP))

#define for_each_equals_in_dmap(entry_p, dmap_p, key_p) for(const dmap_entry* entry_p = get_from_dmap(dmap_p, key_p); entry_p != NULL; entry_p = get_next_of_in_hashmap(dmap_p, entry_p, ANY_THAT_EQUALS))

#endif