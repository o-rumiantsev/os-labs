#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "allocator.h"

int compute_index(void *data);
struct memory_block *split(int size, struct memory_block *block);
struct memory_block *find_buddy(struct memory_block *block);
struct memory_block *merge_buddies(
    struct memory_block *block1,
    struct memory_block *block2
);
void free_block(struct memory_block *block);
void alloc_remove_block(struct memory_block *block);
void alloc_add_block(struct memory_block *block);

/**
 * Initialize memory control unit
 */
void mem_init(int size) {
    struct memory_control_unit mcu = initialize_memory_unit(size);
    memory.mcu = mcu;

    int blocks_size = mcu.size * sizeof(struct memory_block *);
    struct memory_block **blocks = (struct memory_block **)malloc(blocks_size);
    memory.blocks = blocks;

    struct list_node *block_node = mcu.blocks[mcu.lists_count - 1];
    struct memory_block *block = (struct memory_block *)block_node->data;
    int index = compute_index(block->data);

    blocks[index] = block;
}

/**
 * Dump memory info
 */
void mem_dump() {
    struct memory_control_unit mcu = memory.mcu;

    printf("---- MEMORY DUMP -----\n");

    for (int i = 0; i < mcu.size; ++i) {
        if (memory.blocks[i] != NULL) {
            printf("ADDRESS: %d -> SIZE: %d\n", i, memory.blocks[i]->size);
        }
    }

    for (int i = 0; i < mcu.lists_count; ++i) {
        printf("--- %.0fb ---\n", pow(2, i));

        struct list_node *node = mcu.blocks[i];
        while (node) {
            struct memory_block *block = (struct memory_block *)node->data;
            printf(
                "    IS FREE: %d, SIZE: %d, ADDRESS: %d\n",
                block->is_free,
                block->size,
                compute_index(block->data)
            );
            node = node->next;
        }
    }

    printf("---- MEMORY DUMP -----\n");
}

/**
 * Allocate memory
 */
void *mem_alloc(int size) {
    struct memory_block *block = find_free_block(size, memory.mcu);

    if (!block) {
        return NULL;
    }

    block = split(size, block);
    block->is_free = false;

    return block->data;
}


/**
 * Free memory
 */
void mem_free(void *data) {
    if (!data) {
        return;
    }

    int index = compute_index(data);
    struct memory_block *block = memory.blocks[index];

    free_block(block);
}

void free_block(struct memory_block *block) {
    struct memory_block *buddy = find_buddy(block);
    block->is_free = true;

    if (!buddy) {
        return;
    }

    alloc_remove_block(block);
    alloc_remove_block(buddy);

    struct memory_block *merged = merge_buddies(block, buddy);

    alloc_add_block(merged);

    free_block(merged);
}

int compute_index(void *data) {
    return (int)((size_t)data - (size_t)memory.mcu.address);
}

void alloc_remove_block(struct memory_block *block) {
    struct memory_control_unit mcu = memory.mcu;
    int index = compute_index(block->data);
    memory.blocks[index] = NULL;
    remove_block(block, memory.mcu);
}

void alloc_add_block(struct memory_block *block) {
    struct memory_control_unit mcu = memory.mcu;
    int index = compute_index(block->data);
    memory.blocks[index] = block;
    add_block(block, memory.mcu);
}

struct memory_block *split(int size, struct memory_block *block) {
    if (block->size == min_pow_of_two(size)) {
        return block;
    }

    int sub_block_size = block->size / 2;

    struct memory_block *block1 =
        (struct memory_block *)malloc(sizeof(struct memory_block *));

    struct memory_block *block2 =
        (struct memory_block *)malloc(sizeof(struct memory_block *));

    block1->size = sub_block_size;
    block1->is_free = true;
    block1->data = block->data;

    block2->size = sub_block_size;
    block2->is_free = true;
    block2->data = (void *)((size_t)block->data + (size_t)sub_block_size);

    alloc_remove_block(block);

    alloc_add_block(block1);
    alloc_add_block(block2);

    return split(size, block1);
}


bool are_buddies(struct memory_block *block1, struct memory_block *block2) {
    if (block1->size != block2->size) {
        return false;
    }

    int block1_address = compute_index(block1->data);
    int block2_address = compute_index(block2->data);

    return (block1_address + block1->size) == block2_address;
}

struct memory_block *find_buddy(struct memory_block *block) {
    int aligned_size = min_pow_of_two(block->size);
    int index = blocks_index(aligned_size);

    struct list_node *node = memory.mcu.blocks[index];

    while (node  && !are_buddies(block, node->data)) {
        node = node->next;
        if (!node) {
            return NULL;
        }
    }

    struct memory_block *buddy = (struct memory_block *)node->data;

    if (!buddy->is_free) {
        return NULL;
    }

    return buddy;
}


struct memory_block *merge_buddies(
    struct memory_block *block1,
    struct memory_block *block2
) {
    block1->size *= 2;
//    free(block2);
    return block1;
}
