#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdlib.h>
#define DEFAULT_MEM_SIZE 1024 * 1024 // 1 MB

/*
 * Memory heap structure
 */
struct {
    void *bytes;
    size_t size;
    size_t blocks_count;
} mem_heap;

/*
 * Memory block header type
 */
typedef struct mem_block_header_t mem_block_header_t;
struct mem_block_header_t {
    char is_free;
    size_t size;
    void *addr;
    mem_block_header_t *next;
    mem_block_header_t *prev;
};

/*
 * List of memory block headers
 */
struct {
    char order;
    mem_block_header_t *start;
    mem_block_header_t *end;
} mem_blocks;

/*
 * Initialize heap
 */
int mem_init(size_t);

/*
 * Free heap
 */
void mem_drop();

/*
 * Allocate memory
 */
void *mem_alloc(size_t);

/*
 * Reallocate memory
 */
void *mem_realloc(void *, size_t);

/*
 * Free memory
 */
void mem_free(void *);

#endif