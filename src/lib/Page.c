#include <stdint.h>
#include "Screen.h"
#include "std/transform.h"

#define PAGE_PRESENT   0x1
#define PAGE_RW        0x2
#define PAGE_USER      0x4

#define PAGE_TABLE_ENTRIES 1024
#define PAGE_DIRECTORY_ENTRIES 1024

#define PAGE_SIZE 4096

static uint32_t next_free_page = 0x100000; // Start after kernel
uint32_t page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
uint32_t first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

void paging_init() {
    // Identity-map first 4MB
    for (uint32_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_RW;
    }

    // Point first directory entry to page table
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    // Clear the rest of the directory
    for (uint32_t i = 1; i < PAGE_DIRECTORY_ENTRIES; i++) {
        page_directory[i] = 0;
    }

    // Load page directory into CR3
    __asm__ volatile("mov %0, %%cr3" :: "r"(page_directory));

    // Enable paging in CR0
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set PG bit
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));
    
    Screen_SetColor(WHITE,BLACK);
    putS("[");
    Screen_SetColor(GREEN,BLACK);
    putS("yes");
    Screen_SetColor(WHITE,BLACK);
    putS("] ");
    putS("init Paging\n");
}

void page_fault_handler() {
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

    putS("Page fault at: 0x");
    char* buf;
    int_to_string(faulting_address, buf);
    putS(buf);
    putS("\n");
    
    while (1); // Halt the system
}

void* alloc_page() {
    void* addr = (void*)next_free_page;
    next_free_page += PAGE_SIZE;
    return addr;
}
