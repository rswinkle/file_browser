
#include "myinttypes.h"

#define CVEC_ONLY_INT
#define CVEC_ONLY_STR
#define CVEC_SIZE_T i64
#define PRIcv_sz PRIiMAX
#include "cvector.h"

#define RESIZE(x) ((x+1)*2)

