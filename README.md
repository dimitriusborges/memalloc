# Memalloc

This PoC's intent is to study different ways to safely manipulate memory with C.

Although experimental, the code presented here can be used as an alternative to alloc/free dinamic, avoiding well known problems and giving to the developer more control over memory allocation and usage.

Most of it was done using [ginberBill](https://www.gingerbill.org/series/memory-allocation-strategies/) as source material.

Three strategies are presented:

- Arena
- Stack
- Poll
