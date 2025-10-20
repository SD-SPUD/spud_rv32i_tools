# RISC-V RV32I Toolchain Configuration
RISCV_PREFIX = riscv32-unknown-elf-
CC = $(RISCV_PREFIX)gcc
AS = $(RISCV_PREFIX)as
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump

# Compiler flags for RV32IM
CFLAGS = -march=rv32im_zicsr -mabi=ilp32 -O2 -Wall -Wextra -nostdlib -nostartfiles -fno-builtin -Ispudkit/include -funroll-all-loops

ASFLAGS = -march=rv32im_zicsr -mabi=ilp32
LDFLAGS = -march=rv32im_zicsr -mabi=ilp32 -nostdlib -lgcc

# Optional simulation display flag - use with SIM_DISPLAY=1
ifdef SIM_DISPLAY
CFLAGS += -DSIM_DISPLAY
endif

# Optional UART display flag - use with UART_DISPLAY=1
ifdef UART_DISPLAY
CFLAGS += -DUART_DISPLAY
endif

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
	@echo "  make run <demo_name>      - Run demo on testbench"
	@echo ""
	@echo "Options:"
	@echo "  SIM_DISPLAY=1      - Enable simulation display mode"
	@echo "  UART_DISPLAY=1     - Enable UART display mode"
	@echo ""
	@echo "Available demos:"
	@for demo in $(DEMOS); do echo "  $$demo"; done
	@echo ""
	@echo "Examples:"
	@echo "  make hello_world               - Build hello_world demo"
	@echo "  SIM_DISPLAY=1 make hello_world  - Build with simulation display"
	@echo "  UART_DISPLAY=1 make hello_world - Build with UART display"
	@echo "  make run hello_world           - Run hello_world on testbench"

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

	# Compile spudkit library files first
	@echo "Building spudkit library..."
	@mkdir -p spudkit/build
	@for c_file in spudkit/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			echo "Compiling $$c_file..."; \
			$(CC) $(CFLAGS) -c $$c_file -o spudkit/build/$$(basename $$c_file .c).o; \
		fi \
	done

	# Assemble shared startup code
	@echo "Assembling shared startup code..."
	@$(AS) $(ASFLAGS) spudkit/build-support/start.s -o spudkit/build/start.o

	# Compile demo C files
	@for c_file in demos/$@/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			echo "Compiling $$c_file..."; \
			$(CC) $(CFLAGS) -c $$c_file -o demos/$@/build/$$(basename $$c_file .c).o; \
		fi \
	done

	# Assemble demo-specific assembly files (if any exist)
	@for s_file in demos/$@/src/*.s; do \
		if [ -f "$$s_file" ]; then \
			echo "Assembling $$s_file..."; \
			$(AS) $(ASFLAGS) $$s_file -o demos/$@/build/$$(basename $$s_file .s).o; \
		fi \
	done

	# Link all object files (including spudkit) using shared linker script
	@echo "Linking $@..."
	@$(CC) $(LDFLAGS) -T spudkit/build-support/rv32i.ld demos/$@/build/*.o spudkit/build/*.o -o demos/$@/build/$@.elf

	# Generate additional outputs
	@$(OBJDUMP) -D demos/$@/build/$@.elf > demos/$@/build/$@.dis
	@$(OBJCOPY) -O binary demos/$@/build/$@.elf demos/$@/build/$@.bin

	@echo "Demo '$@' built successfully!"
	@echo "ELF file: demos/$@/build/$@.elf"
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
		rm -rf demos/$$demo/build; \
	done
	@rm -rf spudkit/build
	@echo "Clean complete!"

# Run a demo on the testbench
.PHONY: run
run:
	@DEMO=$(word 2,$(MAKECMDGOALS)); \
	if [ -z "$$DEMO" ]; then \
		echo "Usage: make run <demo_name>"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -d "demos/$$DEMO" ]; then \
		echo "Error: Demo '$$DEMO' not found!"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -f "demos/$$DEMO/build/$$DEMO.elf" ]; then \
		echo "Demo '$$DEMO' not built. Building now..."; \
		$(MAKE) $$DEMO; \
	fi; \
	echo "Running $$DEMO on testbench..."; \
	cd ../spud_riscv_soc/tb && ./build/test.x -f ../../spud_rv32i-tools/demos/$$DEMO/build/$$DEMO.elf -b 0x80000000

# Catch-all rule to ignore demo names when used as arguments to "run"
%:
	@if [ "$(firstword $(MAKECMDGOALS))" = "run" ]; then \
		true; \
	else \
		echo "Error: Unknown target '$@'"; \
		echo "Run 'make help' to see available targets"; \
		exit 1; \
	fi

# Check if toolchain is installed
.PHONY: check-toolchain
check-toolchain:
	@echo "Checking RISC-V toolchain..."
	@which $(CC) > /dev/null || (echo "Error: $(CC) not found! Please install RISC-V toolchain."; exit 1)
	@echo "Toolchain found: $$($(CC) --version | head -n1)"
