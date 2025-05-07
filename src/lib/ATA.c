#include <stdint.h>
#include <stddef.h>
#include "std/string.h"
#include "std/mem.h"
#include "Screen.h"
#include "ATA.h"
#include "DataPort.h"

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_ALT_STATUS 0x206
#define ATA_REG_CONTROL    0x206

#define ATA_CMD_IDENTIFY   0xEC
#define ATA_CMD_READ_PIO   0x20

#define ATA_SR_BSY 0x80
#define ATA_SR_DRQ 0x08
#define ATA_SR_ERR 0x01

static ATA_Device ata_devices[MAX_ATA_DEVICES];
static uint8_t ata_device_count = 0;

static void ata_wait(uint16_t io_base) {
    while (inb(io_base + ATA_REG_STATUS) & ATA_SR_BSY);
    while (!(inb(io_base + ATA_REG_STATUS) & ATA_SR_DRQ));
}

static void ata_delay(uint16_t ctrl_base) {
    inb(ctrl_base); inb(ctrl_base);
    inb(ctrl_base); inb(ctrl_base);
}

static bool ata_identify(uint16_t io, uint16_t ctrl, uint8_t slave, ATA_Device* out) {
    outb(io + ATA_REG_HDDEVSEL, 0xA0 | (slave << 4));
    ata_delay(ctrl);
    outb(io + ATA_REG_SECCOUNT0, 0);
    outb(io + ATA_REG_LBA0, 0);
    outb(io + ATA_REG_LBA1, 0);
    outb(io + ATA_REG_LBA2, 0);
    outb(io + ATA_REG_COMMAND, ATA_CMD_IDENTIFY);
    ata_delay(ctrl);

    uint8_t status = inb(io + ATA_REG_STATUS);
    if (status == 0) return false;

    while (inb(io + ATA_REG_STATUS) & ATA_SR_BSY);
    if (inb(io + ATA_REG_LBA1) != 0 || inb(io + ATA_REG_LBA2) != 0) return false;

    while (!(inb(io + ATA_REG_STATUS) & ATA_SR_DRQ)) {
        if (inb(io + ATA_REG_STATUS) & ATA_SR_ERR) return false;
    }

    uint16_t id_data[256];
    for (int i = 0; i < 256; i++) {
        id_data[i] = inw(io + ATA_REG_DATA);
    }

    for (int i = 0; i < 40; i += 2) {
        out->model[i]     = (char)(id_data[27 + i / 2] >> 8);
        out->model[i + 1] = (char)(id_data[27 + i / 2] & 0xFF);
    }
    out->model[40] = '\0';

    out->present = true;
    out->io_base = io;
    out->ctrl_base = ctrl;
    out->slave = slave;
    return true;
}

void ata_initialize() {
    uint16_t bases[][2] = {
        {0x1F0, 0x3F6}, // primary
        {0x170, 0x376}, // secondary
    };

    for (int ch = 0; ch < 2; ch++) {
        for (int sl = 0; sl < 2; sl++) {
            ATA_Device dev = {0};
            if (ata_identify(bases[ch][0], bases[ch][1], sl, &dev)) {
                ata_devices[ata_device_count++] = dev;
            }
        }
    }
    Screen_SetColor(WHITE, BLACK);
    putS("[");
    Screen_SetColor(GREEN, BLACK);
    putS("yes");
    Screen_SetColor(WHITE, BLACK);
    putS("] ATA initialized\n");
}

void ata_pio_read_sector(uint8_t device_index, uint32_t lba, uint8_t* buffer) {
    if (device_index >= ata_device_count) return;
    ATA_Device* dev = &ata_devices[device_index];
    uint16_t io = dev->io_base;

    outb(io + ATA_REG_HDDEVSEL, 0xE0 | (dev->slave << 4) | ((lba >> 24) & 0x0F));
    outb(io + ATA_REG_SECCOUNT0, 1);
    outb(io + ATA_REG_LBA0, (uint8_t)(lba & 0xFF));
    outb(io + ATA_REG_LBA1, (uint8_t)((lba >> 8) & 0xFF));
    outb(io + ATA_REG_LBA2, (uint8_t)((lba >> 16) & 0xFF));
    outb(io + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    ata_wait(io);

    for (int i = 0; i < 256; i++) {
        uint16_t data = inw(io + ATA_REG_DATA);
        buffer[i * 2] = data & 0xFF;
        buffer[i * 2 + 1] = data >> 8;
    }

    ata_delay(dev->ctrl_base);
}

const ATA_Device* ata_get_device(uint8_t index) {
    return (index < ata_device_count) ? &ata_devices[index] : NULL;
}

uint8_t ata_get_device_count() {
    return ata_device_count;
}
