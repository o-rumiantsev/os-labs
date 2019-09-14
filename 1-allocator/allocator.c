#include <stdio.h>
#include <string.h>
#include "allocator.h"

/*
 * Flag to determine searching direction. from start - 1, from end - 0
 */
#define FROM_START 1

/*
 * Align given size by 4
 */
size_t align(size_t);

/*
 * Find free block with corresponding size
 */
mem_block_header_t *find_free_block(size_t);

/*
 * Find block, responsible for with given address
 */
mem_block_header_t *find_block(void *);

/*
 * Get address for reallocation given address with given size
 */
void *get_realloc_addr(mem_block_header_t *, size_t);

/*
 * Split memory block in two parts, where one will have given size
 */
void split_mem_block(mem_block_header_t *, size_t);

/*
 * Merge memory blocks with given headers. Second one will be merged in first
 */
void merge_mem_blocks(mem_block_header_t *, mem_block_header_t *);

/*
 * Mark block header as free and merge with neighbours whether is able to
 */
void free_block(mem_block_header_t *);

int mem_init(size_t size) {
    if (mem_heap.bytes != NULL) {
        return 0;
    }

    if (size < sizeof(mem_block_header_t)) {
        size = DEFAULT_MEM_SIZE;
    }

    mem_heap.bytes = malloc(size);

    if (mem_heap.bytes == NULL) {
        return 0;
    }

    mem_heap.size = size;

    mem_block_header_t *block =
        (mem_block_header_t *)malloc(sizeof(mem_block_header_t));

    block->is_free = 1;
    block->addr = mem_heap.bytes;
    block->size = size;
    block->next = NULL;
    block->prev = NULL;

    mem_heap.blocks_count = 1;

    mem_blocks.order = FROM_START;
    mem_blocks.start = block;
    mem_blocks.end = block;

    return 1;
};

void mem_drop() {
    if (mem_heap.bytes == NULL) {
        return;
    }

    mem_block_header_t *block = mem_blocks.start;

    do {
        free_block(block);

        block = block->next;
    } while (block != NULL);

    free(mem_heap.bytes);
    mem_heap.bytes = NULL;
};

void *mem_alloc(size_t size) {
    size = align(size);

    mem_block_header_t *block = find_free_block(size);

    if (block == NULL) {
       return NULL;
    }

    split_mem_block(block, size);

    block->is_free = 0;
    return block->addr;
};

void *mem_realloc(void *addr, size_t size) {
    if (addr == NULL) {
        return mem_alloc(size);
    }

    if (size == 0) {
        mem_free(addr);
        return NULL;
    }

    mem_block_header_t *block = find_block(addr);

    return get_realloc_addr(block, size);
};

void mem_free(void *addr) {
    mem_block_header_t *block = find_block(addr);

    if (block == NULL) {
        return;
    }

    free_block(block);
};

size_t align(size_t size) {
    if (size % 4 == 0) {
        return size;
    } else {
        return size + 4 - size % 4;
    }
};

mem_block_header_t *find_free_block(size_t size) {
    mem_block_header_t *block;

    if (mem_blocks.order == FROM_START) {
        block = mem_blocks.start;
        while (!block->is_free || block->size < size) {
            if (block->next == NULL) {
                  return NULL;
            }
            block = block->next;
        }
    } else {
        block = mem_blocks.end;
        while (!block->is_free || block->size < size) {
            if (block->prev == NULL) {
                  return NULL;
            }
            block = block->prev;
        }
    }

    mem_blocks.order = !mem_blocks.order;

    return block;
};

mem_block_header_t *find_block(void *addr) {
    mem_block_header_t *block = mem_blocks.start;

    while (block->addr != addr) {
        if (block->next == NULL) {
            return NULL;
        }
        block = block->next;
    }

    return block;
};

void *get_realloc_addr(mem_block_header_t *block, size_t size) {
    size = align(size);

    void *old_addr = block->addr;
    size_t old_size = block->size;
    size_t delta = size - old_size;

    if (old_size == size) {
        return block->addr;
    }

    if (old_size > size) {
        split_mem_block(block, size);
        mem_free(block->next);
        return block->addr;
    }

    if (
        block->next &&
        block->next->is_free &&
        block->next->size >= delta
    ) {
        split_mem_block(block->next, delta);
        merge_mem_blocks(block, block->next);
        return block->addr;
    }

    if (
        block->prev &&
        block->prev->is_free &&
        block->prev->size >= delta
    ) {
        delta = block->prev->size - delta;
        mem_block_header_t *prev = block->prev;
        split_mem_block(block->prev, delta);
        merge_mem_blocks(block->prev, block);
        block->prev->is_free = 0;
        memmove(block->prev->addr, old_addr, old_size);
        return block->prev->addr;
    }

    mem_block_header_t *free_block = find_free_block(size);

    if (free_block == NULL) {
        return NULL;
    }

    free_block->is_free = 0;
    split_mem_block(free_block, size);
    memcpy(free_block->addr, old_addr, old_size);

    mem_free(old_addr);

    return free_block->addr;
};

void split_mem_block(mem_block_header_t *block, size_t size) {
    if (block->size >= size + 4) {
        mem_heap.blocks_count += 1;

        mem_block_header_t *next_block =
            (mem_block_header_t *)malloc(sizeof(mem_block_header_t));

        next_block->is_free = 1;
        next_block->size = align(block->size - size);
        next_block->addr = (void *)((size_t)block->addr + size);
        next_block->next = block->next;
        next_block->prev = block;

        block->size = size;
        block->next = next_block;

        if (next_block->next == NULL) {
            mem_blocks.end = next_block;
        } else if (next_block->next->is_free) {
            merge_mem_blocks(next_block, next_block->next);
        } else {
            next_block->next->prev = next_block;
        }
    }
};

void merge_mem_blocks(mem_block_header_t *block1, mem_block_header_t *block2) {
    block1->next = block2->next;
    block1->size += block2->size;

    if (block2->next != NULL) {
        block2->next->prev = block1;
    } else {
        mem_blocks.end = block1;
    }

    mem_heap.blocks_count -= 1;
    free(block2);
};

void free_block(mem_block_header_t *block) {
    if (block->next && block->next->is_free) {
        merge_mem_blocks(block, block->next);
    }

    if (block->prev && block->prev->is_free) {
        merge_mem_blocks(block->prev, block);
    } else {
        block->is_free = 1;
    }
};
