#ifndef __MEM_ALLOC_H
#define __MEM_ALLOC_H

#include <stddef.h>

#define MAX_INDEX 20
#define HEAP_SIZE (1 << MAX_INDEX)


char mem_heap[HEAP_SIZE];

void mem_init();
void *mem_alloc(size_t size);
void mem_free(void *zone, size_t size);
void mem_show(void (*print)(void *zone, size_t size));

#endif
