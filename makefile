# Tools
ASM     = nasm
CC      = clang
CCXX    = clang++
LD      = ld.lld
MOUNT   = mtools
QEMU    = qemu-system-x86_64

# Build structure
SRC     = src
LIB     = $(SRC)/lib
BUILD   = build
IMG     = $(BUILD)/os.iso
MOUNTDIR = $(BUILD)/mnt
TARGET = kernel.bin

# Flags
CFLAGS  = -ffreestanding -Wall -Wextra -m32 -fno-stack-protector -O0 -target x86_64-unknown-none
CXXFLAGS = $(CFLAGS) -std=c++20 -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -m elf_i386

# Sources
C_SOURCES    := $(shell find $(SRC) -name '*.c')
CXX_SOURCES  := $(shell find $(SRC) -name '*.cpp')
ASM_SOURCES  := $(shell find $(SRC) -name '*.asm')

C_OBJS       := $(patsubst $(SRC)/%.c,$(BUILD)/%.o,$(C_SOURCES))
CXX_OBJS     := $(patsubst $(SRC)/%.cpp,$(BUILD)/%.cpp.o,$(CXX_SOURCES))
ASM_OBJS     := $(patsubst $(SRC)/%.asm,$(BUILD)/%.asm.o,$(ASM_SOURCES))
OBJS         := $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS)

.PHONY: all clean image run

all: $(IMG)

# Compile C source files
$(BUILD)/%.o: $(SRC)/%.c | makefile
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile C++ source files
$(BUILD)/%.cpp.o: $(SRC)/%.cpp | makefile
	@mkdir -p $(dir $@)
	$(CCXX) $(CXXFLAGS) -c $< -o $@

# Compile ASM source files
$(BUILD)/%.asm.o: $(SRC)/%.asm | makefile
	@mkdir -p $(dir $@)
	$(ASM) -f elf32 $< -o $@

# Link kernel
$(BUILD)/$(TARGET): $(OBJS) | makefile
	@mkdir -p $(BUILD)
	$(LD) $(LDFLAGS) -o $@ $^

# Build ISO image
$(IMG): $(BUILD)/$(TARGET) grub.cfg | makefile
	mkdir -p $(BUILD)/iso/boot/grub/
	cp grub.cfg $(BUILD)/iso/boot/grub
	cp $(BUILD)/$(TARGET) $(BUILD)/iso/boot/
	mkdir -p $(BUILD)/iso/bin/
	cp init.v4 $(BUILD)/iso/bin/
	grub-mkrescue -o $(IMG) $(BUILD)/iso --modules="video video_bochs video_cirrus gfxterm"

# Run in QEMU
run: $(IMG) makefile
	./run.sh $(QEMU) $(IMG) $(BUILD)/$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD)
