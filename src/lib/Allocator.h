#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <stdint.h>

// Memory block metadata
typedef struct Block {
    uint32_t size;            // Size of the block
    struct Block* next;       // Pointer to the next free block
} Block;

// Functions
void allocator_init(void* heap_start, uint32_t heap_size);
void* malloc(uint32_t size);
void* calloc(uint32_t num, uint32_t size);
void* realloc(void* ptr, uint32_t size);
void free(void* ptr);

#endif
