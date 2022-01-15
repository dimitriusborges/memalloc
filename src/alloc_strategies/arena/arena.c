#include "arena.h"
#include "../mem_align.h"
#include <stdio.h>
#include <string.h>

void arena_init(Arena *arena, unsigned char *mem_bank, size_t bank_s, size_t align_bytes);

void free(Arena *arena);

void free_all(Arena *arena);

void arena_init(Arena *arena, unsigned char *mem_bank, size_t bank_s, size_t align_bytes){
    arena->bank_s = bank_s;
    arena->mem_bank = mem_bank;
    arena->offset = 0;
    arena->align = align_bytes;

    memset(arena->mem_bank, 0, arena->bank_s);

    printf("Memory bank starting on position %p\n\n\n", arena->mem_bank);
}

void *arena_alloc(Arena *arena, size_t n_bytes_alloc){

    printf("allocating %lu bytes\n", n_bytes_alloc);

    uintptr_t free_pos = (uintptr_t)arena->mem_bank + (uintptr_t)arena->offset;
    uintptr_t best_pos = align(free_pos, arena->align);
    uintptr_t pos_idx = best_pos - (uintptr_t)arena->mem_bank;

    printf("Idx for alloc: %lu\n", pos_idx);

    if(arena->offset + n_bytes_alloc <= arena->bank_s){
        void *free_pos_ptr = &arena->mem_bank[pos_idx];
        arena->offset = pos_idx + n_bytes_alloc;

        memset(free_pos_ptr, 0, n_bytes_alloc);

        printf("Pointing pos %p for allocating. New free pos is [%lu] %p\n", 
        free_pos_ptr, arena->offset, &arena->mem_bank[arena->offset]);

        return free_pos_ptr;
    }
    else{
        printf("Allocation failed: Not enough space\n\n");
        return NULL;
    }
}

void free(Arena *arena){
    printf("Arena can't deallocate single positions." 
    "Only full deallocation is supported. Check \"free_all\"");
}

void free_all(Arena *arena){
    arena->offset = 0;
    //memset is just to highlight that the positions are free to be allocated.
    //Reseting the offset should be enough, although you must remember that
    //the data on a free_flaged positions isn't valid anymore
    memset(arena->mem_bank, 0, arena->bank_s);
}
