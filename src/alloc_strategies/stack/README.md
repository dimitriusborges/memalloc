# Stack

## Definition
Stack allocation uses a simple linear memory block as a stack, where every new allocation occurs at the end of it and **only the last position can be freed** 


## Implementation

We use a single array as our memory bank. We control its size, current free position and the last position used, so we can go back to it when a free request is done:

```
unsigned char mem_bank[1000];

/*
(...)
*/

void stack_init(Stack *stack, unsigned char *mem_bank, size_t bank_s, size_t align_bytes){
    stack->bank_s = bank_s;
    stack->mem_bank = mem_bank;
    stack->curr_offset = 0;
    stack->last_offset = 0;
    stack->align = align_bytes;

    memset(stack->mem_bank, 0, stack->bank_s);

}

```

Each array position, that is = 1 Byte,  represents a memory position and, when an allocation is requested, we deliver a pointer to the first free position.


```
void *stack_alloc(Stack *stack, size_t space){

    /*
    Process to check aligment, based on given WORDSIZE, and return the next free position accordingly 
    */

    uintptr_t free_pos = (uintptr_t)stack->mem_bank + (uintptr_t)stack->curr_offset;
    uintptr_t best_pos = align(free_pos, stack->align);
    uintptr_t pos_idx = best_pos - (uintptr_t)stack->mem_bank;

    if(stack->curr_offset + HEADER_S + space<= stack->bank_s){
        void *free_pos_ptr = &stack->mem_bank[pos_idx];

        StackHeader *header = (StackHeader *) free_pos_ptr;
        header->last_free = &stack->mem_bank[stack->last_offset];

        stack->last_offset = pos_idx;
        stack->curr_offset = pos_idx + HEADER_S + space;

        free_pos_ptr += HEADER_S;
        memset(free_pos_ptr, 0, space);
        printf("Pointing pos %p for allocating. New free pos is [%lu] %p\n", 
        free_pos_ptr, stack->curr_offset, &stack->mem_bank[stack->curr_offset]);

        return free_pos_ptr;
    }

    else{
        printf("Allocation failed: Not enough space \n\n");
        return NULL;
    }
}
```

Since the offset is always pointing to the current free position, a free request makes it point to the last used, i.e., now it is considered free:

```
void *stack_free(Stack *stack){

    uintptr_t last_free = (uintptr_t) &stack->mem_bank[stack->last_offset];

    uintptr_t last_idx = last_free - (uintptr_t) stack->mem_bank;

    printf("Last idx used was: %lu\n", last_idx);

    stack->curr_offset = last_idx;

    StackHeader *header = (StackHeader *) last_free;
    uintptr_t new_last_pos = (uintptr_t) header->last_free - (uintptr_t) stack->mem_bank;
    printf("New last idx is: %lu\n\n", new_last_pos);
    
    stack->last_offset = new_last_pos;


    return NULL;
}
```

The Stack allocation logic is pretty much the same as the Stack structure, we manipulate an array using two control vars: current_offset, pointing to the current free position and last_offset, ponting to the begining of the last block allocated: 

Empty mem block:
```
 ___ ___ ___ ___ ___ ___
|___|___|___|___|___|___|
^
| 
curr_offset=0
last_offset=0
```

Since we always have to control both current free position and last used position, we use a header, allocated along the true data, the address used before this allocation. So, after allocing 2 bytes for str "AB":
```
 ________ ___ ___ ___ ___ ___ ___
|_h->0x0_|_A_|_B_|___|___|___|___|
^              ^
|              | 
last_offset=0  curr_offset=3    
```

We are, in fact, using 3 bytes, 2 for the data and 1 for the header


And after allocing 4 more bytes for 2 str: "CD" and "EF":

```
 ________ ___ ___ ________ ___ ___ ________ ___ ___ ___ ___
|_h->0x0_|_A_|_B_|_h->0x0_|_C_|_D_|_h->0x3_|_E_|_F_|___|___|
                                  ^                ^
                                  |                | 
                                  last_offset=6    curr_offset=9    
```

We need the header because last_offset only points to the imediate last used position, but the header points to the one **before**, so we can keep track of every position used.

So, when a free request occurs:

```
 ________ ___ ___ ________ ___ ___ ___ ___ ___ ___ ___
|_h->0x0_|_A_|_B_|_h->0x0_|_C_|_D_|___|___|___|___|___|
                 ^                ^
                 |                | 
                 last_offset=3    curr_offset=6    
```

The curr_offset point to the value on last_offset and the last_offset checks the new last position, that is saved on the header, and points to it.