#ifndef _MEM_ALIGN_H
#define _MEM_ALIGN_H

#include <stdint.h>
#include <stddef.h>

uintptr_t align(uintptr_t curr_pos, size_t align_bytes_s);

#endif