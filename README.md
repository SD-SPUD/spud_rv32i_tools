# RISC-V RV32I Tools & SpudKit

This repository contains tools, SpudKit library, and demo projects for creating ELF files to run on the [RISC-V SoC](../vivado_proj/spud_riscv_soc) FPGA project.

## Project Structure

```
spud_rv32i-tools/
├── spudkit/                # SpudKit development library
│   ├── include/            # Header files
│   │   ├── spudkit.h       # Main library header
│   │   ├── display.h       # 64x64 display engine
│   │   ├── uart.h          # UART communication
│   │   ├── gpio.h          # GPIO control
│   │   ├── timer.h         # Timer functions
│   │   ├── spi.h           # SPI interface
│   │   ├── irq.h           # Interrupt handling
│   │   └── utils.h         # Utility functions
│   ├── src/                # Library source files
│   ├── build-support/      # Shared build files
│   │   ├── rv32i.ld        # Centralized linker script
│   │   └── start.s         # Centralized startup code
│   └── build/              # Library build artifacts (generated)
├── demos/                  # Demo projects
│   ├── hello_world/        # Basic UART demo
│   ├── gpio_test/          # GPIO testing demo
│   └── display_demo/       # Visual display demo with animations
│       ├── src/            # Source code
│       │   └── main.c      # Demo program
│       └── build/          # Build artifacts (generated)
├── Makefile               # Enhanced build system
└── README.md             # This file
```

## RISC-V Toolchain Setup

### Prerequisites

You need the RISC-V GNU toolchain installed to build programs for RV32I. This includes:
- `riscv32-unknown-elf-gcc` (C compiler)
- `riscv32-unknown-elf-as` (Assembler)
- `riscv32-unknown-elf-ld` (Linker)
- `riscv32-unknown-elf-objcopy` (Object copy utility)
- `riscv32-unknown-elf-objdump` (Disassembler)

### Installation

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install gcc-riscv64-unknown-elf
```

```
**Windows:**
- Download pre-built toolchain from [SiFive](https://www.sifive.com/software)
- Or use WSL with Ubuntu instructions

### Verify Installation

Check if the toolchain is properly installed:
```bash
make check-toolchain
```

You should see output like:
```
Checking RISC-V toolchain...
Toolchain found: riscv32-unknown-elf-gcc (GCC) 12.2.0
```

## SpudKit Library

SpudKit is a comprehensive development library for the RISC-V SoC that provides:

- **Display Engine**: 64x64 pixel framebuffer with drawing primitives (lines, rectangles, circles, text)
- **Dual Display Modes**:
  - `SIM_DISPLAY=1` - Terminal simulation mode for development
  - `UART_DISPLAY=1` - Hardware UART output with ANSI colors
- **Peripheral Drivers**: UART, GPIO, Timer, SPI, IRQ controller access
- **Utility Functions**: Random number generation, string operations, math functions
- **Color Support**: RGB565 color format with ANSI terminal color mapping

### Display Features

The display engine supports:
- 64x64 pixel framebuffer
- RGB565 color format
- Drawing primitives: pixels, lines, rectangles, circles
- Text rendering capabilities
- Terminal visualization using Unicode half-height blocks
- Real-time animation support

## Usage

### Build Options

```bash
# Basic build
make hello_world

# Build with simulation display (terminal output)
SIM_DISPLAY=1 make display_demo

# Build with UART display (hardware ANSI colors)
UART_DISPLAY=1 make display_demo
```

### Build a Specific Demo

```bash
make display_demo
```

This will:
1. Compile SpudKit library files
2. Assemble shared startup code
3. Compile demo C source files
4. Link everything with SpudKit library
5. Generate ELF file in `demos/display_demo/build/`
6. Create binary and disassembly files

### Build All Demos

```bash
make all
```

### List Available Demos

```bash
make list
```

### Clean Build Artifacts

```bash
make clean
```

### Run Demo on Hardware

```bash
make run display_demo
```

This will automatically build the demo (if needed) and run it on the testbench.

### Get Help

```bash
make help
```

## Target Hardware Specifications

The generated ELF files are designed for the RISC-V SoC with these specifications:

- **Architecture**: RV32I (32-bit RISC-V Integer instruction set)
- **Memory Map**:
  - Main Memory: `0x8000_0000 - 0x8fff_ffff` (256MB)
  - UART: `0x9200_0000 - 0x92ff_ffff`
  - Timer: `0x9100_0000 - 0x91ff_ffff` 
  - IRQ Controller: `0x9000_0000 - 0x90ff_ffff`
  - SPI: `0x9300_0000 - 0x93ff_ffff`
  - GPIO: `0x9400_0000 - 0x94ff_ffff`

## Creating New Demos

To create a new demo project:

1. Create a new directory under `demos/`:
```bash
mkdir demos/my_demo
mkdir demos/my_demo/src
```

2. Add your source files to `demos/my_demo/src/`:
   - `main.c` - Your main C program
   - `start.s` - Assembly startup (copy from hello_world)
   - `rv32i.ld` - Linker script (copy from hello_world)

3. Build your demo:
```bash
make my_demo
```

## Demo Descriptions

### hello_world
A simple "Hello World" program that outputs text via UART. Demonstrates:
- Basic UART communication
- Bare-metal C programming for RISC-V
- Memory-mapped peripheral access
- SpudKit library integration

### gpio_test
GPIO testing and control demo. Demonstrates:
- GPIO pin control and reading
- Hardware peripheral interaction
- Real-time input/output operations

### display_demo
Interactive visual display with animated graphics. Demonstrates:
- SpudKit display engine usage
- RGB565 color graphics
- Drawing primitives (lines, rectangles, circles)
- Real-time animation with random graphics
- Terminal visualization or UART color output
- Random number generation for dynamic content

## Hardware Integration

The generated ELF files are designed to work with the RISC-V SoC FPGA project spud.

### FPGA Integration
To load and run programs on the FPGA:
1. Build your demo: `make display_demo`
2. Use the ELF file from `demos/display_demo/build/display_demo.elf` with the SoC's debug bridge
3. The program will output to UART at 1Mbaud

### UART Console Access
For UART display mode, connect to the hardware using:

**Using minicom with color support:**
```bash
minicom -c -D /dev/ttyUSB1 -b 1000000
```

**Using screen:**
```bash
screen /dev/ttyUSB1 1000000
```

**Python console script:**
```bash
cd ../spud_riscv_soc/fpga/arty
python run.py -f ../../spud_rv32i-tools/demos/display_demo/build/display_demo.elf
```

### Display Modes

- **Development Mode**: Use `SIM_DISPLAY=1` to see visual output in your terminal during development
- **Hardware Mode**: Use `UART_DISPLAY=1` for colorized ANSI output over UART when running on hardware
- **Hardware Mode (no display)**: Build normally for hardware-only GPIO/peripheral testing

## Troubleshooting

**Error: "riscv32-unknown-elf-gcc not found"**
- Install the RISC-V toolchain (see setup instructions above)
- Make sure the toolchain is in your PATH

**Linking errors:**
- Check that all required source files exist
- Verify the linker script matches your target memory layout
- Ensure SpudKit library files are being compiled correctly

**Program doesn't run on hardware:**
- Verify the memory map matches your SoC configuration
- Check that the reset vector is correctly set
- Ensure UART baud rate matches hardware configuration (1Mbaud)

**UART colors not displaying:**
- Use `minicom -c` to enable color support
- Try `screen` instead of minicom for better ANSI support
- Ensure terminal supports 256-color ANSI sequences
- Check serial port permissions and connection

**Display not updating:**
- Verify `display_update()` is being called
- Check that framebuffer operations are working correctly
- For UART mode, ensure proper line endings (`\r\n`)

**Serial port issues:**
- Check device permissions: `ls -l /dev/ttyUSB*`
- Kill stuck processes: `sudo fuser -k /dev/ttyUSB1`
- Verify baud rate matches hardware: 1000000
