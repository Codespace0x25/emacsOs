#pragma once
#include <stdint.h>
#include <stddef.h>



void map_physical_memory(uint32_t addr);
void map_physical_range(uint32_t base, uint32_t size);

void flush_tlb();
