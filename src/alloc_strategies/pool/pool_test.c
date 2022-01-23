#include <stdio.h>
#include "pool.h"
#include "../vars.h"

// gcc pool_test.c pool.c ../mem_align.c -o pool
void main(){

    Pool pool;
    size_t mem_bank_s = 80;
    unsigned char mem_bank[mem_bank_s];

    pool_init(&pool, mem_bank, mem_bank_s, 8, __WORDSIZE/8);

    fourBytes *var1 = pool_alloc(&pool, sizeof(fourBytes));
    var1->data[0] = 'A';
    var1->data[1] = 'B';
    var1->data[2] = 'C';
    var1->data[3] = 'D';
    printf("\n\n");     

    SevenBytes *var2 = pool_alloc(&pool, sizeof(SevenBytes));
    var2->data1 = 'b';
    var2->data2[0] = 'E';
    var2->data2[1] = 'F';
    var2->data2[2] = 'G';
    var2->data2[3] = 'H';
    var2->data2[4] = 'I';
    var2->data3 = 8;
    printf("\n\n");
    
    fourBytes *var3 = pool_alloc(&pool, sizeof(fourBytes));
    var3->data[0] = 'J';
    var3->data[1] = 'K';
    var3->data[2] = 'L';
    var3->data[3] = 'M';
    printf("\n\n");     

    SixteenBytes *noVar = pool_alloc(&pool, sizeof(SixteenBytes));

    pool_free(&pool, var2);


    print_pool_addrs(pool.head);

    SevenBytes *var4 = pool_alloc(&pool, sizeof(SevenBytes));
    var2->data1 = 'b';
    var2->data2[0] = 'E';
    var2->data2[1] = 'F';
    var2->data2[2] = 'G';
    var2->data2[3] = 'H';
    var2->data2[4] = 'I';
    var2->data3 = 8;
    printf("\n\n");



}