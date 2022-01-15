#include "stack.h"
#include "../mem_align.h"
#include <stdio.h>
#include <string.h>

void stack_init(Stack *stack, unsigned char *mem_bank, size_t bank_s, size_t align_bytes);

void *stack_alloc(Stack *stack, size_t space);

void *stack_free(Stack *stack);

void stack_init(Stack *stack, unsigned char *mem_bank, size_t bank_s, size_t align_bytes){
    stack->bank_s = bank_s;
    stack->mem_bank = mem_bank;
    stack->curr_offset = 0;
    stack->last_offset = 0;
    stack->align = align_bytes;

    memset(stack->mem_bank, 0, stack->bank_s);

    printf("Memory bank starting on position %p\n\n\n", stack->mem_bank);

}

void *stack_alloc(Stack *stack, size_t n_bytes_alloc){
    printf("allocating %lu bytes\n", n_bytes_alloc);

    uintptr_t free_pos = (uintptr_t)stack->mem_bank + (uintptr_t)stack->curr_offset;
    uintptr_t best_pos = align(free_pos, stack->align);
    uintptr_t pos_idx = best_pos - (uintptr_t)stack->mem_bank;

    printf("Idx for alloc: %lu\n", pos_idx);

    if(stack->curr_offset + HEADER_S + n_bytes_alloc<= stack->bank_s){
        void *free_pos_ptr = &stack->mem_bank[pos_idx];

        StackHeader *header = (StackHeader *) free_pos_ptr;
        header->last_free = &stack->mem_bank[stack->last_offset];

        stack->last_offset = pos_idx;
        stack->curr_offset = pos_idx + HEADER_S + n_bytes_alloc;

        free_pos_ptr += HEADER_S;
        memset(free_pos_ptr, 0, n_bytes_alloc);
        printf("Pointing pos %p for allocating. New free pos is [%lu] %p\n", 
        free_pos_ptr, stack->curr_offset, &stack->mem_bank[stack->curr_offset]);

        return free_pos_ptr;
    }

    else{
        printf("Allocation failed: Not enough space \n\n");
        return NULL;
    }
}


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