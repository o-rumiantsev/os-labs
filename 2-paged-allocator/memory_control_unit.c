#include <stdio.h>
#include <stdlib.h>
#include "memory_control_unit.h"

/**
 * Get the minimum number which is greater than or
 * equal to `number` which is a blocks_index of two
 */
int min_pow_of_two(int number) {
    int i = 1;
    while (number > i) {
        i *= 2;
    }
    return i;
}

/**
 *
 */
int blocks_index(int number) {
    int i = 0;
    int j = 1;
    while (number > j) {
        j *= 2;
        ++i;
    }
    return i;
}

/**
 * Create memory control unit struct and allocate pages
 */
struct memory_control_unit initialize_memory_unit(int size) {
    int aligned_size = min_pow_of_two(size);
    int lists_max_index = blocks_index(aligned_size);
    int lists_size = (lists_max_index + 1) * sizeof(struct list_node *);

    struct memory_control_unit mcu;
    mcu.size = aligned_size;
    mcu.lists_count = lists_max_index + 1;
    mcu.blocks = (struct list_node **)malloc(lists_size);

    struct memory_block *block = (struct memory_block *)malloc(aligned_size);
    block->size = aligned_size;
    block->is_free = true;
    block->data = malloc(aligned_size);

    add_block(block, mcu);

    mcu.address = block->data;
    return mcu;
};

bool is_free(void *block) {
    return ((struct memory_block *)block)->is_free == 1;
};

/**
 * Get free block from memory control unit
 */
struct memory_block *find_free_block(int size, struct memory_control_unit mcu) {
    int aligned_size = min_pow_of_two(size);
    int index = blocks_index(aligned_size);
    struct list_node *node = find_node(mcu.blocks[index], &is_free);
    if (node) {
        return (struct memory_block *)node->data;
    }
    while (index < mcu.lists_count - 1) {
        node = find_node(mcu.blocks[++index], &is_free);
        if (node) {
            return (struct memory_block *)node->data;
        }
    }
    return NULL;
};

/**
 * Remove block
 */
void remove_block(struct memory_block *block, struct memory_control_unit mcu) {
    int index = blocks_index(block->size);
    struct list_node *node = mcu.blocks[index];

    while ((struct memory_block *)node->data != block) {
        node = node->next;
    }

    if (node == mcu.blocks[index]) {
        mcu.blocks[index] = node->next;
    }

    remove_node(node);
};

/**
 * Add block
 */
void add_block(struct memory_block *block, struct memory_control_unit mcu) {
    int index = blocks_index(block->size);
    struct list_node *node = mcu.blocks[index];

    if (node) {
        node->prev = create_node(block);
        node->prev->next = node;
        mcu.blocks[index] = node->prev;
    } else {
        mcu.blocks[index] = create_node(block);
    }
};
