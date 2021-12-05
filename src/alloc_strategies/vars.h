#include <stdio.h>
#include<inttypes.h>

typedef struct sevenB{
    char data1;
    char data2[5];
    int8_t data3;
}SevenBytes;

typedef struct sixteenB{
    int64_t data1;
    int64_t data2;
}SixteenBytes;