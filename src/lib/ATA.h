#pragma once
#include <stdint.h>
#include <stdbool.h>

#define MAX_ATA_DEVICES 4

typedef struct {
    bool present;
    char model[41];
    uint16_t io_base;
    uint16_t ctrl_base;
    uint8_t slave; // 0 = master, 1 = slave
} ATA_Device;

void ata_initialize();
void ata_pio_read_sector(uint8_t device_index, uint32_t lba, uint8_t* buffer);
const ATA_Device* ata_get_device(uint8_t index);
uint8_t ata_get_device_count();
