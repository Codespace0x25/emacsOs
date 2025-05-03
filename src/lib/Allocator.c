#include "Allocator.h"
#include "Screen.h"
#include <stddef.h>

static Block* free_list = NULL;  // Pointer to the free list

// Initialize the allocator with a heap start and size
void allocator_init(void* heap_start, uint32_t heap_size) {
    free_list = (Block*)heap_start;

    // Initialize the first free block with the full heap size
    free_list->size = heap_size - sizeof(Block); // Subtract metadata size
    free_list->next = NULL;
    Screen_SetColor(WHITE,BLACK);
    putS("[");
    Screen_SetColor(GREEN,BLACK);
    putS("yes");
    Screen_SetColor(WHITE,BLACK);
    putS("] ");
    putS("init allocater\n");
}

// Simple memory allocation using the first-fit strategy
void* malloc(uint32_t size) {
    Block* current = free_list;
    Block* previous = NULL;

    // Align size to a multiple of 4 bytes for word alignment
    size = (size + 3) & ~3;  // Round up to the nearest multiple of 4 bytes

    while (current != NULL) {
        if (current->size >= size) {
            // Found a suitable block

            if (current->size > size + sizeof(Block)) {
                // Split the block if it’s larger than the requested size
                Block* new_block = (Block*)((uint32_t)current + sizeof(Block) + size);
                new_block->size = current->size - size - sizeof(Block);
                new_block->next = current->next;

                current->size = size;
                current->next = new_block;
            }

            // Remove the block from the free list
            if (previous != NULL) {
                previous->next = current->next;
            } else {
                free_list = current->next;
            }

            return (void*)((uint32_t)current + sizeof(Block));  // Return a pointer to the memory (skip the Block metadata)
        }

        previous = current;
        current = current->next;
    }

    return NULL;  // No suitable block found
}

// Calloc: Allocate memory and initialize to zero
void* calloc(uint32_t num, uint32_t size) {
    uint32_t total_size = num * size;
    void* ptr = malloc(total_size);
    if (ptr != NULL) {
        // Zero out the allocated memory
        for (uint32_t i = 0; i < total_size; i++) {
            ((char*)ptr)[i] = 0;
        }
    }
    return ptr;
}

// Realloc: Resize a previously allocated block
void* realloc(void* ptr, uint32_t size) {
    if (ptr == NULL) {
        return malloc(size);
    }

    Block* block = (Block*)((uint32_t)ptr - sizeof(Block));
    void* new_ptr = malloc(size);

    if (new_ptr != NULL) {
        // Copy data from the old block to the new block
        uint32_t copy_size = (block->size < size) ? block->size : size;
        for (uint32_t i = 0; i < copy_size; i++) {
            ((char*)new_ptr)[i] = ((char*)ptr)[i];
        }

        // Free the old block
        free(ptr);
    }

    return new_ptr;
}

// Free: Free a previously allocated block
void free(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    Block* block = (Block*)((uint32_t)ptr - sizeof(Block));

    // Add the block back to the free list
    block->next = free_list;
    free_list = block;
}
