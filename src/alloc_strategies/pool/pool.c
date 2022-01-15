#include <string.h>
#include <stdio.h>
#include "pool.h"
#include <assert.h>

void pool_free_all(Pool *pool){

    for(int idx = 0; idx < pool->number_chunks; idx++){
        void *aux = &pool->mem_bank[idx * pool->chunk_size];
        PoolPointer *node = (PoolPointer *) aux;
        node->next = pool->head;
        pool->head = node;
    }

}

void pool_init(Pool *pool, unsigned char *mem_bank, size_t bank_s, size_t chunk_size, size_t align_bytes){
        
    pool->mem_bank = mem_bank;
    pool->bank_s = bank_s;
    pool->chunk_size = chunk_size;
    pool->number_chunks = bank_s/chunk_size;
    pool->head = NULL;

    memset(pool->mem_bank, 0, pool->bank_s);

    pool_free_all(pool);

    printf("Creating Pool with %ld positions...\n", pool->number_chunks);

    printf("Memory pool going from %p to %p\n\n\n", pool->mem_bank, pool->head);
}

void* pool_alloc(Pool *pool, size_t n_bytes_alloc){

    assert(n_bytes_alloc <= pool->chunk_size && "Size asked for allocation is bigger than a chunk! Can't allocate");
    
    PoolPointer *free_pos = pool->head;
    
    if(free_pos == NULL){
        printf("Pool already full!\n\n");
        return NULL;
    }

    pool->head = pool->head->next;

    return free_pos;
}

void pool_free(Pool *pool, void *addr_to_free){
    void *start = pool->mem_bank;
    void *end = &pool->mem_bank[pool->bank_s - 1];

    if(addr_to_free == NULL){
        return;
    }

    if(!(start <= addr_to_free && addr_to_free<end)){
        printf("Addr passed (%p) is out of bounds (%p -> %p)\n", 
            addr_to_free, start, end);
        return;
    }

    printf("Freeing pos: %p\n", addr_to_free);
    PoolPointer *node = (PoolPointer *) addr_to_free;
    memset(node, 0, pool->chunk_size);

    node->next = pool->head;
    pool->head->next = node;
}
