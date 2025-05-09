#include <stdint.h>
#include <stddef.h>
#include "Page.h"

extern uint32_t* page_directory; // your active page directory base

#define PAGE_PRESENT    0x1
#define PAGE_WRITE      0x2
#define PAGE_SIZEm       0x1000
#define PDE_INDEX(x)    (((x) >> 22) & 0x3FF)
#define PTE_INDEX(x)    (((x) >> 12) & 0x3FF)

extern void flush_tlb(); // you need to reload CR3 after edits

// allocate a zeroed page of memory (e.g., from a physical memory manager)
extern void* alloc_page();

void map_physical_memory(uint32_t addr) {
    paging_map((void*)addr, (void*)addr, PAGE_RW);
}
void flush_tlb() {
    asm volatile (
        "mov %%cr3, %%eax\n"
        "mov %%eax, %%cr3\n"
        ::: "eax"
    );
}
void map_physical_range(uint32_t base, uint32_t size) {
    uint32_t aligned_base = base & ~0xFFF;
    uint32_t end = base + size;

    for (uint32_t addr = aligned_base; addr < end; addr += PAGE_SIZEm) {
        paging_map((void*)addr, (void*)addr, PAGE_RW);
    }
}
