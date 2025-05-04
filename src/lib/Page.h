#include "Screen.h"
#include <stdint.h>


#define PAGE_PRESENT   0x1
#define PAGE_RW        0x2
#define PAGE_USER      0x4

void paging_init();

void page_fault_handler();

void *alloc_page();

void paging_map(void* virt, void* phys, uint32_t flags);
