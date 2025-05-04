#pragma once
#include <stddef.h>
#include <stdint.h>

#include "DataPort.h"
#include "Page.h"
#include "Screen.h"
#include "std/mem.h"


uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func,
                         uint8_t offset);

void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset,
                      uint32_t value) ;

void pci_init() ;
