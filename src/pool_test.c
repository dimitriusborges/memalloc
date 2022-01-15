#include <stdio.h>
#include "alloc_strategies/pool/pool.h"

// gcc pool_test.c alloc_strategies/pool/*.c alloc_strategies/*.c -o output/pool
void main(){
    Pool pool;
    size_t mem_bank_s = 1024;
    unsigned char mem_bank[mem_bank_s];

    pool_init(&pool, mem_bank, mem_bank_s, 8, __WORDSIZE/8);

    PoolPointer *ptr = pool.head;

    // while(ptr != NULL){
    //     printf("Chunk at pos: %p\n", ptr);
    //     ptr = ptr->next;
    // }

    void *free_pos = pool_alloc(&pool, 8);

    printf("Free position: %p\n", free_pos);

    pool_free(&pool, free_pos);

}