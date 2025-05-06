#include <stdint.h>
#include "Screen.h"
#include "std/transform.h"

#define PAGE_PRESENT   0x1
#define PAGE_RW        0x2
#define PAGE_USER      0x4

#define PAGE_TABLE_ENTRIES     1024
#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_SIZE              4096

// Start allocating pages after the kernel
static uint32_t next_free_page = 0x100000;

uint32_t page_directory[PAGE_DIRECTORY_ENTRIES] __attribute__((aligned(4096)));
uint32_t first_page_table[PAGE_TABLE_ENTRIES] __attribute__((aligned(4096)));

void* alloc_page() {
    // Align the address to PAGE_SIZE
    next_free_page = (next_free_page + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    void* addr = (void*)next_free_page;
    next_free_page += PAGE_SIZE;
    return addr;
}

void paging_map(void* virt, void* phys, uint32_t flags) {
    uint32_t vaddr = (uint32_t)virt;
    uint32_t paddr = (uint32_t)phys;
    uint32_t pd_index = vaddr >> 22;
    uint32_t pt_index = (vaddr >> 12) & 0x03FF;

    uint32_t* pt;
    if (page_directory[pd_index] & PAGE_PRESENT) {
        pt = (uint32_t*)(page_directory[pd_index] & ~0xFFF);
    } else {
        pt = (uint32_t*)alloc_page();
        if (!pt) {
            putS("ERROR: Failed to allocate new page table.\n");
            return;
        }
        for (int i = 0; i < PAGE_TABLE_ENTRIES; i++) {
            pt[i] = 0;
        }
        page_directory[pd_index] = ((uint32_t)pt) | flags | PAGE_PRESENT;
    }

    pt[pt_index] = (paddr & ~0xFFF) | flags | PAGE_PRESENT;

    // Invalidate TLB for this page
    __asm__ volatile("invlpg (%0)" :: "r"(vaddr) : "memory");
}

void paging_init() {
    // Identity-map the first 4MB using first page table
    for (uint32_t i = 0; i < PAGE_TABLE_ENTRIES; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_RW;
    }

    // Point the first entry of the page directory to the first page table
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_RW;

    // Clear the rest of the page directory
    for (uint32_t i = 1; i < PAGE_DIRECTORY_ENTRIES; i++) {
        page_directory[i] = 0;
    }

    // Load page directory into CR3
    __asm__ volatile("mov %0, %%cr3" :: "r"(page_directory));

    // Enable paging (set PG bit in CR0)
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" :: "r"(cr0));

    Screen_SetColor(WHITE, BLACK);
    putS("[");
    Screen_SetColor(GREEN, BLACK);
    putS("yes");
    Screen_SetColor(WHITE, BLACK);
    putS("] Paging initialized\n");
}

void page_fault_handler() {
    uint32_t faulting_address;
    __asm__ volatile("mov %%cr2, %0" : "=r"(faulting_address));

    char buf[16];  // Enough to hold 32-bit hex
    int_to_hex(faulting_address, buf); // Use hex for clarity

    putS("Page fault at address: 0x");
    putS(buf);
    putS("\nSystem Halted.\n");

    while (1); // Halt the system
}
