#ifndef _STACK_H
#define _STACK_H
#include <stdint.h>
#include <stddef.h>


typedef struct StackHeaderType{
    void *last_free;
} StackHeader;

#define HEADER_S sizeof(StackHeader)

typedef struct OffsetType{
    size_t pos;
    struct OffsetType *ant_offset;
} StackOffset;

typedef struct StackType{
    size_t bank_s;
    unsigned char *mem_bank;
    uintptr_t curr_offset;
    uintptr_t last_offset;
    size_t align;
} Stack;


void stack_init(Stack *stack, unsigned char *mem_bank, size_t bank_s, size_t align_bytes);

void *stack_alloc(Stack *stack, size_t space);

void *stack_free(Stack *stack);

void stack_free_all();


#endif 