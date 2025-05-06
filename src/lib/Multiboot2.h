#pragma once
#include <cstdint>
#include <stdint.h>

struct multiboot_tag{
  uint32_t type;
  uint32_t size;
};

struct multiboot_tag_framebuffer {
};
