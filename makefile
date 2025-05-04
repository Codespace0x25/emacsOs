# Tools
ASM     = nasm
CC      = clang
LD      = ld.lld
MOUNT   = mtools
QEMU    = qemu-system-i386

# Build structure
SRC     = src
LIB     = $(SRC)/lib
BUILD   = build
IMG     = $(BUILD)/os.iso
MOUNTDIR = $(BUILD)/mnt
TARGET  = kernel.bin

# Flags
CFLAGS  = -ffreestanding -Wall -Wextra -m32 -fno-stack-protector -v -O0 
LDFLAGS = -T linker.ld -m elf_i386

# Sources
C_SOURCES   := $(shell find $(SRC) -name '*.c')
ASM_SOURCES := $(shell find $(SRC) -name '*.asm')

C_OBJS      := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(C_SOURCES))
ASM_OBJS    := $(patsubst $(SRC)/%.asm,$(BUILD)/%.asm.o,$(ASM_SOURCES))
OBJS        := $(C_OBJS) $(ASM_OBJS)

.PHONY: all clean image run

all: $(IMG)

# Compile C source files
$(BUILD)/%.o: $(SRC)/%.c | makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile ASM source files
$(BUILD)/%.asm.o: $(SRC)/%.asm  |makefile
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

# Link kernel
$(BUILD)/$(TARGET): $(OBJS) |makefile
	@mkdir -p $(BUILD)
	$(LD) $(LDFLAGS) -o $@ $^

# Create FAT32 disk image and install GRUB + files
$(IMG): $(BUILD)/$(TARGET) grub.cfg | makefile
	mkdir -p $(BUILD)/iso/boot/grub/
	cp grub.cfg $(BUILD)/iso/boot/grub
	cp $(BUILD)/$(TARGET) $(BUILD)/iso/boot/
	mkdir -p $(BUILD)/iso/bin/
	cp init.v4 $(BUILD)/iso/bin/
	grub-mkrescue -o $(IMG) $(BUILD)/iso
run: $(IMG)  makefile
	$(QEMU) -drive file=$(IMG),format=raw,if=ide -serial stdio -audiodev pa,id=speaker -machine pcspk-audiodev=speaker 

clean:
	rm -rf $(BUILD)
