#ifndef HEADER_VALUE_H
#define HEADER_VALUE_H

#include<dstring.h>
#include<dmap.h>

typedef struct header_value header_value;
struct header_value
{
	// the first thing that appears in a header is its value
	dstring value;

	// the next comes attributes, separted by semicolon
	dmap attributes;
};

#endif