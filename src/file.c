
#include "file.h"

#include "string_compare.c"

CVEC_NEW_DEFS2(file, RESIZE)

void free_file(void* f)
{
	free(((file*)f)->path);
}

// comparison functions
int filename_cmp_lt(const void* a, const void* b)
{
	return StringCompare(((file*)a)->name, ((file*)b)->name);
}

int filename_cmp_gt(const void* a, const void* b)
{
	return StringCompare(((file*)b)->name, ((file*)a)->name);
}

int filepath_cmp_lt(const void* a, const void* b)
{
	return StringCompare(((file*)a)->path, ((file*)b)->path);
}

int filepath_cmp_gt(const void* a, const void* b)
{
	return StringCompare(((file*)b)->path, ((file*)a)->path);
}

int filesize_cmp_lt(const void* a, const void* b)
{
	file* f1 = (file*)a;
	file* f2 = (file*)b;
	if (f1->size < f2->size)
		return -1;
	if (f1->size > f2->size)
		return 1;

	return 0;
}

int filesize_cmp_gt(const void* a, const void* b)
{
	file* f1 = (file*)a;
	file* f2 = (file*)b;
	if (f1->size > f2->size)
		return -1;
	if (f1->size < f2->size)
		return 1;

	return 0;
}

int filemodified_cmp_lt(const void* a, const void* b)
{
	file* f1 = (file*)a;
	file* f2 = (file*)b;
	if (f1->modified < f2->modified)
		return -1;
	if (f1->modified > f2->modified)
		return 1;

	return 0;
}

int filemodified_cmp_gt(const void* a, const void* b)
{
	file* f1 = (file*)a;
	file* f2 = (file*)b;
	if (f1->modified > f2->modified)
		return -1;
	if (f1->modified < f2->modified)
		return 1;

	return 0;
}

