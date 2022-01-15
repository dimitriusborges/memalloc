#include <stdint.h>
#include <stddef.h>

typedef struct POOL_POINTER{
    struct POOL_POINTER *next;
}PoolPointer;

typedef struct POOL{
    unsigned char *mem_bank;
    size_t bank_s;
    size_t chunk_size;
    size_t number_chunks;
    PoolPointer *head;
}Pool;

void pool_init(Pool *pool, unsigned char *mem_bank, size_t bank_s, size_t chunk_size, size_t align_bytes);

void* pool_alloc(Pool *pool, size_t n_bytes_alloc);

void pool_free_all(Pool *pool);

void pool_free(Pool *pool, void *addr_to_free);