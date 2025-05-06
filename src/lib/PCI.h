#include <stdint.h>
#include "Screen.h"
#include "std/transform.h"
#include "DataPort.h"

uint32_t pci_config_read(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) ;

uint16_t pci_config_read_word(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) ;

void pci_scan_bus() ;
