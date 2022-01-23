# Pool

## Definition
Pool allocation uses a simple linear memory block, divided into chunks, i.e., smaller blocks with a **pre-define size**. Every allocation request receives one of this blocks, as long it does not exceed the max size. For example, in a Pool where every chunk has a size of 8 bytes, any allocation request, going from 1 to 8 bytes, will receive a chunck of 8 bytes, anything bigger than that, will receive an error/null value.

## Implementation

We use a single array as our memory bank. We control its size, the chunk size and the next free position through a linked-list. Since we use a list, it allows free operation on any chunk, instead of all of them at once (like an Arena) or just the last one allocated (like a Stack).

```
unsigned char mem_bank[80];
size_t cunk_size = 8;

/*
(...)
*/

void pool_init(Pool *pool, unsigned char *mem_bank, size_t bank_s, size_t chunk_size, size_t align_bytes){

    //since pool always give a chunk as a mem addr to use, we are forcing the chunks to be a multiple of wordsize
    assert(is_word_sized(chunk_size, align_bytes) && "Chunk size isn't a multiple of the wordsize given as it should be");

    pool->mem_bank = mem_bank;
    pool->bank_s = bank_s;
    pool->chunk_size = chunk_size;
    pool->number_chunks = bank_s/chunk_size;
    pool->head = NULL;

    memset(pool->mem_bank, 0, pool->bank_s);

    //create the linked-list
    pool_free_all(pool);
}
```

Since there is no flexibility on the allocation size (it is always the size of the chunk), the alloc process is simpler:
```
void* pool_alloc(Pool *pool, size_t n_bytes_alloc){

    printf("Allocating %lu bytes\n", n_bytes_alloc);
    
    if(n_bytes_alloc > pool->chunk_size){
        printf("Size asked for allocation is bigger than a chunk! Can't allocate\n\n"); 
        return NULL;
    } 
    
    
    PoolPointer *free_pos = pool->head;
    
    if(free_pos == NULL){
        printf("Pool already full!\n\n");
        return NULL;
    }

    
    pool->head = pool->head->next;

    printf("Pointing pos %p for alloctaing. New free pos is %p\n", free_pos, pool->head);

    return free_pos;
}

```

It just returns the next free position, which is the first one pointed by the head of the list.

Free operation is also simpler:

```
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

    printf("Freeing pos: %p\n\n", addr_to_free);
    PoolPointer *node = (PoolPointer *) addr_to_free;
    memset(node, 0, pool->chunk_size);

    node->next = pool->head;
    pool->head = node;
}

```

It just re-insert the chunk at the beggining of the list, that is, a recently deallocated position will be the next free position, changing the sequence but, since every allocation operation just returns an entire chunk (there is no size calculation), there is no problem on keeping the list out of order.


Considering that every chunk has a 4 bytes size. An empty pool would look like:

```
head->  pos1----------->pos2----------->pos3
        |               |               |
        V___ ___ ___ ___V___ ___ ___ ___v___ ___ ___ ___
        |___|___|___|___|___|___|___|___|___|___|___|___|

```

When a allocation occurs, we follow the list to return the free position.

Allocating two different chunks, one for str "AB" then other for "CDEF":

```
head----------------------------------->pos3
                                        |
         ___ ___ ___ ___ ___ ___ ___ ___v___ ___ ___ ___
        |_A_|_B_|___|___|_C_|_D_|_E_|_F_|___|___|___|___|

```

And if a free request occurs, the released position assumes the first position.

Releseaing pos1 and pos2 two, in this order:


```
head->  pos1--------------------------->pos3
        |                               |
        V___ ___ ___ ___ ___ ___ ___ ___v___ ___ ___ ___
        |___|___|___|___|_C_|_D_|_E_|_F_|___|___|___|___|



head->  pos2----------->pos1------------>pos3
        |    ___________|               |
        |___|___________                |
         ___|           |               |
        |               |               |
        v___ ___ ___ ___v___ ___ ___ ___v___ ___ ___ ___
        |___|___|___|___|___|___|___|___|___|___|___|___|

```

Next time an allocation is requested, the position *pos2* will be returned as the available chunk.