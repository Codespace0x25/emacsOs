#include <stdint.h>
#include "Screen.h"
#include "std/transform.h"

#define PAGE_PRESENT   0x1
#define PAGE_RW        0x2
#define PAGE_USER      0x4

#define PAGE_TABLE_ENTRIES     1024
#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_SIZE              4096

void* alloc_page();

void paging_map(void* virt, void* phys, uint32_t flags);

void paging_init();

void page_fault_handler();
