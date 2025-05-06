#include "Mouse.h"


// I/O port macros
#define PS2_DATA       0x60
#define PS2_STATUS     0x64
#define PS2_COMMAND    0x64

// Controller commands
#define PS2_CMD_ENABLE_AUX_DEVICE     0xA8
#define PS2_CMD_READ_CONFIG           0x20
#define PS2_CMD_WRITE_CONFIG          0x60
#define PS2_CMD_WRITE_MOUSE           0xD4

// Mouse commands
#define MOUSE_CMD_SET_DEFAULTS        0xF6
#define MOUSE_CMD_ENABLE_REPORTING    0xF4



volatile int mouse_cycle = 0;
volatile uint8_t mouse_packet[3];

static void ps2_wait_input(void) {
    while (!(inb(PS2_STATUS) & 1));
}

static void ps2_wait_output(void) {
    while (inb(PS2_STATUS) & 2);
}

static void ps2_write_command(uint8_t cmd) {
    ps2_wait_output();
    outb(PS2_COMMAND, cmd);
}

static void ps2_write_data(uint8_t data) {
    ps2_wait_output();
    outb(PS2_DATA, data);
}

static void ps2_send_mouse(uint8_t data) {
    ps2_write_command(PS2_CMD_WRITE_MOUSE);
    ps2_write_data(data);
}

// Mouse interrupt handler for IRQ12
void mouse_handler(void) {
    uint8_t data = inb(PS2_DATA);
    mouse_packet[mouse_cycle++] = data;

    if (mouse_cycle == 3) {
        mouse_cycle = 0;

        int left   = mouse_packet[0] & 0x1;
        int right  = mouse_packet[0] & 0x2;
        int middle = mouse_packet[0] & 0x4;

        int dx = (int8_t)mouse_packet[1];
        int dy = (int8_t)mouse_packet[2];

    }

    outb(0xA0, 0x20);  // EOI to slave PIC
    outb(0x20, 0x20);  // EOI to master PIC
}

void enable_irq12(void) {
    uint8_t mask = inb(0xA1);
    mask &= ~(1 << 4);  // Clear bit 4 (IRQ12) on slave PIC
    outb(0xA1, mask);
}

void init_ps2_mouse(void) {
    ps2_write_command(PS2_CMD_ENABLE_AUX_DEVICE);

    ps2_write_command(PS2_CMD_READ_CONFIG);
    ps2_wait_input();
    uint8_t config = inb(PS2_DATA);
    config |= 0x02;  // Enable IRQ12
    ps2_write_command(PS2_CMD_WRITE_CONFIG);
    ps2_write_data(config);

    ps2_send_mouse(MOUSE_CMD_SET_DEFAULTS);
    ps2_wait_input(); inb(PS2_DATA); // ACK

    ps2_send_mouse(MOUSE_CMD_ENABLE_REPORTING);
    ps2_wait_input(); inb(PS2_DATA); // ACK
}
