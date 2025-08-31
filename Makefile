# RISC-V RV32I Toolchain Configuration
RISCV_PREFIX = riscv64-unknown-elf-
CC = $(RISCV_PREFIX)gcc
AS = $(RISCV_PREFIX)as
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump

# Compiler flags for RV32I
CFLAGS = -march=rv32i_zicsr -mabi=ilp32 -O2 -Wall -Wextra -nostdlib -nostartfiles
ASFLAGS = -march=rv32i_zicsr -mabi=ilp32
LDFLAGS = -march=rv32i_zicsr -mabi=ilp32 -nostdlib

# Default demo to build if none specified
DEFAULT_DEMO = hello_world

# Available demos (auto-detected from demos/ directory)
DEMOS = $(notdir $(wildcard demos/*))

# Help message
.PHONY: help
help:
	@echo "RISC-V RV32I Demo Builder"
	@echo "========================="
	@echo ""
	@echo "Usage:"
	@echo "  make <demo_name>    - Build specific demo"
	@echo "  make all           - Build all demos"
	@echo "  make clean         - Clean all build artifacts"
	@echo "  make list          - List available demos"
	@echo ""
	@echo "Available demos:"
	@for demo in $(DEMOS); do echo "  $$demo"; done
	@echo ""
	@echo "Example:"
	@echo "  make hello_world   - Build the hello_world demo"

# List available demos
.PHONY: list
list:
	@echo "Available demos:"
	@for demo in $(DEMOS); do echo "  $$demo"; done

# Default target
.DEFAULT_GOAL := help

# Build a specific demo
$(DEMOS):
	@echo "Building demo: $@"
	@if [ ! -d "demos/$@" ]; then \
		echo "Error: Demo '$@' not found!"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi
	@mkdir -p demos/$@/build
	
	# Compile C files
	@for c_file in demos/$@/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			echo "Compiling $$c_file..."; \
			$(CC) $(CFLAGS) -c $$c_file -o demos/$@/build/$$(basename $$c_file .c).o; \
		fi \
	done
	
	# Assemble assembly files
	@for s_file in demos/$@/src/*.s; do \
		if [ -f "$$s_file" ]; then \
			echo "Assembling $$s_file..."; \
			$(AS) $(ASFLAGS) $$s_file -o demos/$@/build/$$(basename $$s_file .s).o; \
		fi \
	done
	
	# Link all object files
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -T demos/$@/src/rv32i.ld demos/$@/build/*.o -o demos/$@/build/$@.elf
	
	# Copy ELF to elf directory
	@mkdir -p demos/$@/elf
	@cp demos/$@/build/$@.elf demos/$@/elf/
	
	# Generate additional outputs
	@$(OBJDUMP) -D demos/$@/build/$@.elf > demos/$@/build/$@.dis
	@$(OBJCOPY) -O binary demos/$@/build/$@.elf demos/$@/build/$@.bin
	
	@echo "Demo '$@' built successfully!"
	@echo "ELF file: demos/$@/elf/$@.elf"
	@echo "Binary:   demos/$@/build/$@.bin"
	@echo "Disasm:   demos/$@/build/$@.dis"

# Build all demos
.PHONY: all
all: $(DEMOS)

# Clean all build artifacts
.PHONY: clean
clean:
	@echo "Cleaning build artifacts..."
	@for demo in $(DEMOS); do \
		rm -rf demos/$$demo/build demos/$$demo/elf; \
	done
	@echo "Clean complete!"

# Check if toolchain is installed
.PHONY: check-toolchain
check-toolchain:
	@echo "Checking RISC-V toolchain..."
	@which $(CC) > /dev/null || (echo "Error: $(CC) not found! Please install RISC-V toolchain."; exit 1)
	@echo "Toolchain found: $$($(CC) --version | head -n1)"