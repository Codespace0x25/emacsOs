#pragma once
#include "DataPort.h"
#include <stdint.h>
#include <stddef.h>

// Public initialization function
void init_ps2_mouse(void);

// Should be installed to IRQ12 (interrupt vector 0x2C)
void mouse_handler(void);

// Unmasks IRQ12 (mouse IRQ)
void enable_irq12(void);
