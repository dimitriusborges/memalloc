# Memalloc

This PoC's intent is to study different ways to safely manipulate memory with C.

Although experimental, the code presented here can be used as an alternative to the alloc/free dinamic, avoiding well known problems and giving to the developer more control over memory allocation and usage.

With these approaches, the developer:
 - Directly control how much memory the process will use, since the memory bank is abstracted to an array.
 - Fully control memory allocation and access
 - Avoid memory leak and segmentation fault 


Three strategies are presented:

- Arena
- Stack
- Pool

All the strategies use an Array as the memory bank and take some extra (and simplified) steps to guarantee that the memory aligment is according to a predefined word size, to avoid multiple word-sized memory readings by the processor.

Most of it was done using [ginberBill](https://www.gingerbill.org/series/memory-allocation-strategies/) as source material.


