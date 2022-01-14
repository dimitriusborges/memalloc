# Arena 

## Definition
Arena allocation uses a simple linear memory block, where every new allocation occurs at the end of it and there is no option to free single positions, i.e., 
**all blocks are released at once**.

## Implementation

We use a single array as our memory bank. We control its size and current free position:

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

Each array position, that is = 1 Byte,  represents a memory position and, when an allocation is requested, we deliver a pointer to the first free position.


```
void *arena_alloc(Arena *arena, size_t space){

    /*
    Process to check aligment, based on given WORDSIZE, and return the next free position accordingly 
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

And the free request frees the entire mem bank at once

```
void free_all(Arena *arena){
    arena->offset = 0;

    memset(arena->mem_bank, 0, arena->bank_s);
}
```

In fact, all we do is manipulate the array, using an pointer (offset) to control the next free position. When a alloc is requested, we deliver this position and set the offset + allocated_size positions.

Empty mem block:
```
 ___ ___ ___ ___ ___ ___
|___|___|___|___|___|___|
^
| 
offset=0
```

After allocing 4 bytes for str "ABCD":
```
 ___ ___ ___ ___ ___ ___
|_A_|_B_|_C_|_D_|___|___|
                ^
                | 
                offset=4
```