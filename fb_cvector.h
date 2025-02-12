
#include "myinttypes.h"

#define CVEC_ONLY_INT
#define CVEC_ONLY_STR
#define CVEC_SIZE_T i64
#define PRIcv_sz PRIiMAX

#ifndef CVECTOR_H
#define CVECTOR_H

#if defined(CVEC_ONLY_INT) || defined(CVEC_ONLY_DOUBLE) || defined(CVEC_ONLY_STR) || defined(CVEC_ONLY_VOID)
   #ifndef CVEC_ONLY_INT
   #define CVEC_NO_INT
   #endif
   #ifndef CVEC_ONLY_DOUBLE
   #define CVEC_NO_DOUBLE
   #endif
   #ifndef CVEC_ONLY_STR
   #define CVEC_NO_STR
   #endif
   #ifndef CVEC_ONLY_VOID
   #define CVEC_NO_VOID
   #endif
#endif

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

#ifndef CVEC_NO_INT



/** Data structure for int vector. */
typedef struct cvector_i
{
	int* a;            /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_i;

extern cvec_sz CVEC_I_START_SZ;

int cvec_i(cvector_i* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_i(cvector_i* vec, int* vals, cvec_sz num);

cvector_i* cvec_i_heap(cvec_sz size, cvec_sz capacity);
cvector_i* cvec_init_i_heap(int* vals, cvec_sz num);
int cvec_copyc_i(void* dest, void* src);
int cvec_copy_i(cvector_i* dest, cvector_i* src);

int cvec_push_i(cvector_i* vec, int a);
int cvec_pop_i(cvector_i* vec);

int cvec_extend_i(cvector_i* vec, cvec_sz num);
int cvec_insert_i(cvector_i* vec, cvec_sz i, int a);
int cvec_insert_array_i(cvector_i* vec, cvec_sz i, int* a, cvec_sz num);
int cvec_replace_i(cvector_i* vec, cvec_sz i, int a);
void cvec_erase_i(cvector_i* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_i(cvector_i* vec, cvec_sz size);
#define cvec_shrink_to_fit_i(vec) cvec_set_cap_i((vec), (vec)->size)
int cvec_set_cap_i(cvector_i* vec, cvec_sz size);
void cvec_set_val_sz_i(cvector_i* vec, int val);
void cvec_set_val_cap_i(cvector_i* vec, int val);

int* cvec_back_i(cvector_i* vec);

void cvec_clear_i(cvector_i* vec);
void cvec_free_i_heap(void* vec);
void cvec_free_i(void* vec);

#endif

#ifndef CVEC_NO_DOUBLE



/** Data structure for double vector. */
typedef struct cvector_d
{
	double* a;         /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_d;

extern cvec_sz CVEC_D_START_SZ;

int cvec_d(cvector_d* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_d(cvector_d* vec, double* vals, cvec_sz num);

cvector_d* cvec_d_heap(cvec_sz size, cvec_sz capacity);
cvector_d* cvec_init_d_heap(double* vals, cvec_sz num);
int cvec_copyc_d(void* dest, void* src);
int cvec_copy_d(cvector_d* dest, cvector_d* src);

int cvec_push_d(cvector_d* vec, double a);
double cvec_pop_d(cvector_d* vec);

int cvec_extend_d(cvector_d* vec, cvec_sz num);
int cvec_insert_d(cvector_d* vec, cvec_sz i, double a);
int cvec_insert_array_d(cvector_d* vec, cvec_sz i, double* a, cvec_sz num);
double cvec_replace_d(cvector_d* vec, cvec_sz i, double a);
void cvec_erase_d(cvector_d* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_d(cvector_d* vec, cvec_sz size);
#define cvec_shrink_to_fit_d(vec) cvec_set_cap_d((vec), (vec)->size)
int cvec_set_cap_d(cvector_d* vec, cvec_sz size);
void cvec_set_val_sz_d(cvector_d* vec, double val);
void cvec_set_val_cap_d(cvector_d* vec, double val);

double* cvec_back_d(cvector_d* vec);

void cvec_clear_d(cvector_d* vec);
void cvec_free_d_heap(void* vec);
void cvec_free_d(void* vec);

#endif

#ifndef CVEC_NO_STR



/** Data structure for string vector. */
typedef struct cvector_str
{
	char** a;          /**< Array. */
	cvec_sz size;       /**< Current size (amount you use when manipulating array directly). */
	cvec_sz capacity;   /**< Allocated size of array; always >= size. */
} cvector_str;

extern cvec_sz CVEC_STR_START_SZ;

#ifndef CVEC_STRDUP
#define CVEC_STRDUP cvec_strdup
char* cvec_strdup(const char* str);
#endif

int cvec_str(cvector_str* vec, cvec_sz size, cvec_sz capacity);
int cvec_init_str(cvector_str* vec, char** vals, cvec_sz num);

cvector_str* cvec_str_heap(cvec_sz size, cvec_sz capacity);
cvector_str* cvec_init_str_heap(char** vals, cvec_sz num);
int cvec_copyc_str(void* dest, void* src);
int cvec_copy_str(cvector_str* dest, cvector_str* src);

int cvec_push_str(cvector_str* vec, char* a);
void cvec_pop_str(cvector_str* vec, char* ret);

int cvec_pushm_str(cvector_str* vec, char* a);
#define cvec_popm_str(vec) (vec).a[--(vec).size]
int cvec_insertm_str(cvector_str* vec, cvec_sz i, char* a);
int cvec_insert_arraym_str(cvector_str* vec, cvec_sz i, char** a, cvec_sz num);
#define cvec_replacem_str(vec, i, s, ret) ((ret) = (vec).a[i], (vec).a[i] = (s))

int cvec_extend_str(cvector_str* vec, cvec_sz num);
int cvec_insert_str(cvector_str* vec, cvec_sz i, char* a);
int cvec_insert_array_str(cvector_str* vec, cvec_sz i, char** a, cvec_sz num);
void cvec_replace_str(cvector_str* vec, cvec_sz i, char* a, char* ret);
void cvec_erase_str(cvector_str* vec, cvec_sz start, cvec_sz end);
void cvec_remove_str(cvector_str* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_str(cvector_str* vec, cvec_sz size);
#define cvec_shrink_to_fit_str(vec) cvec_set_cap_str((vec), (vec)->size)
int cvec_set_cap_str(cvector_str* vec, cvec_sz size);
void cvec_set_val_sz_str(cvector_str* vec, char* val);
void cvec_set_val_cap_str(cvector_str* vec, char* val);

char** cvec_back_str(cvector_str* vec);

void cvec_clear_str(cvector_str* vec);
void cvec_free_str_heap(void* vec);
void cvec_free_str(void* vec);

#endif

#ifndef CVEC_NO_VOID



typedef unsigned char cvec_u8;

/** Data structure for generic type (cast to void) vectors */
typedef struct cvector_void
{
	cvec_u8* a;                 /**< Array. */
	cvec_sz size;             /**< Current size (amount you should use when manipulating array directly). */
	cvec_sz capacity;         /**< Allocated size of array; always >= size. */
	cvec_sz elem_size;        /**< Size in bytes of type stored (sizeof(T) where T is type). */
	void (*elem_free)(void*);
	int (*elem_init)(void*, void*);
} cvector_void;

extern cvec_sz CVEC_VOID_START_SZ;

#define CVEC_GET_VOID(VEC, TYPE, I) ((TYPE*)&(VEC)->a[(I)*(VEC)->elem_size])

int cvec_void(cvector_void* vec, cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));
int cvec_init_void(cvector_void* vec, void* vals, cvec_sz num, cvec_sz elem_sz, void(*elem_free)(void*), int(*elem_init)(void*, void*));

cvector_void* cvec_void_heap(cvec_sz size, cvec_sz capacity, cvec_sz elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));
cvector_void* cvec_init_void_heap(void* vals, cvec_sz num, cvec_sz elem_sz, void (*elem_free)(void*), int(*elem_init)(void*, void*));

int cvec_copyc_void(void* dest, void* src);
int cvec_copy_void(cvector_void* dest, cvector_void* src);

int cvec_push_void(cvector_void* vec, void* a);
void cvec_pop_void(cvector_void* vec, void* ret);
void* cvec_get_void(cvector_void* vec, cvec_sz i);

int cvec_pushm_void(cvector_void* vec, void* a);
void cvec_popm_void(cvector_void* vec, void* ret);
int cvec_insertm_void(cvector_void* vec, cvec_sz i, void* a);
int cvec_insert_arraym_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num);
void cvec_replacem_void(cvector_void* vec, cvec_sz i, void* a, void* ret);

int cvec_extend_void(cvector_void* vec, cvec_sz num);
int cvec_insert_void(cvector_void* vec, cvec_sz i, void* a);
int cvec_insert_array_void(cvector_void* vec, cvec_sz i, void* a, cvec_sz num);
int cvec_replace_void(cvector_void* vec, cvec_sz i, void* a, void* ret);
void cvec_erase_void(cvector_void* vec, cvec_sz start, cvec_sz end);
void cvec_remove_void(cvector_void* vec, cvec_sz start, cvec_sz end);
int cvec_reserve_void(cvector_void* vec, cvec_sz size);
#define cvec_shrink_to_fit_void(vec) cvec_set_cap_void((vec), (vec)->size)
int cvec_set_cap_void(cvector_void* vec, cvec_sz size);
int cvec_set_val_sz_void(cvector_void* vec, void* val);
int cvec_set_val_cap_void(cvector_void* vec, void* val);

void* cvec_back_void(cvector_void* vec);

void cvec_clear_void(cvector_void* vec);
void cvec_free_void_heap(void* vec);
void cvec_free_void(void* vec);

#endif


#ifdef __cplusplus
}
#endif


#define CVEC_NEW_DECLS(TYPE)                                                          \
  typedef struct cvector_##TYPE {                                                     \
    TYPE* a;                                                                          \
    cvec_sz size;                                                                     \
    cvec_sz capacity;                                                                 \
  } cvector_##TYPE;                                                                   \
                                                                                      \
  extern cvec_sz CVEC_##TYPE##_SZ;                                                    \
                                                                                      \
  int cvec_##TYPE(cvector_##TYPE* vec, cvec_sz size, cvec_sz capacity);               \
  int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, cvec_sz num);                 \
                                                                                      \
  cvector_##TYPE* cvec_##TYPE##_heap(cvec_sz size, cvec_sz capacity);                 \
  cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, cvec_sz num);                   \
                                                                                      \
  int cvec_copyc_##TYPE(void* dest, void* src);                                       \
  int cvec_copy_##TYPE(cvector_##TYPE* dest, cvector_##TYPE* src);                    \
                                                                                      \
  int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE a);                                  \
  TYPE cvec_pop_##TYPE(cvector_##TYPE* vec);                                          \
                                                                                      \
  int cvec_extend_##TYPE(cvector_##TYPE* vec, cvec_sz num);                           \
  int cvec_insert_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE a);                     \
  int cvec_insert_array_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num); \
  TYPE cvec_replace_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE a);                   \
  void cvec_erase_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end);            \
  int cvec_reserve_##TYPE(cvector_##TYPE* vec, cvec_sz size);                         \
  int cvec_set_cap_##TYPE(cvector_##TYPE* vec, cvec_sz size);                         \
  void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE val);                         \
  void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE val);                        \
                                                                                      \
  TYPE* cvec_back_##TYPE(cvector_##TYPE* vec);                                        \
                                                                                      \
  void cvec_clear_##TYPE(cvector_##TYPE* vec);                                        \
  void cvec_free_##TYPE##_heap(void* vec);                                            \
  void cvec_free_##TYPE(void* vec);

#define CVEC_NEW_DEFS(TYPE, RESIZE_MACRO)                                                   \
  cvec_sz CVEC_##TYPE##_SZ = 50;                                                            \
                                                                                            \
  cvector_##TYPE* cvec_##TYPE##_heap(cvec_sz size, cvec_sz capacity)                        \
  {                                                                                         \
    cvector_##TYPE* vec;                                                                    \
    if (!(vec = (cvector_##TYPE*)CVEC_MALLOC(sizeof(cvector_##TYPE)))) {                    \
      CVEC_ASSERT(vec != NULL);                                                             \
      return NULL;                                                                          \
    }                                                                                       \
                                                                                            \
    vec->size     = size;                                                                   \
    vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))          \
                        ? capacity                                                          \
                        : vec->size + CVEC_##TYPE##_SZ;                                     \
                                                                                            \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                     \
      CVEC_ASSERT(vec->a != NULL);                                                          \
      CVEC_FREE(vec);                                                                       \
      return NULL;                                                                          \
    }                                                                                       \
                                                                                            \
    return vec;                                                                             \
  }                                                                                         \
                                                                                            \
  cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, cvec_sz num)                          \
  {                                                                                         \
    cvector_##TYPE* vec;                                                                    \
                                                                                            \
    if (!(vec = (cvector_##TYPE*)CVEC_MALLOC(sizeof(cvector_##TYPE)))) {                    \
      CVEC_ASSERT(vec != NULL);                                                             \
      return NULL;                                                                          \
    }                                                                                       \
                                                                                            \
    vec->capacity = num + CVEC_##TYPE##_SZ;                                                 \
    vec->size     = num;                                                                    \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                     \
      CVEC_ASSERT(vec->a != NULL);                                                          \
      CVEC_FREE(vec);                                                                       \
      return NULL;                                                                          \
    }                                                                                       \
                                                                                            \
    CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE) * num);                                         \
                                                                                            \
    return vec;                                                                             \
  }                                                                                         \
                                                                                            \
  int cvec_##TYPE(cvector_##TYPE* vec, cvec_sz size, cvec_sz capacity)                      \
  {                                                                                         \
    vec->size     = size;                                                                   \
    vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))          \
                        ? capacity                                                          \
                        : vec->size + CVEC_##TYPE##_SZ;                                     \
                                                                                            \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                     \
      CVEC_ASSERT(vec->a != NULL);                                                          \
      vec->size = vec->capacity = 0;                                                        \
      return 0;                                                                             \
    }                                                                                       \
                                                                                            \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, cvec_sz num)                        \
  {                                                                                         \
    vec->capacity = num + CVEC_##TYPE##_SZ;                                                 \
    vec->size     = num;                                                                    \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                     \
      CVEC_ASSERT(vec->a != NULL);                                                          \
      vec->size = vec->capacity = 0;                                                        \
      return 0;                                                                             \
    }                                                                                       \
                                                                                            \
    CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE) * num);                                         \
                                                                                            \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_copyc_##TYPE(void* dest, void* src)                                              \
  {                                                                                         \
    cvector_##TYPE* vec1 = (cvector_##TYPE*)dest;                                           \
    cvector_##TYPE* vec2 = (cvector_##TYPE*)src;                                            \
                                                                                            \
    vec1->a = NULL;                                                                         \
    vec1->size = 0;                                                                         \
    vec1->capacity = 0;                                                                     \
                                                                                            \
    return cvec_copy_##TYPE(vec1, vec2);                                                    \
  }                                                                                         \
                                                                                            \
  int cvec_copy_##TYPE(cvector_##TYPE* dest, cvector_##TYPE* src)                           \
  {                                                                                         \
    TYPE* tmp = NULL;                                                                       \
    if (!(tmp = (TYPE*)CVEC_REALLOC(dest->a, src->capacity*sizeof(TYPE)))) {                \
      CVEC_ASSERT(tmp != NULL);                                                             \
      return 0;                                                                             \
    }                                                                                       \
    dest->a = tmp;                                                                          \
                                                                                            \
    CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(TYPE));                                  \
    dest->size = src->size;                                                                 \
    dest->capacity = src->capacity;                                                         \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE a)                                         \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    cvec_sz tmp_sz;                                                                         \
    if (vec->capacity > vec->size) {                                                        \
      vec->a[vec->size++] = a;                                                              \
    } else {                                                                                \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                 \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
        CVEC_ASSERT(tmp != NULL);                                                           \
        return 0;                                                                           \
      }                                                                                     \
      vec->a              = tmp;                                                            \
      vec->a[vec->size++] = a;                                                              \
      vec->capacity       = tmp_sz;                                                         \
    }                                                                                       \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  TYPE cvec_pop_##TYPE(cvector_##TYPE* vec) { return vec->a[--vec->size]; }                 \
                                                                                            \
  TYPE* cvec_back_##TYPE(cvector_##TYPE* vec) { return &vec->a[vec->size - 1]; }            \
                                                                                            \
  int cvec_extend_##TYPE(cvector_##TYPE* vec, cvec_sz num)                                  \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    cvec_sz tmp_sz;                                                                         \
    if (vec->capacity < vec->size + num) {                                                  \
      tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
        CVEC_ASSERT(tmp != NULL);                                                           \
        return 0;                                                                           \
      }                                                                                     \
      vec->a        = tmp;                                                                  \
      vec->capacity = tmp_sz;                                                               \
    }                                                                                       \
                                                                                            \
    vec->size += num;                                                                       \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_insert_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE a)                            \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    cvec_sz tmp_sz;                                                                         \
    if (vec->capacity > vec->size) {                                                        \
      CVEC_MEMMOVE(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));             \
      vec->a[i] = a;                                                                        \
    } else {                                                                                \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                 \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
        CVEC_ASSERT(tmp != NULL);                                                           \
        return 0;                                                                           \
      }                                                                                     \
      vec->a = tmp;                                                                         \
      CVEC_MEMMOVE(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));             \
      vec->a[i]     = a;                                                                    \
      vec->capacity = tmp_sz;                                                               \
    }                                                                                       \
                                                                                            \
    vec->size++;                                                                            \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_insert_array_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num)        \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    cvec_sz tmp_sz;                                                                         \
    if (vec->capacity < vec->size + num) {                                                  \
      tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                    \
        CVEC_ASSERT(tmp != NULL);                                                           \
        return 0;                                                                           \
      }                                                                                     \
      vec->a        = tmp;                                                                  \
      vec->capacity = tmp_sz;                                                               \
    }                                                                                       \
                                                                                            \
    CVEC_MEMMOVE(&vec->a[i + num], &vec->a[i], (vec->size - i) * sizeof(TYPE));             \
    CVEC_MEMMOVE(&vec->a[i], a, num * sizeof(TYPE));                                        \
    vec->size += num;                                                                       \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  TYPE cvec_replace_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE a)                          \
  {                                                                                         \
    TYPE tmp  = vec->a[i];                                                                  \
    vec->a[i] = a;                                                                          \
    return tmp;                                                                             \
  }                                                                                         \
                                                                                            \
  void cvec_erase_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end)                   \
  {                                                                                         \
    cvec_sz d = end - start + 1;                                                            \
    CVEC_MEMMOVE(&vec->a[start], &vec->a[end + 1], (vec->size - 1 - end) * sizeof(TYPE));   \
    vec->size -= d;                                                                         \
  }                                                                                         \
                                                                                            \
  int cvec_reserve_##TYPE(cvector_##TYPE* vec, cvec_sz size)                                \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    if (vec->capacity < size) {                                                             \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * (size + CVEC_##TYPE##_SZ)))) { \
        CVEC_ASSERT(tmp != NULL);                                                           \
        return 0;                                                                           \
      }                                                                                     \
      vec->a        = tmp;                                                                  \
      vec->capacity = size + CVEC_##TYPE##_SZ;                                              \
    }                                                                                       \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  int cvec_set_cap_##TYPE(cvector_##TYPE* vec, cvec_sz size)                                \
  {                                                                                         \
    TYPE* tmp;                                                                              \
    if (size < vec->size) {                                                                 \
      vec->size = size;                                                                     \
    }                                                                                       \
                                                                                            \
    if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * size))) {                        \
      CVEC_ASSERT(tmp != NULL);                                                             \
      return 0;                                                                             \
    }                                                                                       \
    vec->a        = tmp;                                                                    \
    vec->capacity = size;                                                                   \
    return 1;                                                                               \
  }                                                                                         \
                                                                                            \
  void cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE val)                                \
  {                                                                                         \
    cvec_sz i;                                                                              \
    for (i = 0; i < vec->size; i++) {                                                       \
      vec->a[i] = val;                                                                      \
    }                                                                                       \
  }                                                                                         \
                                                                                            \
  void cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE val)                               \
  {                                                                                         \
    cvec_sz i;                                                                              \
    for (i = 0; i < vec->capacity; i++) {                                                   \
      vec->a[i] = val;                                                                      \
    }                                                                                       \
  }                                                                                         \
                                                                                            \
  void cvec_clear_##TYPE(cvector_##TYPE* vec) { vec->size = 0; }                            \
                                                                                            \
  void cvec_free_##TYPE##_heap(void* vec)                                                   \
  {                                                                                         \
    cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                             \
    if (!tmp) return;                                                                       \
    CVEC_FREE(tmp->a);                                                                      \
    CVEC_FREE(tmp);                                                                         \
  }                                                                                         \
                                                                                            \
  void cvec_free_##TYPE(void* vec)                                                          \
  {                                                                                         \
    cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                             \
    CVEC_FREE(tmp->a);                                                                      \
    tmp->a        = NULL;                                                                   \
    tmp->size     = 0;                                                                      \
    tmp->capacity = 0;                                                                      \
  }

#define CVEC_NEW_DECLS2(TYPE)                                                                    \
  typedef struct cvector_##TYPE {                                                                \
    TYPE* a;                                                                                     \
    cvec_sz size;                                                                                \
    cvec_sz capacity;                                                                            \
    void (*elem_free)(void*);                                                                    \
    int (*elem_init)(void*, void*);                                                              \
  } cvector_##TYPE;                                                                              \
                                                                                                 \
  extern cvec_sz CVEC_##TYPE##_SZ;                                                               \
                                                                                                 \
  int cvec_##TYPE(cvector_##TYPE* vec, cvec_sz size, cvec_sz capacity, void (*elem_free)(void*), \
                  int (*elem_init)(void*, void*));                                               \
  int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, cvec_sz num, void (*elem_free)(void*),   \
                       int (*elem_init)(void*, void*));                                          \
                                                                                                 \
  cvector_##TYPE* cvec_##TYPE##_heap(cvec_sz size, cvec_sz capacity, void (*elem_free)(void*),   \
                                     int (*elem_init)(void*, void*));                            \
  cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, cvec_sz num, void (*elem_free)(void*),     \
                                          int (*elem_init)(void*, void*));                       \
                                                                                                 \
  int cvec_copyc_##TYPE(void* dest, void* src);                                                  \
  int cvec_copy_##TYPE(cvector_##TYPE* dest, cvector_##TYPE* src);                               \
                                                                                                 \
  int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE* val);                                          \
  void cvec_pop_##TYPE(cvector_##TYPE* vec, TYPE* ret);                                          \
                                                                                                 \
  int cvec_pushm_##TYPE(cvector_##TYPE* vec, TYPE* a);                                           \
  void cvec_popm_##TYPE(cvector_##TYPE* vec, TYPE* ret);                                         \
  int cvec_insertm_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a);                              \
  int cvec_insert_arraym_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num);           \
  void cvec_replacem_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, TYPE* ret);                 \
                                                                                                 \
  int cvec_extend_##TYPE(cvector_##TYPE* vec, cvec_sz num);                                      \
  int cvec_insert_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a);                               \
  int cvec_insert_array_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num);            \
  int cvec_replace_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, TYPE* ret);                   \
  void cvec_erase_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end);                       \
  void cvec_remove_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end);                      \
  int cvec_reserve_##TYPE(cvector_##TYPE* vec, cvec_sz size);                                    \
  int cvec_set_cap_##TYPE(cvector_##TYPE* vec, cvec_sz size);                                    \
  int cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE* val);                                    \
  int cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE* val);                                   \
                                                                                                 \
  TYPE* cvec_back_##TYPE(cvector_##TYPE* vec);                                                   \
                                                                                                 \
  void cvec_clear_##TYPE(cvector_##TYPE* vec);                                                   \
  void cvec_free_##TYPE##_heap(void* vec);                                                       \
  void cvec_free_##TYPE(void* vec);

#define CVEC_NEW_DEFS2(TYPE, RESIZE_MACRO)                                                       \
  cvec_sz CVEC_##TYPE##_SZ = 20;                                                                 \
                                                                                                 \
  cvector_##TYPE* cvec_##TYPE##_heap(cvec_sz size, cvec_sz capacity, void (*elem_free)(void*),   \
                                     int (*elem_init)(void*, void*))                             \
  {                                                                                              \
    cvector_##TYPE* vec;                                                                         \
    if (!(vec = (cvector_##TYPE*)CVEC_MALLOC(sizeof(cvector_##TYPE)))) {                         \
      CVEC_ASSERT(vec != NULL);                                                                  \
      return NULL;                                                                               \
    }                                                                                            \
                                                                                                 \
    vec->size     = size;                                                                        \
    vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))               \
                        ? capacity                                                               \
                        : vec->size + CVEC_##TYPE##_SZ;                                          \
                                                                                                 \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                          \
      CVEC_ASSERT(vec->a != NULL);                                                               \
      CVEC_FREE(vec);                                                                            \
      return NULL;                                                                               \
    }                                                                                            \
                                                                                                 \
    vec->elem_free = elem_free;                                                                  \
    vec->elem_init = elem_init;                                                                  \
                                                                                                 \
    return vec;                                                                                  \
  }                                                                                              \
                                                                                                 \
  cvector_##TYPE* cvec_init_##TYPE##_heap(TYPE* vals, cvec_sz num, void (*elem_free)(void*),     \
                                          int (*elem_init)(void*, void*))                        \
  {                                                                                              \
    cvector_##TYPE* vec;                                                                         \
    cvec_sz i;                                                                                   \
                                                                                                 \
    if (!(vec = (cvector_##TYPE*)CVEC_MALLOC(sizeof(cvector_##TYPE)))) {                         \
      CVEC_ASSERT(vec != NULL);                                                                  \
      return NULL;                                                                               \
    }                                                                                            \
                                                                                                 \
    vec->capacity = num + CVEC_##TYPE##_SZ;                                                      \
    vec->size     = num;                                                                         \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                          \
      CVEC_ASSERT(vec->a != NULL);                                                               \
      CVEC_FREE(vec);                                                                            \
      return NULL;                                                                               \
    }                                                                                            \
                                                                                                 \
    if (elem_init) {                                                                             \
      for (i = 0; i < num; ++i) {                                                                \
        if (!elem_init(&vec->a[i], &vals[i])) {                                                  \
          CVEC_ASSERT(0);                                                                        \
          CVEC_FREE(vec->a);                                                                     \
          CVEC_FREE(vec);                                                                        \
          return NULL;                                                                           \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE) * num);                                            \
    }                                                                                            \
                                                                                                 \
    vec->elem_free = elem_free;                                                                  \
    vec->elem_init = elem_init;                                                                  \
                                                                                                 \
    return vec;                                                                                  \
  }                                                                                              \
                                                                                                 \
  int cvec_##TYPE(cvector_##TYPE* vec, cvec_sz size, cvec_sz capacity, void (*elem_free)(void*), \
                  int (*elem_init)(void*, void*))                                                \
  {                                                                                              \
    vec->size     = size;                                                                        \
    vec->capacity = (capacity > vec->size || (vec->size && capacity == vec->size))               \
                        ? capacity                                                               \
                        : vec->size + CVEC_##TYPE##_SZ;                                          \
                                                                                                 \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                          \
      CVEC_ASSERT(vec->a != NULL);                                                               \
      vec->size = vec->capacity = 0;                                                             \
      return 0;                                                                                  \
    }                                                                                            \
                                                                                                 \
    vec->elem_free = elem_free;                                                                  \
    vec->elem_init = elem_init;                                                                  \
                                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_init_##TYPE(cvector_##TYPE* vec, TYPE* vals, cvec_sz num, void (*elem_free)(void*),   \
                       int (*elem_init)(void*, void*))                                           \
  {                                                                                              \
    cvec_sz i;                                                                                   \
                                                                                                 \
    vec->capacity = num + CVEC_##TYPE##_SZ;                                                      \
    vec->size     = num;                                                                         \
    if (!(vec->a = (TYPE*)CVEC_MALLOC(vec->capacity * sizeof(TYPE)))) {                          \
      CVEC_ASSERT(vec->a != NULL);                                                               \
      vec->size = vec->capacity = 0;                                                             \
      return 0;                                                                                  \
    }                                                                                            \
                                                                                                 \
    if (elem_init) {                                                                             \
      for (i = 0; i < num; ++i) {                                                                \
        if (!elem_init(&vec->a[i], &vals[i])) {                                                  \
          CVEC_ASSERT(0);                                                                        \
          return 0;                                                                              \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(vec->a, vals, sizeof(TYPE) * num);                                            \
    }                                                                                            \
                                                                                                 \
    vec->elem_free = elem_free;                                                                  \
    vec->elem_init = elem_init;                                                                  \
                                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_copyc_##TYPE(void* dest, void* src)                                                   \
  {                                                                                              \
    cvector_##TYPE* vec1 = (cvector_##TYPE*)dest;                                                \
    cvector_##TYPE* vec2 = (cvector_##TYPE*)src;                                                 \
                                                                                                 \
    vec1->a = NULL;                                                                              \
    vec1->size = 0;                                                                              \
    vec1->capacity = 0;                                                                          \
                                                                                                 \
    return cvec_copy_##TYPE(vec1, vec2);                                                         \
  }                                                                                              \
                                                                                                 \
  int cvec_copy_##TYPE(cvector_##TYPE* dest, cvector_##TYPE* src)                                \
  {                                                                                              \
    int i;                                                                                       \
    TYPE* tmp = NULL;                                                                            \
    if (!(tmp = (TYPE*)CVEC_REALLOC(dest->a, src->capacity*sizeof(TYPE)))) {                     \
      CVEC_ASSERT(tmp != NULL);                                                                  \
      return 0;                                                                                  \
    }                                                                                            \
    dest->a = tmp;                                                                               \
                                                                                                 \
    if (src->elem_init) {                                                                        \
      for (i=0; i<src->size; ++i) {                                                              \
        if (!src->elem_init(&dest->a[i], &src->a[i])) {                                          \
          CVEC_ASSERT(0);                                                                        \
          return 0;                                                                              \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(dest->a, src->a, src->size*sizeof(TYPE));                                     \
    }                                                                                            \
                                                                                                 \
    dest->size = src->size;                                                                      \
    dest->capacity = src->capacity;                                                              \
    dest->elem_free = src->elem_free;                                                            \
    dest->elem_init = src->elem_init;                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_push_##TYPE(cvector_##TYPE* vec, TYPE* a)                                             \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity == vec->size) {                                                            \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
    if (vec->elem_init) {                                                                        \
      if (!vec->elem_init(&vec->a[vec->size], a)) {                                              \
        CVEC_ASSERT(0);                                                                          \
        return 0;                                                                                \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(TYPE));                                         \
    }                                                                                            \
                                                                                                 \
    vec->size++;                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_pushm_##TYPE(cvector_##TYPE* vec, TYPE* a)                                            \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity == vec->size) {                                                            \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
    CVEC_MEMMOVE(&vec->a[vec->size], a, sizeof(TYPE));                                           \
                                                                                                 \
    vec->size++;                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  void cvec_pop_##TYPE(cvector_##TYPE* vec, TYPE* ret)                                           \
  {                                                                                              \
    if (ret) {                                                                                   \
      CVEC_MEMMOVE(ret, &vec->a[--vec->size], sizeof(TYPE));                                     \
    } else {                                                                                     \
      vec->size--;                                                                               \
    }                                                                                            \
                                                                                                 \
    if (vec->elem_free) {                                                                        \
      vec->elem_free(&vec->a[vec->size]);                                                        \
    }                                                                                            \
  }                                                                                              \
                                                                                                 \
  void cvec_popm_##TYPE(cvector_##TYPE* vec, TYPE* ret)                                          \
  {                                                                                              \
    vec->size--;                                                                                 \
    if (ret) {                                                                                   \
      CVEC_MEMMOVE(ret, &vec->a[vec->size], sizeof(TYPE));                                       \
    }                                                                                            \
  }                                                                                              \
                                                                                                 \
  TYPE* cvec_back_##TYPE(cvector_##TYPE* vec) { return &vec->a[vec->size - 1]; }                 \
                                                                                                 \
  int cvec_extend_##TYPE(cvector_##TYPE* vec, cvec_sz num)                                       \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity < vec->size + num) {                                                       \
      tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                           \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
                                                                                                 \
    vec->size += num;                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_insert_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a)                                \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity == vec->size) {                                                            \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
                                                                                                 \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
    CVEC_MEMMOVE(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));                    \
                                                                                                 \
    if (vec->elem_init) {                                                                        \
      if (!vec->elem_init(&vec->a[i], a)) {                                                      \
        CVEC_ASSERT(0);                                                                          \
        return 0;                                                                                \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));                                                 \
    }                                                                                            \
                                                                                                 \
    vec->size++;                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_insertm_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a)                               \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity == vec->size) {                                                            \
      tmp_sz = RESIZE_MACRO(vec->capacity);                                                      \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
                                                                                                 \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
    CVEC_MEMMOVE(&vec->a[i + 1], &vec->a[i], (vec->size - i) * sizeof(TYPE));                    \
                                                                                                 \
    CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));                                                   \
                                                                                                 \
    vec->size++;                                                                                 \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_insert_array_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num)             \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz, j;                                                                           \
    if (vec->capacity < vec->size + num) {                                                       \
      tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                           \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
                                                                                                 \
    CVEC_MEMMOVE(&vec->a[i + num], &vec->a[i], (vec->size - i) * sizeof(TYPE));                  \
    if (vec->elem_init) {                                                                        \
      for (j = 0; j < num; ++j) {                                                                \
        if (!vec->elem_init(&vec->a[j + i], &a[j])) {                                            \
          CVEC_ASSERT(0);                                                                        \
          return 0;                                                                              \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(&vec->a[i], a, num * sizeof(TYPE));                                           \
    }                                                                                            \
    vec->size += num;                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_insert_arraym_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, cvec_sz num)            \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    cvec_sz tmp_sz;                                                                              \
    if (vec->capacity < vec->size + num) {                                                       \
      tmp_sz = vec->capacity + num + CVEC_##TYPE##_SZ;                                           \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * tmp_sz))) {                         \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = tmp_sz;                                                                    \
    }                                                                                            \
                                                                                                 \
    CVEC_MEMMOVE(&vec->a[i + num], &vec->a[i], (vec->size - i) * sizeof(TYPE));                  \
                                                                                                 \
    CVEC_MEMMOVE(&vec->a[i], a, num * sizeof(TYPE));                                             \
    vec->size += num;                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_replace_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, TYPE* ret)                    \
  {                                                                                              \
    if (ret) {                                                                                   \
      CVEC_MEMMOVE(ret, &vec->a[i], sizeof(TYPE));                                               \
    } else if (vec->elem_free) {                                                                 \
      vec->elem_free(&vec->a[i]);                                                                \
    }                                                                                            \
                                                                                                 \
    if (vec->elem_init) {                                                                        \
      if (!vec->elem_init(&vec->a[i], a)) {                                                      \
        CVEC_ASSERT(0);                                                                          \
        return 0;                                                                                \
      }                                                                                          \
    } else {                                                                                     \
      CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));                                                 \
    }                                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  void cvec_replacem_##TYPE(cvector_##TYPE* vec, cvec_sz i, TYPE* a, TYPE* ret)                  \
  {                                                                                              \
    if (ret) {                                                                                   \
      CVEC_MEMMOVE(ret, &vec->a[i], sizeof(TYPE));                                               \
    }                                                                                            \
                                                                                                 \
    CVEC_MEMMOVE(&vec->a[i], a, sizeof(TYPE));                                                   \
  }                                                                                              \
                                                                                                 \
  void cvec_erase_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end)                        \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    cvec_sz d = end - start + 1;                                                                 \
    if (vec->elem_free) {                                                                        \
      for (i = start; i <= end; i++) {                                                           \
        vec->elem_free(&vec->a[i]);                                                              \
      }                                                                                          \
    }                                                                                            \
    CVEC_MEMMOVE(&vec->a[start], &vec->a[end + 1], (vec->size - 1 - end) * sizeof(TYPE));        \
    vec->size -= d;                                                                              \
  }                                                                                              \
                                                                                                 \
  void cvec_remove_##TYPE(cvector_##TYPE* vec, cvec_sz start, cvec_sz end)                       \
  {                                                                                              \
    cvec_sz d = end - start + 1;                                                                 \
    CVEC_MEMMOVE(&vec->a[start], &vec->a[end + 1], (vec->size - 1 - end) * sizeof(TYPE));        \
    vec->size -= d;                                                                              \
  }                                                                                              \
                                                                                                 \
  int cvec_reserve_##TYPE(cvector_##TYPE* vec, cvec_sz size)                                     \
  {                                                                                              \
    TYPE* tmp;                                                                                   \
    if (vec->capacity < size) {                                                                  \
      if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * (size + CVEC_##TYPE##_SZ)))) {      \
        CVEC_ASSERT(tmp != NULL);                                                                \
        return 0;                                                                                \
      }                                                                                          \
      vec->a        = tmp;                                                                       \
      vec->capacity = size + CVEC_##TYPE##_SZ;                                                   \
    }                                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_set_cap_##TYPE(cvector_##TYPE* vec, cvec_sz size)                                     \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    TYPE* tmp;                                                                                   \
    if (size < vec->size) {                                                                      \
      if (vec->elem_free) {                                                                      \
        for (i = vec->size - 1; i >= size; i--) {                                                \
          vec->elem_free(&vec->a[i]);                                                            \
        }                                                                                        \
      }                                                                                          \
      vec->size = size;                                                                          \
    }                                                                                            \
                                                                                                 \
    vec->capacity = size;                                                                        \
                                                                                                 \
    if (!(tmp = (TYPE*)CVEC_REALLOC(vec->a, sizeof(TYPE) * size))) {                             \
      CVEC_ASSERT(tmp != NULL);                                                                  \
      return 0;                                                                                  \
    }                                                                                            \
    vec->a = tmp;                                                                                \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_set_val_sz_##TYPE(cvector_##TYPE* vec, TYPE* val)                                     \
  {                                                                                              \
    cvec_sz i;                                                                                   \
                                                                                                 \
    if (vec->elem_free) {                                                                        \
      for (i = 0; i < vec->size; i++) {                                                          \
        vec->elem_free(&vec->a[i]);                                                              \
      }                                                                                          \
    }                                                                                            \
                                                                                                 \
    if (vec->elem_init) {                                                                        \
      for (i = 0; i < vec->size; i++) {                                                          \
        if (!vec->elem_init(&vec->a[i], val)) {                                                  \
          CVEC_ASSERT(0);                                                                        \
          return 0;                                                                              \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      for (i = 0; i < vec->size; i++) {                                                          \
        CVEC_MEMMOVE(&vec->a[i], val, sizeof(TYPE));                                             \
      }                                                                                          \
    }                                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  int cvec_set_val_cap_##TYPE(cvector_##TYPE* vec, TYPE* val)                                    \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    if (vec->elem_free) {                                                                        \
      for (i = 0; i < vec->size; i++) {                                                          \
        vec->elem_free(&vec->a[i]);                                                              \
      }                                                                                          \
      vec->size = vec->capacity;                                                                 \
    }                                                                                            \
                                                                                                 \
    if (vec->elem_init) {                                                                        \
      for (i = 0; i < vec->capacity; i++) {                                                      \
        if (!vec->elem_init(&vec->a[i], val)) {                                                  \
          CVEC_ASSERT(0);                                                                        \
          return 0;                                                                              \
        }                                                                                        \
      }                                                                                          \
    } else {                                                                                     \
      for (i = 0; i < vec->capacity; i++) {                                                      \
        CVEC_MEMMOVE(&vec->a[i], val, sizeof(TYPE));                                             \
      }                                                                                          \
    }                                                                                            \
    return 1;                                                                                    \
  }                                                                                              \
                                                                                                 \
  void cvec_clear_##TYPE(cvector_##TYPE* vec)                                                    \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    if (vec->elem_free) {                                                                        \
      for (i = 0; i < vec->size; ++i) {                                                          \
        vec->elem_free(&vec->a[i]);                                                              \
      }                                                                                          \
    }                                                                                            \
    vec->size = 0;                                                                               \
  }                                                                                              \
                                                                                                 \
  void cvec_free_##TYPE##_heap(void* vec)                                                        \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                                  \
    if (!tmp) return;                                                                            \
    if (tmp->elem_free) {                                                                        \
      for (i = 0; i < tmp->size; i++) {                                                          \
        tmp->elem_free(&tmp->a[i]);                                                              \
      }                                                                                          \
    }                                                                                            \
    CVEC_FREE(tmp->a);                                                                           \
    CVEC_FREE(tmp);                                                                              \
  }                                                                                              \
                                                                                                 \
  void cvec_free_##TYPE(void* vec)                                                               \
  {                                                                                              \
    cvec_sz i;                                                                                   \
    cvector_##TYPE* tmp = (cvector_##TYPE*)vec;                                                  \
    if (tmp->elem_free) {                                                                        \
      for (i = 0; i < tmp->size; i++) {                                                          \
        tmp->elem_free(&tmp->a[i]);                                                              \
      }                                                                                          \
    }                                                                                            \
                                                                                                 \
    CVEC_FREE(tmp->a);                                                                           \
    tmp->a        = NULL;                                                                        \
    tmp->size     = 0;                                                                           \
    tmp->capacity = 0;                                                                           \
  }



/* header ends */
#endif


#define RESIZE(x) ((x+1)*2)

