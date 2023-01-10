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

void init_dmap_entry(dmap_entry* dmap_entry_p, const dstring* key, const dstring* value);

void deinit_dmap_entry(dmap_entry* dmap_entry_p);



typedef hashmap dmap;

void init_dmap(dmap* dmap_p, int ignore_case_for_key);

dmap_entry* get_from_dmap(const dmap* dmap_p, const dstring* key);

// inserts a new dmap_entry in dmap with the given key and returns the inserted dmap_entry
// if an entry with the given key already exists then, no insertion will be performed and the existing entry will be returned
dmap_entry* insert_in_dmap(dmap* dmap_p, const dstring* key, const dstring* value);

int delete_from_dmap(dmap* dmap_p, const dmap_entry* dmap_entry_p);

void deinit_dmap(dmap* dmap_p);

#endif