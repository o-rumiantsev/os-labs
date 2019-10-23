#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "allocator.h"

const int ITERATIONS_COUNT = 100;
const int ALLOCATION_UNITS_COUNT = 10;

int random_in_range(int start, int end) {
    return random() % end + start;
}

int main() {
    srandom(time(0));


    mem_init(1024);


    void **data = (void **)malloc(10 * sizeof(void *));


    for (int i = 0; i < ITERATIONS_COUNT; ++i) {
        for (int j = 0; j < ALLOCATION_UNITS_COUNT; ++j) {
            int size = random_in_range(0, 256);
            data[j] = mem_alloc(size);
        }

        for (int j = 0; j < ALLOCATION_UNITS_COUNT; ++j) {
            mem_free(data[j]);
            data[j] = NULL;
        }
    }

    mem_dump();

    return 0;
}
