# RISC-V RV32I Toolchain Configuration
RISCV_PREFIX = riscv64-unknown-elf-
CC = $(RISCV_PREFIX)gcc
AS = $(RISCV_PREFIX)as
LD = $(RISCV_PREFIX)ld
OBJCOPY = $(RISCV_PREFIX)objcopy
OBJDUMP = $(RISCV_PREFIX)objdump
SIZE = $(RISCV_PREFIX)size
NM = $(RISCV_PREFIX)nm

# Compiler flags for RV32IM
CFLAGS = -march=rv32im_zicsr -mabi=ilp32 -O2 -Wall -Wextra -nostdlib -nostartfiles -fno-builtin -Ispudkit/include -funroll-all-loops
ASFLAGS = -march=rv32im_zicsr -mabi=ilp32
LDFLAGS = -march=rv32im_zicsr -mabi=ilp32 -nostdlib -lgcc

# Optional stack usage analysis flags
ifdef STACK_USAGE
CFLAGS += -fstack-usage
endif

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

# Launcher games list
LAUNCHER_GAMES = snake tetris chess spudman donut

# Help message
.PHONY: help
help:
	@echo "RISC-V RV32I Demo Builder"
	@echo "========================="
	@echo ""
	@echo "Usage:"
	@echo "  make <demo_name>              - Build specific demo"
	@echo "  make all                      - Build all demos"
	@echo "  make clean                    - Clean all build artifacts"
	@echo "  make list                     - List available demos"
	@echo "  make run <demo_name>          - Run demo on testbench"
	@echo "  make memory <demo_name>       - Show memory usage analysis"
	@echo "  make memory-detailed <demo>   - Show detailed function-level analysis"
	@echo "  make stack-analysis <demo>    - Analyze stack usage (compile-time)"
	@echo ""
	@echo "Options:"
	@echo "  SIM_DISPLAY=1      - Enable simulation display mode"
	@echo "  UART_DISPLAY=1     - Enable UART display mode"
	@echo ""
	@echo "Available demos:"
	@for demo in $(DEMOS); do echo "  $$demo"; done
	@echo ""
	@echo "Examples:"
	@echo "  make hello_world                - Build hello_world demo"
	@echo "  SIM_DISPLAY=1 make hello_world  - Build with simulation display"
	@echo "  UART_DISPLAY=1 make hello_world - Build with UART display"
	@echo "  make run hello_world            - Run hello_world on testbench"
	@echo "  make memory hello_world         - Analyze memory usage"

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
	@echo "Generating disassembly..."
	@$(OBJDUMP) -D demos/$@/build/$@.elf > demos/$@/build/$@.dis 2>&1 || \
		(echo "Warning: Full disassembly failed, generating code-only disassembly..." && \
		$(OBJDUMP) -d demos/$@/build/$@.elf > demos/$@/build/$@.dis 2>&1 || \
		echo "Warning: Disassembly generation failed, skipping...")
	@echo "Generating binary..."
	@$(OBJCOPY) -O binary demos/$@/build/$@.elf demos/$@/build/$@.bin

	# Generate memory usage report
	@echo "Generating memory usage report..."
	@$(MAKE) --no-print-directory memory-report DEMO=$@

	@echo ""
	@echo "Demo '$@' built successfully!"
	@echo "ELF file: demos/$@/build/$@.elf"
	@echo "Binary:   demos/$@/build/$@.bin"
	@echo "Disasm:   demos/$@/build/$@.dis"
	@echo "Memory:   demos/$@/build/$@.mem"

# Internal target for generating memory reports
.PHONY: memory-report
memory-report:
	@if [ -z "$(DEMO)" ]; then \
		echo "Error: DEMO variable not set"; \
		exit 1; \
	fi
	@ELF_FILE="demos/$(DEMO)/build/$(DEMO).elf"; \
	MEM_FILE="demos/$(DEMO)/build/$(DEMO).mem"; \
	if [ ! -f "$$ELF_FILE" ]; then \
		echo "Error: ELF file not found: $$ELF_FILE"; \
		exit 1; \
	fi; \
	\
	echo "================================================================" > $$MEM_FILE; \
	echo "Memory Usage Report for: $(DEMO)" >> $$MEM_FILE; \
	echo "Generated: $$(date)" >> $$MEM_FILE; \
	echo "================================================================" >> $$MEM_FILE; \
	echo "" >> $$MEM_FILE; \
	\
	TEXT_SIZE=$$($(SIZE) -A -d $$ELF_FILE | grep '\.text' | awk '{sum += $$2} END {print sum}'); \
	RODATA_SIZE=$$($(SIZE) -A -d $$ELF_FILE | grep '\.rodata' | awk '{sum += $$2} END {print sum}'); \
	DATA_SIZE=$$($(SIZE) -A -d $$ELF_FILE | grep '\.data' | awk '{sum += $$2} END {print sum}'); \
	BSS_SIZE=$$($(SIZE) -A -d $$ELF_FILE | grep '\.bss' | awk '{sum += $$2} END {print sum}'); \
	\
	TEXT_SIZE=$${TEXT_SIZE:-0}; \
	RODATA_SIZE=$${RODATA_SIZE:-0}; \
	DATA_SIZE=$${DATA_SIZE:-0}; \
	BSS_SIZE=$${BSS_SIZE:-0}; \
	\
	TOTAL_SIZE=$$((TEXT_SIZE + RODATA_SIZE + DATA_SIZE + BSS_SIZE)); \
	\
	echo "Program Size:" >> $$MEM_FILE; \
	echo "----------------------------------------------------------------" >> $$MEM_FILE; \
	printf "  .text (code):               %10d bytes (%6d KB)\n" $$TEXT_SIZE $$((TEXT_SIZE / 1024)) >> $$MEM_FILE; \
	printf "  .rodata (constants):        %10d bytes (%6d KB)\n" $$RODATA_SIZE $$((RODATA_SIZE / 1024)) >> $$MEM_FILE; \
	printf "  .data (initialized data):   %10d bytes (%6d KB)\n" $$DATA_SIZE $$((DATA_SIZE / 1024)) >> $$MEM_FILE; \
	printf "  .bss (uninitialized data):  %10d bytes (%6d KB)\n" $$BSS_SIZE $$((BSS_SIZE / 1024)) >> $$MEM_FILE; \
	echo "  ----------------------------------------" >> $$MEM_FILE; \
	printf "  Total program size:         %10d bytes (%6d KB)\n" $$TOTAL_SIZE $$((TOTAL_SIZE / 1024)) >> $$MEM_FILE; \
	echo "" >> $$MEM_FILE; \
	\
	echo "Detailed Breakdown by Object File:" >> $$MEM_FILE; \
	echo "================================================================" >> $$MEM_FILE; \
	echo "" >> $$MEM_FILE; \
	echo "SpudKit Library:" >> $$MEM_FILE; \
	echo "----------------------------------------------------------------" >> $$MEM_FILE; \
	SPUDKIT_TOTAL=0; \
	for obj in spudkit/build/*.o; do \
		if [ -f "$$obj" ] && [ "$$(basename $$obj)" != "start.o" ]; then \
			OBJ_SIZE=$$($(SIZE) -A -d "$$obj" 2>/dev/null | grep -E '^\.(text|rodata|data|bss)' | awk '{sum += $$2} END {print sum+0}'); \
			if [ ! -z "$$OBJ_SIZE" ] && [ $$OBJ_SIZE -gt 0 ]; then \
				printf "  %-20s %10d bytes (%6d KB)\n" "$$(basename $$obj)" $$OBJ_SIZE $$((OBJ_SIZE / 1024)) >> $$MEM_FILE; \
				SPUDKIT_TOTAL=$$((SPUDKIT_TOTAL + OBJ_SIZE)); \
			fi; \
		fi; \
	done; \
	echo "  ----------------------------------------" >> $$MEM_FILE; \
	printf "  %-20s %10d bytes (%6d KB)\n" "SpudKit Total" $$SPUDKIT_TOTAL $$((SPUDKIT_TOTAL / 1024)) >> $$MEM_FILE; \
	echo "" >> $$MEM_FILE; \
	\
	echo "Startup Code:" >> $$MEM_FILE; \
	echo "----------------------------------------------------------------" >> $$MEM_FILE; \
	if [ -f "spudkit/build/start.o" ]; then \
		START_SIZE=$$($(SIZE) -A -d "spudkit/build/start.o" 2>/dev/null | grep -E '^\.(text|rodata|data|bss)' | awk '{sum += $$2} END {print sum+0}'); \
		printf "  %-20s %10d bytes (%6d KB)\n" "start.o" $$START_SIZE $$((START_SIZE / 1024)) >> $$MEM_FILE; \
	fi; \
	echo "" >> $$MEM_FILE; \
	\
	echo "Application Code ($(DEMO)):" >> $$MEM_FILE; \
	echo "----------------------------------------------------------------" >> $$MEM_FILE; \
	APP_TOTAL=0; \
	for obj in demos/$(DEMO)/build/*.o; do \
		if [ -f "$$obj" ]; then \
			OBJ_SIZE=$$($(SIZE) -A -d "$$obj" 2>/dev/null | grep -E '^\.(text|rodata|data|bss)' | awk '{sum += $$2} END {print sum+0}'); \
			if [ ! -z "$$OBJ_SIZE" ] && [ $$OBJ_SIZE -gt 0 ]; then \
				printf "  %-20s %10d bytes (%6d KB)\n" "$$(basename $$obj)" $$OBJ_SIZE $$((OBJ_SIZE / 1024)) >> $$MEM_FILE; \
				APP_TOTAL=$$((APP_TOTAL + OBJ_SIZE)); \
			fi; \
		fi; \
	done; \
	echo "  ----------------------------------------" >> $$MEM_FILE; \
	printf "  %-20s %10d bytes (%6d KB)\n" "Application Total" $$APP_TOTAL $$((APP_TOTAL / 1024)) >> $$MEM_FILE; \
	echo "" >> $$MEM_FILE; \
	\
	echo "================================================================" >> $$MEM_FILE; \
	\
	cat $$MEM_FILE; \
	echo ""

# Analyze memory usage for a specific demo
.PHONY: memory
memory:
	@DEMO=$(word 2,$(MAKECMDGOALS)); \
	if [ -z "$$DEMO" ]; then \
		echo "Usage: make memory <demo_name>"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -d "demos/$$DEMO" ]; then \
		echo "Error: Demo '$$DEMO' not found!"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -f "demos/$$DEMO/build/$$DEMO.elf" ]; then \
		echo "Error: Demo '$$DEMO' not built yet. Building now..."; \
		$(MAKE) $$DEMO; \
	else \
		$(MAKE) --no-print-directory memory-report DEMO=$$DEMO; \
	fi

# Detailed function-level memory analysis
.PHONY: memory-detailed
memory-detailed:
	@DEMO=$(word 2,$(MAKECMDGOALS)); \
	if [ -z "$$DEMO" ]; then \
		echo "Usage: make memory-detailed <demo_name>"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -d "demos/$$DEMO" ]; then \
		echo "Error: Demo '$$DEMO' not found!"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -f "demos/$$DEMO/build/$$DEMO.elf" ]; then \
		echo "Error: Demo '$$DEMO' not built yet. Please build first with: make $$DEMO"; \
		exit 1; \
	fi; \
	\
	echo "================================================================"; \
	echo "Detailed Function-Level Memory Analysis for: $$DEMO"; \
	echo "================================================================"; \
	echo ""; \
	echo "Top 20 Largest Functions (by size):"; \
	echo "----------------------------------------------------------------"; \
	$(NM) --print-size --size-sort --radix=d demos/$$DEMO/build/$$DEMO.elf 2>/dev/null | \
		grep -E ' [TtWw] ' | tail -20 | tac | \
		awk '{printf "  %-40s %6d bytes\n", $$4, $$2}'; \
	echo ""; \
	echo "Top 20 Largest Data/BSS Symbols:"; \
	echo "----------------------------------------------------------------"; \
	$(NM) --print-size --size-sort --radix=d demos/$$DEMO/build/$$DEMO.elf 2>/dev/null | \
		grep -E ' [BbDdGgSs] ' | tail -20 | tac | \
		awk '{printf "  %-40s %6d bytes\n", $$4, $$2}'; \
	echo ""; \
	echo "All Functions Sorted by Size:"; \
	echo "----------------------------------------------------------------"; \
	$(NM) --print-size --size-sort --radix=d demos/$$DEMO/build/$$DEMO.elf 2>/dev/null | \
		grep -E ' [TtWw] ' | \
		awk '{total+=$$2; printf "  %-40s %6d bytes\n", $$4, $$2} END {printf "\n  Total function size: %d bytes (%d KB)\n", total, total/1024}'; \
	echo ""; \
	echo "================================================================"

# Stack usage analysis
.PHONY: stack-analysis
stack-analysis:
	@DEMO=$(word 2,$(MAKECMDGOALS)); \
	if [ -z "$$DEMO" ]; then \
		echo "Usage: make stack-analysis <demo_name>"; \
		echo "This will rebuild the demo with stack usage analysis enabled."; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	if [ ! -d "demos/$$DEMO" ]; then \
		echo "Error: Demo '$$DEMO' not found!"; \
		echo "Available demos: $(DEMOS)"; \
		exit 1; \
	fi; \
	\
	echo "================================================================"; \
	echo "Stack Usage Analysis for: $$DEMO"; \
	echo "================================================================"; \
	echo ""; \
	echo "Rebuilding with stack usage tracking enabled..."; \
	$(MAKE) clean > /dev/null 2>&1; \
	STACK_USAGE=1 $(MAKE) $$DEMO > /dev/null 2>&1; \
	\
	if [ ! -f "demos/$$DEMO/build/$$DEMO.elf" ]; then \
		echo "Error: Build failed!"; \
		exit 1; \
	fi; \
	\
	echo ""; \
	echo "Top 20 Functions by Stack Usage:"; \
	echo "----------------------------------------------------------------"; \
	find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null | \
		xargs cat 2>/dev/null | \
		awk -F'\t' '{split($$1, a, ":"); printf "%06d\t%s\t%s\n", $$2, a[length(a)], $$3}' | \
		sort -n -r | head -20 | \
		awk -F'\t' '{printf "  %-40s %6d bytes (%s)\n", $$2, $$1+0, $$3}'; \
	\
	echo ""; \
	echo "Total Stack Usage Estimate:"; \
	echo "----------------------------------------------------------------"; \
	TOTAL_STACK=$$(find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null | \
		xargs cat 2>/dev/null | \
		awk -F'\t' '{sum += $$2} END {print sum+0}'); \
	MAX_FUNC=$$(find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null | \
		xargs cat 2>/dev/null | \
		awk -F'\t' '{if($$2 > max) max=$$2} END {print max+0}'); \
	printf "  Sum of all functions:       ~%d bytes (~%d KB)\n" $$TOTAL_STACK $$((TOTAL_STACK / 1024)); \
	printf "  Largest single function:    ~%d bytes\n" $$MAX_FUNC; \
	echo ""; \
	echo "  Note: Actual runtime stack usage depends on the deepest call chain,"; \
	echo "  not the sum of all functions. The allocated stack is 1 MB (1048576 bytes)."; \
	echo ""; \
	\
	DYNAMIC_COUNT=$$(find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null -exec cat {} \; | \
		grep -c "dynamic" 2>/dev/null || true); \
	DYNAMIC_COUNT=$${DYNAMIC_COUNT:-0}; \
	BOUNDED_COUNT=$$(find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null -exec cat {} \; | \
		grep -c "bounded" 2>/dev/null || true); \
	BOUNDED_COUNT=$${BOUNDED_COUNT:-0}; \
	\
	if [ "$$DYNAMIC_COUNT" -gt 0 ] 2>/dev/null; then \
		echo "  ⚠️  Warning: $$DYNAMIC_COUNT function(s) use dynamic stack allocation!"; \
		echo "  Dynamic allocation makes precise stack analysis impossible."; \
		echo ""; \
		find spudkit/build demos/$$DEMO/build -name "*.su" 2>/dev/null | \
			xargs grep "dynamic" 2>/dev/null | \
			awk -F':' '{print "    - " $$1}' | sed 's|spudkit/src/||g; s|demos/'$$DEMO'/src/||g'; \
		echo ""; \
	fi; \
	\
	if [ "$$BOUNDED_COUNT" -gt 0 ] 2>/dev/null; then \
		echo "  ℹ️  Info: $$BOUNDED_COUNT function(s) use bounded (variable) stack allocation."; \
		echo ""; \
	fi; \
	\
	echo "================================================================"; \
	echo ""; \
	echo "Stack usage files (.su) available in:"; \
	echo "  - spudkit/build/*.su"; \
	echo "  - demos/$$DEMO/build/*.su"

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

# Special launcher target that combines multiple games
.PHONY: launcher
launcher:
	@echo "Building launcher with games: $(LAUNCHER_GAMES)"
	@mkdir -p demos/launcher/build

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

	# Compile launcher files
	@echo "Compiling launcher..."
	@for c_file in demos/launcher/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			echo "Compiling $$c_file..."; \
			$(CC) $(CFLAGS) -Idemos/launcher/src -c $$c_file -o demos/launcher/build/$$(basename $$c_file .c).o; \
		fi \
	done

	# Compile each game as a single relocatable object (to avoid symbol conflicts)
	@echo "Compiling snake game..."
	@mkdir -p demos/launcher/build/snake_objs
	@for c_file in demos/snake/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			$(CC) $(CFLAGS) -Idemos/snake/src -c $$c_file -o demos/launcher/build/snake_objs/$$(basename $$c_file .c).o; \
		fi \
	done
	@$(LD) -m elf32lriscv -r demos/launcher/build/snake_objs/*.o -o demos/launcher/build/snake.o

	@echo "Compiling tetris game..."
	@mkdir -p demos/launcher/build/tetris_objs
	@for c_file in demos/tetris/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			$(CC) $(CFLAGS) -Idemos/tetris/src -c $$c_file -o demos/launcher/build/tetris_objs/$$(basename $$c_file .c).o; \
		fi \
	done
	@$(LD) -m elf32lriscv -r demos/launcher/build/tetris_objs/*.o -o demos/launcher/build/tetris.o

	@echo "Compiling chess game..."
	@mkdir -p demos/launcher/build/chess_objs
	@for c_file in demos/chess/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			$(CC) $(CFLAGS) -Idemos/chess/src -c $$c_file -o demos/launcher/build/chess_objs/$$(basename $$c_file .c).o; \
		fi \
	done
	@$(LD) -m elf32lriscv -r demos/launcher/build/chess_objs/*.o -o demos/launcher/build/chess.o

	@echo "Compiling spudman game..."
	@mkdir -p demos/launcher/build/spudman_objs
	@for c_file in demos/spudman/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			$(CC) $(CFLAGS) -Idemos/spudman/src -c $$c_file -o demos/launcher/build/spudman_objs/$$(basename $$c_file .c).o; \
		fi \
	done
	@$(LD) -m elf32lriscv -r demos/launcher/build/spudman_objs/*.o -o demos/launcher/build/spudman.o

	@echo "Compiling donut game..."
	@mkdir -p demos/launcher/build/sample_objs
	@for c_file in demos/sample/src/*.c; do \
		if [ -f "$$c_file" ]; then \
			$(CC) $(CFLAGS) -Idemos/sample/src -c $$c_file -o demos/launcher/build/donut_objs/$$(basename $$c_file .c).o; \
		fi \
	done
	@$(LD) -m elf32lriscv -r demos/launcher/build/donut_objs/*.o -o demos/launcher/build/donut.o

	# Link everything together
	@echo "Linking launcher..."
	@$(CC) $(LDFLAGS) -Wl,--allow-multiple-definition -T spudkit/build-support/rv32i.ld \
		demos/launcher/build/main.o \
		demos/launcher/build/menu.o \
		demos/launcher/build/snake.o \
		demos/launcher/build/tetris.o \
		demos/launcher/build/chess.o \
		demos/launcher/build/spudman.o \
		demos/launcher/build/donut.o \
		spudkit/build/*.o \
		-o demos/launcher/build/launcher.elf

	# Generate additional outputs
	@echo "Generating disassembly..."
	@$(OBJDUMP) -D demos/launcher/build/launcher.elf > demos/launcher/build/launcher.dis 2>&1 || \
		(echo "Warning: Full disassembly failed, generating code-only disassembly..." && \
		$(OBJDUMP) -d demos/launcher/build/launcher.elf > demos/launcher/build/launcher.dis 2>&1 || \
		echo "Warning: Disassembly generation failed, skipping...")
	@echo "Generating binary..."
	@$(OBJCOPY) -O binary demos/launcher/build/launcher.elf demos/launcher/build/launcher.bin

	@echo ""
	@echo "Launcher built successfully!"
	@echo "ELF file: demos/launcher/build/launcher.elf"
	@echo "Binary:   demos/launcher/build/launcher.bin"
	@echo "Disasm:   demos/launcher/build/launcher.dis"

# Catch-all rule to ignore demo names when used as arguments to "run", "memory", etc.
%:
	@if [ "$(firstword $(MAKECMDGOALS))" = "run" ] || \
	    [ "$(firstword $(MAKECMDGOALS))" = "memory" ] || \
	    [ "$(firstword $(MAKECMDGOALS))" = "memory-detailed" ] || \
	    [ "$(firstword $(MAKECMDGOALS))" = "stack-analysis" ]; then \
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
