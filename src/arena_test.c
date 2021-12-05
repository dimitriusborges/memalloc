#include <stdio.h>
#include <string.h>
#include "alloc_strategies/arena/arena.h"
#include "alloc_strategies/vars.h"

 // gcc arena_test.c alloc_strategies/arena/*.c alloc_strategies/*.c -o output/arena


void main(){

    Arena arena;
    size_t mem_bank_s = 50;
    unsigned char mem_bank[mem_bank_s];

    arena_init(&arena, mem_bank, mem_bank_s, __WORDSIZE/8);

    SixteenBytes *var1 = arena_alloc(&arena, sizeof(SixteenBytes));
    var1->data1 = 128;
    var1->data2 = 256;
    printf("\n\n");

    
    SevenBytes *var2 = arena_alloc(&arena, sizeof(SevenBytes));
    var2->data1 = 'a';
    memset(var2->data2, 0x41, 5);
    var2->data3 = 1;
    printf("\n\n");

    SevenBytes *var3 = arena_alloc(&arena, sizeof(SevenBytes));
    var3->data1 = 'b';
    memset(var3->data2, 0x42, 5);
    var3->data3 = 2;
    printf("\n\n");

    printf("Data1 [%ld] pos: %p\nData2 [%ld] pos: %p\n\n",
        var1->data1, &var1->data1,
        var1->data2, &var1->data2);

    printf("Data1 [%c] pos: %p\nData2 [%s] pos: %p\nData3[%d] pos: %p\n\n", 
        var2->data1, &var2->data1,
        var2->data2, &var2->data2,
        var2->data3, &var2->data3);
    
    printf("Data1 [%c] pos: %p\nData2 [%s] pos: %p\nData3[%d] pos: %p\n\n", 
        var3->data1, &var3->data1,
        var3->data2, &var3->data2,
        var3->data3, &var3->data3);  
}