#include <stdio.h>
#include "allocator.h"
#include "colors.h"
#include "dump.h"

void mem_dump() {
    printf(COLOR_WHITE "========= Memory Dump =========\n" COLOR_RESET);
    printf("Heap size: ");
    printf(COLOR_YELLOW "%d\n" COLOR_RESET, mem_heap.size);
    printf("Blocks count: ");
    printf(COLOR_YELLOW "%d\n" COLOR_RESET, mem_heap.blocks_count);
    printf("Blocks info:\n");

    mem_block_header_t *block = mem_blocks.start;

    /*
     *  Print detailed blocks info in form of list of items:
     *
     *      n) Free: <true|false>, Size: <size_t>, Address: <size_t>
     */
    for (int i = 1; i <= mem_heap.blocks_count; ++i) {
        printf("    %d) Free: ", i);
        printf(
            COLOR_YELLOW "%s" COLOR_RESET,
            block->is_free ? "true" : "false"
        );
        printf(", Size: ");
        printf(COLOR_YELLOW "%d" COLOR_RESET, block->size);
        printf(", Address: ");
        printf(COLOR_YELLOW "%d" COLOR_RESET, block->addr - mem_heap.bytes);
        printf("\n");
        block = block->next;
    }

    printf(COLOR_WHITE "======= Memory Dump End =======\n" COLOR_RESET);
};
