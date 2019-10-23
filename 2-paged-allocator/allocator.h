#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "memory_control_unit.h"

struct {
    struct memory_control_unit mcu;
    struct memory_block   **blocks;
} memory;

/**
 * Initialize memory control unit
 */
void mem_init(int size);

/**
 * Dump memory info
 */
void mem_dump();

/**
 * Allocate memory
 */
void *mem_alloc(int size);

/**
 * Free memory
 */
void mem_free(void *data);

#endif
