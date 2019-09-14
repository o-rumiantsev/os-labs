#include <stdio.h>
#include "allocator.h"
#include "dump.h"

int main() {

    {
        printf("TEST 1: memory initialization\n");
        mem_init(0);
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1048576
         *   Blocks count: 1
         *   Blocks info:
         *       1) Free: true, Size: 1048576, Address: 0
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 2: memory allocation\n");
        mem_init(1024);
        mem_alloc(512);
        /*
         *  ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *      1) Free: false, Size: 512, Address: 0
         *      2) Free: true, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 3: memory allocation (return NULL)\n");
        mem_init(1024);
        mem_alloc(512);
        mem_alloc(512);
        void *p = mem_alloc(1);
        printf("Returned NULL: %s\n", p == NULL ? "true" : "false");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *       1) Free: false, Size: 512, Address: 0
         *       2) Free: false, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 4: memory allocation (align size)\n");
        mem_init(1024);
        mem_alloc(1);
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *       1) Free: false, Size: 4, Address: 0
         *       2) Free: true, Size: 1020, Address: 4
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 5: memory freeing (no merge)\n");
        mem_init(1024);

        void *p = mem_alloc(256);
        mem_alloc(256);

        printf("Before freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: false, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: true, Size: 512, Address: 512
         */
        mem_dump();

        mem_free(p);

        printf("After freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: true, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: true, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 6: memory freeing (merge with previous free)\n");
        mem_init(1024);

        void *p1 = mem_alloc(512);
        void *p2 = mem_alloc(256);
        mem_alloc(256);
        mem_free(p1);

        printf("Before freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: true, Size: 512, Address: 0
         *       2) Free: false, Size: 256, Address: 512
         *       3) Free: false, Size: 256, Address: 768
         */
        mem_dump();

        mem_free(p2);

        printf("After freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *       1) Free: true, Size: 768, Address: 0
         *       2) Free: false, Size: 256, Address: 768
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 7: memory freeing (merge with next free)\n");
        mem_init(1024);

        mem_alloc(512);
        void *p = mem_alloc(256);

        printf("Before freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *        1) Free: false, Size: 512, Address: 0
         *        2) Free: false, Size: 256, Address: 512
         *        3) Free: true, Size: 256, Address: 768
         */
        mem_dump();

        mem_free(p);

        printf("After freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 1
         *   Blocks info:
         *       1) Free: false, Size: 512, Address: 0
         *       2) Free: true, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf(
            "TEST 8: memory freeing (merge with both next and previous free)\n"
        );
        mem_init(1024);

        void *p1 =mem_alloc(512);
        void *p2 = mem_alloc(256);
        mem_free(p1);

        printf("Before freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *        1) Free: true, Size: 512, Address: 0
         *        2) Free: false, Size: 256, Address: 512
         *        3) Free: true, Size: 256, Address: 768
         */
        mem_dump();

        mem_free(p2);

        printf("After freeing\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 1
         *   Blocks info:
         *       1) Free: true, Size: 1024, Address: 0
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 9: memory reallocation (increase size)\n");
        mem_init(1024);

        void *p = mem_alloc(256);
        mem_alloc(256);

        printf("Before reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: false, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: true, Size: 512, Address: 512
         */
        mem_dump();

        mem_realloc(p, 400);

        printf("After reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 4
         *   Blocks info:
         *       1) Free: true, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: false, Size: 400, Address: 512
         *       4) Free: true, Size: 112, Address: 912
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 10: memory reallocation (decrease size)\n");
        mem_init(1024);

        void *p = mem_alloc(256);
        mem_alloc(256);

        printf("Before reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: false, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: true, Size: 512, Address: 512
         */
        mem_dump();

        mem_realloc(p, 16);

        printf("After reallocation\n");
        /*
         *  ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 4
         *   Blocks info:
         *       1) Free: false, Size: 16, Address: 0
         *       2) Free: true, Size: 240, Address: 16
         *       3) Free: false, Size: 256, Address: 256
         *       4) Free: true, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 11: memory reallocation (borrow from next block)\n");
        mem_init(1024);

        void *p = mem_alloc(256);

        printf("Before reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *       1) Free: false, Size: 256, Address: 0
         *       2) Free: true, Size: 768, Address: 256
         */
        mem_dump();

        mem_realloc(p, 300);

        printf("After reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 2
         *   Blocks info:
         *       1) Free: false, Size: 300, Address: 0
         *       2) Free: true, Size: 724, Address: 300
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 12: memory reallocation (borrow from previous block)\n");
        mem_init(1024);

        void *p1 = mem_alloc(256);
        void *p2 = mem_alloc(256);
        mem_alloc(512);

        mem_free(p1);

        printf("Before reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: true, Size: 256, Address: 0
         *       2) Free: false, Size: 256, Address: 256
         *       3) Free: false, Size: 512, Address: 512
         */
        mem_dump();

        mem_realloc(p2, 300);

        printf("After reallocation\n");
        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 3
         *   Blocks info:
         *       1) Free: true, Size: 212, Address: 0
         *       2) Free: false, Size: 300, Address: 212
         *       3) Free: false, Size: 512, Address: 512
         */
        mem_dump();
        mem_drop();
    }

    {
        printf("TEST 13: complex\n");
        mem_init(1024);

        int *a = (int *)mem_alloc(5 * sizeof(int));
        char *s = (char *)mem_alloc(10);

        a[0] = 1;
        a[1] = 2;
        a[2] = 3;
        a[3] = 4;
        a[4] = 5;

        printf("Data addressed by 'a' before realloc:\n");
        for (int i = 0; i < 5; ++i) {
            printf("%d\n", a[i]);
        }

        a = (int *)mem_realloc(a, 10 * sizeof(int));

        printf("Data addressed by 'a' after realloc:\n");
        for (int i = 0; i < 10; ++i) {
            printf("%d\n", a[i]);
        }

        mem_free(s);
        mem_free(a);

        /*
         *   ========= Memory Dump =========
         *   Heap size: 1024
         *   Blocks count: 1
         *   Blocks info:
         *       1) Free: true, Size: 1024, Address: 0
         */
        mem_dump();
        mem_drop();
    }

    return 0;
}
