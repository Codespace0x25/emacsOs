#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "DataPort.h"
#include "Page.h"
#include "Screen.h"
#include "std/mem.h"
#include "MemMap.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

uint32_t pci_config_read(uint8_t bus, uint8_t slot, uint8_t func,
                         uint8_t offset) {
  uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) |
                                (offset & 0xFC) | 0x80000000);
  outl(PCI_CONFIG_ADDRESS, address);
  return inl(PCI_CONFIG_DATA);
}

void pci_config_write(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset,
                      uint32_t value) {
  uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) |
                                (offset & 0xFC) | 0x80000000);
  outl(PCI_CONFIG_ADDRESS, address);
  outl(PCI_CONFIG_DATA, value);
}

bool pci_device_exists(uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t id = pci_config_read(bus, slot, func, 0x00);
    return (id & 0xFFFF) != 0xFFFF;
}

void pci_check_function(uint8_t bus, uint8_t slot, uint8_t func) {
    uint32_t id = pci_config_read(bus, slot, func, 0x00);
    if ((id & 0xFFFF) == 0xFFFF) return;

    uint16_t vendor_id = id & 0xFFFF;
    uint16_t device_id = (id >> 16) & 0xFFFF;

    uint32_t class_reg = pci_config_read(bus, slot, func, 0x08);
    uint8_t class_code = (class_reg >> 24) & 0xFF;
    uint8_t subclass   = (class_reg >> 16) & 0xFF;

    printf("PCI: %02x:%02x.%x - Vendor: %04x Device: %04x Class: %02x Sub: %02x\n",
           bus, slot, func, vendor_id, device_id, class_code, subclass);

    // Enable memory and I/O
    uint32_t cmd = pci_config_read(bus, slot, func, 0x04);
    cmd |= (1 << 0) | (1 << 1);
    pci_config_write(bus, slot, func, 0x04, cmd);

    // Check BARs
    for (uint8_t bar_num = 0; bar_num < 6; bar_num++) {
        uint8_t offset = 0x10 + bar_num * 4;
        uint32_t bar = pci_config_read(bus, slot, func, offset);
        if (bar == 0 || bar == 0xFFFFFFFF) continue;

        if (bar & 0x1) {
            uint16_t io_base = bar & ~0x3;
            printf("  BAR%d I/O at 0x%04x\n", bar_num, io_base);
        } else {
            uint32_t mem_base = bar & ~0xF;
            printf("  BAR%d MEM at 0x%08x\n", bar_num, mem_base);
	    map_physical_memory(mem_base); // identity-map device BAR
        }
    }
}

void pci_check_slot(uint8_t bus, uint8_t slot) {
    if (!pci_device_exists(bus, slot, 0)) return;

    pci_check_function(bus, slot, 0);

    uint32_t header_type = pci_config_read(bus, slot, 0, 0x0C);
    if ((header_type >> 16) & 0x80) {  // bit 7 set = multi-function device
        for (uint8_t func = 1; func < 8; func++) {
            if (pci_device_exists(bus, slot, func))
                pci_check_function(bus, slot, func);
        }
    }
}

void pci_init() {
    for (uint8_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            pci_check_slot(bus, slot);
        }
    }
}
