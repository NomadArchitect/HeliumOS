#ifndef CSTD_STDLIB_H
#define CSTD_STDLIB_H

#include <error.h>
#include <stddef.h>

void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);

void *realloc(void *ptr, size_t size);
void *reallocarray(void *ptr, size_t nmemb, size_t size);

void free(void *ptr);

// Non standard
void *alloc_block(size_t size, int flags);
void free_block(void *vadr, size_t size);

#endif