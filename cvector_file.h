/*

CVector 4.2.1 MIT Licensed vector (dynamic array) library in strict C89
http://www.robertwinkler.com/projects/cvector.html
http://www.robertwinkler.com/projects/cvector/

Besides the docs and all the Doxygen comments, see cvector_tests.c for
examples of how to use it or look at any of these other projects for
more practical examples:

https://github.com/rswinkle/C_Interpreter
https://github.com/rswinkle/CPIM2
https://github.com/rswinkle/spelling_game
https://github.com/rswinkle/c_bigint
http://portablegl.com/

The MIT License (MIT)

Copyright (c) 2011-2025 Robert Winkler

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and
to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
*/

#ifndef CVECTOR_file_H
#define CVECTOR_file_H

#ifndef CVEC_SIZE_T
#include <stdlib.h>
#define CVEC_SIZE_T size_t
#endif

#ifndef CVEC_SZ
#define CVEC_SZ
typedef CVEC_SIZE_T cvec_sz;
#endif

#ifdef __cplusplus
extern "C" {
#endif


typedef struct cvector_file
{
	file* a;
	cvec_sz size;
	cvec_sz capacity;
	void (*elem_free)(void*);
	int (*elem_init)(void*, void*);
} cvector_file;

extern cvec_sz CVEC_file_SZ;

int cvec_file(cvector_file* vec, cvec_sz size, cvec_sz capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*));
int cvec_init_file(cvector_file* vec, file* vals, cvec_sz num, void(*elem_free)(void*), int(*elem_init)(void*, void*));

cvector_file* cvec_file_heap(cvec_sz size, cvec_sz capacity, void (*elem_free)(void*), int(*elem_init)(void*, void*));
cvector_file* cvec_init_file_heap(file* vals, cvec_sz num, void (*elem_free)(void*), int(*elem_init)(void*, void*));

int cvec_copyc_file(void* dest, void* src);
int cvec_copy_file(cvector_file* dest, cvector_file* src);

int cvec_push_file(cvector_file* vec, file* val);
void cvec_pop_file(cvector_file* vec, file* ret);

int cvec_pushm_file(cvector_file* vec, file* a);
void cvec_popm_file(cvector_file* vec, file* ret);
int cvec_insertm_file(cvector_file* vec, cvec_sz i, file* a);
int cvec_insert_arraym_file(cvector_file* vec, cvec_sz i, file* a, cvec_sz num);
void cvec_replacem_file(cvector_file* vec, cvec_sz i, file* a, file* ret);

int cvec_extend_file(cvector_file* vec, cvec_sz num);
int cvec_insert_file(cvector_file* vec, cvec_sz i, file* a);
int cvec_insert_array_file(cvector_file* vec, cvec_sz i, file* a, cvec_sz num);
int cvec_replace_file(cvector_file* vec, cvec_sz i, file* a, file* ret);
void cvec_erase_file(cvector_file* vec, cvec_sz start, cvec_sz end);
void cvec_remove_file(cvector_file* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_file(cvector_file* vec, cvec_sz size);
#define cvec_shrink_to_fit_file(vec) cvec_set_cap_file((vec), (vec)->size)
int cvec_set_cap_file(cvector_file* vec, cvec_sz size);
int cvec_set_val_sz_file(cvector_file* vec, file* val);
int cvec_set_val_cap_file(cvector_file* vec, file* val);

file* cvec_back_file(cvector_file* vec);

void cvec_clear_file(cvector_file* vec);
void cvec_free_file_heap(void* vec);
void cvec_free_file(void* vec);

#ifdef __cplusplus
}
#endif

/* CVECTOR_file_H */
#endif


#ifdef CVECTOR_file_IMPLEMENTATION

cvec_sz CVEC_file_SZ = 20;

#define CVEC_file_ALLOCATOR(x) ((x+1) * 2)

#if defined(CVEC_MALLOC) && defined(CVEC_FREE) && defined(CVEC_REALLOC)
/* ok */
#elif !defined(CVEC_MALLOC) && !defined(CVEC_FREE) && !defined(CVEC_REALLOC)
/* ok */
#else
#error "Must define all or none of CVEC_MALLOC, CVEC_FREE, and CVEC_REALLOC."
#endif

#ifndef CVEC_MALLOC
#include <stdlib.h>
#define CVEC_MALLOC(sz)      malloc(sz)
#define CVEC_REALLOC(p, sz)  realloc(p, sz)
#define CVEC_FREE(p)         free(p)
#endif

#ifndef CVEC_MEMMOVE
#include <string.h>
#define CVEC_MEMMOVE(dst, src, sz)  memmove(dst, src, sz)
#endif

#ifndef CVEC_ASSERT
#include <assert.h>
#define CVEC_ASSERT(x)       assert(x)
#endif


/*  general vector */

cvector_file* cvec_file_heap(cvec_sz size, cvec_sz capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_file* vec;
	if (!(vec = (cvector_file*)CVEC_MALLOC(sizeof(cvector_file)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_file_SZ;

	if (!(vec->a = (file*)CVEC_MALLOC(vec->capacity * sizeof(file)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

cvector_file* cvec_init_file_heap(file* vals, cvec_sz num, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvector_file* vec;
	cvec_sz i;
	
	if (!(vec = (cvector_file*)CVEC_MALLOC(sizeof(cvector_file)))) {
		CVEC_ASSERT(vec != NULL);
		return NULL;
	}

	vec->capacity = num + CVEC_file_SZ;
	vec->size = num;
	if (!(vec->a = (file*)CVEC_MALLOC(vec->capacity * sizeof(file)))) {
		CVEC_ASSERT(vec->a != NULL);
		CVEC_FREE(vec);
		return NULL;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i], &vals[i])) {
				CVEC_ASSERT(0);
				CVEC_FREE(vec->a);
				CVEC_FREE(vec);
				return NULL;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(file)*num);
	}
	
	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return vec;
}

int cvec_file(cvector_file* vec, cvec_sz size, cvec_sz capacity, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	vec->size = size;
	vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size)) ? capacity : vec->size + CVEC_file_SZ;

	if (!(vec->a = (file*)CVEC_MALLOC(vec->capacity * sizeof(file)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int cvec_init_file(cvector_file* vec, file* vals, cvec_sz num, void(*elem_free)(void*), int(*elem_init)(void*, void*))
{
	cvec_sz i;
	
	vec->capacity = num + CVEC_file_SZ;
	vec->size = num;
	if (!(vec->a = (file*)CVEC_MALLOC(vec->capacity * sizeof(file)))) {
		CVEC_ASSERT(vec->a != NULL);
		vec->size = vec->capacity = 0;
		return 0;
	}

	if (elem_init) {
		for (i=0; i<num; ++i) {
			if (!elem_init(&vec->a[i], &vals[i])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(vec->a, vals, sizeof(file)*num);
	}

	vec->elem_free = elem_free;
	vec->elem_init = elem_init;

	return 1;
}

int cvec_copyc_file(void* dest, void* src)
{
	cvector_file* vec1 = (cvector_file*)dest;
	cvector_file* vec2 = (cvector_file*)src;

	vec1->a = NULL;
	vec1->size = 0;
	vec1->capacity = 0;

	return cvec_copy_file(vec1, vec2);
}

int cvec_copy_file(cvector_file* dest, cvector_file* src)
{
	int i;
	file* tmp = NULL;
	if (!(tmp = (file*)CVEC_REALLOC(dest->a, src->capacity*sizeof(file)))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	dest->a = tmp;

	if (src->elem_init) {
		for (i=0; i<src->size; ++i) {
			if (!src->elem_init(&dest->a[i], &src->a[i])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		/* could use memcpy here since we know we just allocated dest->a */
		CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(file));
	}

	dest->size = src->size;
	dest->capacity = src->capacity;
	dest->elem_free = src->elem_free;
	dest->elem_init = src->elem_init;
	return 1;
}

int cvec_push_file(cvector_file* vec, file* a)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_file_ALLOCATOR(vec->capacity);
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[vec->size], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(file));
	}
	
	vec->size++;
	return 1;
}

int cvec_pushm_file(cvector_file* vec, file* a)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_file_ALLOCATOR(vec->capacity);
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(file));
	
	vec->size++;
	return 1;
}

void cvec_pop_file(cvector_file* vec, file* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(file));
	}

	if (vec->elem_free) {
		vec->elem_free(&vec->a[vec->size]);
	}
}

void cvec_popm_file(cvector_file* vec, file* ret)
{
	vec->size--;
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(file));
	}
}

/** Return pointer to last element */
file* cvec_back_file(cvector_file* vec)
{
	return &vec->a[vec->size-1];
}

int cvec_extend_file(cvector_file* vec, cvec_sz num)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_file_SZ;
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	vec->size += num;
	return 1;
}

int cvec_insert_file(cvector_file* vec, cvec_sz i, file* a)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_file_ALLOCATOR(vec->capacity);
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(file));

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, sizeof(file));
	}

	vec->size++;
	return 1;
}

int cvec_insertm_file(cvector_file* vec, cvec_sz i, file* a)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity == vec->size) {
		tmp_sz = CVEC_file_ALLOCATOR(vec->capacity);
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}
	CVEC_MEMMOVE(&vec->a[i+1], &vec->a[i], (vec->size-i)*sizeof(file));

	CVEC_MEMMOVE(&vec->a[i], a, sizeof(file));

	vec->size++;
	return 1;
}

int cvec_insert_array_file(cvector_file* vec, cvec_sz i, file* a, cvec_sz num)
{
	file* tmp;
	cvec_sz tmp_sz, j;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_file_SZ;
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(file));
	if (vec->elem_init) {
		for (j=0; j<num; ++j) {
			if (!vec->elem_init(&vec->a[j+i], &a[j])) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(file));
	}
	vec->size += num;
	return 1;
}

int cvec_insert_arraym_file(cvector_file* vec, cvec_sz i, file* a, cvec_sz num)
{
	file* tmp;
	cvec_sz tmp_sz;
	if (vec->capacity < vec->size + num) {
		tmp_sz = vec->capacity + num + CVEC_VOID_START_SZ;
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*tmp_sz))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = tmp_sz;
	}

	CVEC_MEMMOVE(&vec->a[i+num], &vec->a[i], (vec->size-i)*sizeof(file));

	CVEC_MEMMOVE(&vec->a[i], a, num*sizeof(file));
	vec->size += num;
	return 1;
}

int cvec_replace_file(cvector_file* vec, cvec_sz i, file* a, file* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i], sizeof(file));
	} else if (vec->elem_free) {
		vec->elem_free(&vec->a[i]);
	}

	if (vec->elem_init) {
		if (!vec->elem_init(&vec->a[i], a)) {
			CVEC_ASSERT(0);
			return 0;
		}
	} else {
		CVEC_MEMMOVE(&vec->a[i], a, sizeof(file));
	}
	return 1;
}

void cvec_replacem_file(cvector_file* vec, cvec_sz i, file* a, file* ret)
{
	if (ret) {
		CVEC_MEMMOVE(ret, &vec->a[i], sizeof(file));
	}

	CVEC_MEMMOVE(&vec->a[i], a, sizeof(file));
}

void cvec_erase_file(cvector_file* vec, cvec_sz start, cvec_sz end)
{
	cvec_sz i;
	cvec_sz d = end - start + 1;
	if (vec->elem_free) {
		for (i=start; i<=end; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(file));
	vec->size -= d;
}

void cvec_remove_file(cvector_file* vec, cvec_sz start, cvec_sz end)
{
	cvec_sz d = end - start + 1;
	CVEC_MEMMOVE(&vec->a[start], &vec->a[end+1], (vec->size-1-end)*sizeof(file));
	vec->size -= d;
}

int cvec_reserve_file(cvector_file* vec, cvec_sz size)
{
	file* tmp;
	if (vec->capacity < size) {
		if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*(size+CVEC_file_SZ)))) {
			CVEC_ASSERT(tmp != NULL);
			return 0;
		}
		vec->a = tmp;
		vec->capacity = size + CVEC_file_SZ;
	}
	return 1;
}

int cvec_set_cap_file(cvector_file* vec, cvec_sz size)
{
	cvec_sz i;
	file* tmp;
	if (size < vec->size) {
		if (vec->elem_free) {
			for (i=vec->size-1; i>=size; i--) {
				vec->elem_free(&vec->a[i]);
			}
		}
		vec->size = size;
	}

	vec->capacity = size;

	if (!(tmp = (file*)CVEC_REALLOC(vec->a, sizeof(file)*size))) {
		CVEC_ASSERT(tmp != NULL);
		return 0;
	}
	vec-> a = tmp;
	return 1;
}

int cvec_set_val_sz_file(cvector_file* vec, file* val)
{
	cvec_sz i;

	if (vec->elem_free) {
		for(i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i]);
		}
	}
	
	if (vec->elem_init) {
		for (i=0; i<vec->size; i++) {
			if (!vec->elem_init(&vec->a[i], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->size; i++) {
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(file));
		}
	}
	return 1;
}

int cvec_set_val_cap_file(cvector_file* vec, file* val)
{
	cvec_sz i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; i++) {
			vec->elem_free(&vec->a[i]);
		}
		vec->size = vec->capacity;
	}

	if (vec->elem_init) {
		for (i=0; i<vec->capacity; i++) {
			if (!vec->elem_init(&vec->a[i], val)) {
				CVEC_ASSERT(0);
				return 0;
			}
		}
	} else {
		for (i=0; i<vec->capacity; i++) {
			CVEC_MEMMOVE(&vec->a[i], val, sizeof(file));
		}
	}
	return 1;
}

void cvec_clear_file(cvector_file* vec)
{
	cvec_sz i;
	if (vec->elem_free) {
		for (i=0; i<vec->size; ++i) {
			vec->elem_free(&vec->a[i]);
		}
	}
	vec->size = 0;
}

void cvec_free_file_heap(void* vec)
{
	cvec_sz i;
	cvector_file* tmp = (cvector_file*)vec;
	if (!tmp) return;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}
	CVEC_FREE(tmp->a);
	CVEC_FREE(tmp);
}

void cvec_free_file(void* vec)
{
	cvec_sz i;
	cvector_file* tmp = (cvector_file*)vec;
	if (tmp->elem_free) {
		for (i=0; i<tmp->size; i++) {
			tmp->elem_free(&tmp->a[i]);
		}
	}

	CVEC_FREE(tmp->a);
	tmp->a = NULL;
	tmp->size = 0;
	tmp->capacity = 0;
}


#endif
