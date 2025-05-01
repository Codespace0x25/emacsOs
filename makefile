# Tools
ASM     = nasm
CC      = clang++
LD      = ld
GRUB    = grub-mkrescue
QEMU    = qemu-system-x86_64

# Directories
SRC     = src
BUILD   = build
ISO     = $(BUILD)/iso
BOOT    = $(ISO)/boot
TARGET  = $(BOOT)/kernel.bin

# Flags
CXXFLAGS = -std=c++17 -ffreestanding -Wall -Wextra -m32 -I$(SRC)
LDFLAGS  = -T linker.ld -m elf_i386

# Sources
CPP_SOURCES = $(wildcard $(SRC)/**/*.cpp $(SRC)/*.cpp)
ASM_SOURCES = $(wildcard $(SRC)/**/*.asm $(SRC)/*.asm)

OBJS = $(patsubst $(SRC)/%.cpp,$(BUILD)/%.o,$(CPP_SOURCES)) \
       $(patsubst $(SRC)/%.asm,$(BUILD)/%.o,$(ASM_SOURCES))

# Default target
.PHONY: all
all: iso

# Compile C++ sources
$(BUILD)/%.o: $(SRC)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CXXFLAGS) -c $< -o $@

# Compile Assembly sources
$(BUILD)/%.o: $(SRC)/%.asm
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

# Link kernel
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

# ISO with GRUB
iso: $(TARGET)
	mkdir -p $(BOOT)/grub
	cp grub.cfg $(BOOT)/grub/
	grub-mkrescue -o $(BUILD)/os.iso $(ISO)

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD)

# Run in QEMU
.PHONY: run
run: iso
	$(QEMU) -cdrom $(BUILD)/os.iso
