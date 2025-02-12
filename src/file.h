#ifndef FILE_H
#define FILE_H

#include "fb_cvector.h"

#include <time.h>

#define SIZE_STR_BUF 16
#define MOD_STR_BUF 24

// TODO struct packing?  save a few bytes?
typedef struct file
{
	char* path;   // could be url;

	// time_t is a long int ...
	time_t modified;
	int size;     // in bytes (hard to believe it'd be bigger than ~2.1 GB)

	//  caching for list mode
	char mod_str[MOD_STR_BUF];
	char size_str[SIZE_STR_BUF];
	char* name;  // pointing at filename in path
} file;

CVEC_NEW_DECLS2(file)

void free_file(void* f);

// comparison functions
int filename_cmp_lt(const void* a, const void* b);
int filename_cmp_gt(const void* a, const void* b);

int filepath_cmp_lt(const void* a, const void* b);
int filepath_cmp_gt(const void* a, const void* b);

int filesize_cmp_lt(const void* a, const void* b);
int filesize_cmp_gt(const void* a, const void* b);

int filemodified_cmp_lt(const void* a, const void* b);
int filemodified_cmp_gt(const void* a, const void* b);

#endif
