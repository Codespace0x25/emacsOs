#include <stdint.h>
#include "PC_Speaker.h"
#include "Screen.h"
#include "std/transform.h"
#include "DataPort.h"

// PCI configuration I/O ports
#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA    0xCFC

// Macros to construct the configuration address
#define PCI_ENABLE_BIT     0x80000000

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t address =
        PCI_ENABLE_BIT |
        ((uint32_t)bus << 16) |
        ((uint32_t)device << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC);  // Ensure it's 4-byte aligned

    outl(PCI_CONFIG_ADDRESS, address);
    uint32_t data = inl(PCI_CONFIG_DATA);
    return data;
}

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    uint32_t data = pci_config_read(bus, device, function, offset);
    return (data >> ((offset & 2) * 8)) & 0xFFFF;
}

void pci_scan_bus() {
    char hex[8];
    for (uint16_t bus = 0; bus < 256; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            uint8_t function_count = 1;

            uint16_t vendor_check = pci_config_read_word(bus, device, 0, 0x00);
            if (vendor_check == 0xFFFF) continue;

            uint8_t header_type = (pci_config_read(bus, device, 0, 0x0C) >> 16) & 0xFF;
            if (header_type & 0x80) {
                function_count = 8;
            }

            for (uint8_t function = 0; function < function_count; function++) {
                uint16_t vendor_id = pci_config_read_word(bus, device, function, 0x00);
                if (vendor_id == 0xFFFF) continue;

                uint16_t device_id = pci_config_read_word(bus, device, function, 0x02);

                putS("PCI Device Found - Bus: ");
                int_to_string(bus, hex); putS(hex);
                putS(", Dev: ");
                int_to_string(device, hex); putS(hex);
                putS(", Func: ");
                int_to_string(function, hex); putS(hex);
                putS(" - Vendor ID: 0x");
                int_to_hex(vendor_id, hex); putS(hex);
                putS(", Device ID: 0x");
                int_to_hex(device_id, hex); putS(hex);
                putS("\n");
            }
        }
    }
}
