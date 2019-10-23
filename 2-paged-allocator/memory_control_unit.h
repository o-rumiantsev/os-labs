#ifndef MEMORY_TABLE_H
#define MEMORY_TABLE_H

#include "list.h"

#define bool char
#define true 1
#define false 0

struct memory_control_unit {
    int                  size;
    int           lists_count;
    void             *address;
    struct list_node **blocks;
};

struct memory_block {
    int     size;
    char is_free;
    void   *data;
};

/**
 * Create memory control unit struct and allocate pages
 */
struct memory_control_unit initialize_memory_unit(int size);

/**
 * Get free block from memory control unit
 */
struct memory_block *find_free_block(int size, struct memory_control_unit mcu);

/**
 * Remove block
 */
void remove_block(struct memory_block *block, struct memory_control_unit mcu);

/**
 * Add block
 */
void add_block(struct memory_block *block, struct memory_control_unit mcu);

/**
 * Get the minimum number which is greater than or
 * equal to `number` which is a power of two
 */
int min_pow_of_two(int number);

/**
 *
 */
int blocks_index(int number);

#endif
