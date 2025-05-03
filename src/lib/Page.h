#include "Screen.h"
#include <stdint.h>

void paging_init();

void page_fault_handler();

void *alloc_page();

void paging_map(void* virt, void* phys, uint32_t flags);
