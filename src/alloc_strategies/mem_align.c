#include "mem_align.h"
#include<stdio.h>

/*
Modern computer architectures will always read memory at its “word size” 
(4 bytes on a 32 bit machine, 8 bytes on a 64 bit machine). If you have an 
unaligned memory access (on a processor that allows for that), the processor 
will have to read multiple “words”. This means that an unaligned memory 
access may be much slower than an aligned memory access[...]

source: https://www.gingerbill.org/article/2019/02/08/memory-allocation-strategies-002/
*/
uintptr_t align(uintptr_t curr_pos, size_t align_bytes_s){
    size_t word_s = align_bytes_s;
    uintptr_t align = curr_pos;
    uintptr_t diff = curr_pos%word_s;

    if (diff != 0){
        
        align += word_s - diff;
        printf("Current position isn't wordsize. Moving %lu positions to align\n", word_s - diff);
    }

    printf("Best position is:%lx \n", align);
    return align;
    
}