#ifndef STRING_COMPARE_H
#define STRING_COMPARE_H

extern inline char tupper(char b);
extern inline char isnum(char b);
extern inline i64 parsenum(char **a_p);
int StringCompare(char *a, char *b);
int StringCompareSort(const void *p, const void *q);

#endif
