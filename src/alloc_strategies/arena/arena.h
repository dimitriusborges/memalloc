#ifndef _ARENA_H 
#define _ARENA_H
#include <stdint.h>
#include <stddef.h>

typedef struct ArenaType{
 unsigned char *mem_bank;
 size_t offset;
 size_t bank_s;
 size_t align;
 
}Arena;


void arena_init(Arena *arena, unsigned char *mem_bank, size_t bank_s, size_t align_bytes);

void *arena_alloc(Arena *arena, size_t n_bytes_alloc);

void free(Arena *arena);

void free_all(Arena *arena);

#endif