# Arena 

## Definition
Arena allocation uses a simple linear memory block, where every new allocation occurs at the end of it and there is no option to free single positions, i.e., 
**all blocks are released at once**.

## Implementation

We use a single array as our memory bank, where we control its size and current free position:

![image](https://user-images.githubusercontent.com/2237507/149237402-adad57bf-bc94-4a2f-83d5-37807efed281.png)

```
unsigned char mem_bank[1000];

/*
(...)
*/

void arena_init(Arena *arena, unsigned char *mem_bank, size_t bank_s, size_t align_bytes){
    arena->bank_s = bank_s;
    arena->mem_bank = mem_bank;
    arena->offset = 0;
    arena->align = align_bytes;

    memset(arena->mem_bank, 0, arena->bank_s);

}

```

Each array position represents a memory position and, when an allocation is requested, we deliver a pointer to the first free position.

![image](https://user-images.githubusercontent.com/2237507/149237564-8697ff8c-a5a7-4a64-8c0d-089c64466b5c.png)

```
void *arena_alloc(Arena *arena, size_t space){

    /*
    Process to check aligment and return the next free position accordingly 
    */
    uintptr_t free_pos = (uintptr_t)arena->mem_bank + (uintptr_t)arena->offset;
    uintptr_t best_pos = align(free_pos, arena->align);
    uintptr_t pos_idx = best_pos - (uintptr_t)arena->mem_bank;

    if(arena->offset + space <= arena->bank_s){
        void *free_pos_ptr = &arena->mem_bank[pos_idx];
        arena->offset = pos_idx + space;

        memset(free_pos_ptr, 0, space);

        printf("Pointing pos %p for allocating. New free pos is [%lu] %p\n", 
        free_pos_ptr, arena->offset, &arena->mem_bank[arena->offset]);

        return free_pos_ptr;
    }
    else{
        printf("Allocation failed: Not enough space\n\n");
        return NULL;
    }
}
```

## Sources

[ginberBill Memory Allocation Strategies - Linear Allocator](https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/)
