# RISC-V RV32I Tools & SpudKit

Comprehensive development toolkit for the SPUD RISC-V SoC. This repository contains the SpudKit library, build system, and a collection of demo applications ranging from simple hardware tests to fully-featured games and graphics demos.

## What's Included

- **SpudKit Library** - Hardware abstraction layer with peripheral drivers, display engine, and utilities
- **Build System** - Makefile-based build system with automatic dependency tracking
- **20+ Demo Applications** - From simple hardware tests to fully-featured games
- **Toolchain Support** - Works with standard RISC-V GNU toolchain

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

SpudKit is a comprehensive hardware abstraction library for bare-metal RISC-V development on the SPUD SoC.

### Core Features

**Display Engine** (`display.h`)
- 64x64 pixel framebuffer with RGB565 colors
- Drawing primitives: pixels, lines, rectangles, circles, filled shapes
- Text rendering support
- Dual display modes:
  - `SIM_DISPLAY=1` - Terminal visualization for development/debugging
  - `UART_DISPLAY=1` - ANSI color output for hardware UART display
- Hardware display controller support at `0x96000000`

**Peripheral Drivers**
- `uart.h` - Serial communication (1Mbaud default)
- `gpio.h` - GPIO control with interrupt support
- `timer.h` - 32-bit hardware timers with interrupts
- `spi.h` - SPI master interface
- `irq.h` - Interrupt controller management
- `led.h` - LED matrix controller

**Math Libraries**
- `ffloat.h` - Fixed-point arithmetic for efficient math without FPU
- `ffvect3.h` - 3D vector operations (rotation, translation, projection)

**Utilities** (`utils.h`)
- String operations (strlen, strcpy, strcmp, itoa, etc.)
- Random number generation
- Memory operations
- Common helper functions

### Using SpudKit

Include the main header in your program:
```c
#include "spudkit.h"

int main() {
    spudkit_init();  // Initialize all peripherals
    uart_puts("Hello from SPUD!\r\n");
    display_clear(0);
    display_draw_rect(10, 10, 40, 40, RGB565(255, 0, 0));
    display_update();
    return 0;
}
```

## Building Demos

### Quick Start

```bash
# Build a specific demo
make hello_world

# List all available demos
make list

# Build all demos
make all

# Clean build artifacts
make clean

# Get help
make help
```

### Build Options

Enable display modes with environment variables or flags:

```bash
# Build with simulation display (terminal output for debugging)
SIM_DISPLAY=1 make donut

# Build with UART display (ANSI color output for hardware)
UART_DISPLAY=1 make snake

# Build with both modes
SIM_DISPLAY=1 UART_DISPLAY=1 make tetris
```

**Tip:** Use `SIM_DISPLAY=1` during development to see visual output in your terminal during Verilator simulation.

### Build Process

When you build a demo, the Makefile:
1. Compiles all SpudKit library source files
2. Assembles the startup code (`start.s`)
3. Compiles demo-specific C source files
4. Links everything with the RV32I linker script
5. Generates output files in `demos/<name>/build/`:
   - `<name>.elf` - Executable for loading to hardware/simulation
   - `<name>.bin` - Raw binary image
   - `<name>.dis` - Disassembly listing for debugging

### Using the Makefile Directly

If you're in this directory, you can use the Makefile directly. Alternatively, use the convenience scripts from the parent `spud_env` directory:
- `../build.sh <demo>` - Simplified build interface
- `../run.sh <demo>` - Build and run on FPGA
- `../sim.sh <demo>` - Build and run in Verilator

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

1. **Create demo directory structure:**
   ```bash
   mkdir -p demos/my_demo/src
   ```

2. **Create your main program** (`demos/my_demo/src/main.c`):
   ```c
   #include "spudkit.h"

   int main() {
       spudkit_init();
       uart_puts("My Demo Starting!\r\n");

       // Your code here
       display_clear(RGB565(0, 0, 0));
       display_draw_text(10, 28, "Hello!", RGB565(255, 255, 255));
       display_update();

       while(1) {
           // Main loop
       }
       return 0;
   }
   ```

3. **Build your demo:**
   ```bash
   make my_demo
   ```

The build system automatically uses the shared startup code and linker script from `spudkit/build-support/`, so you don't need to copy those files.

4. **Test your demo:**
   ```bash
   # In simulation
   ../sim.sh my_demo

   # On hardware
   ../run.sh my_demo
   ```

## Available Demos

### Basic Demos
- **hello_world** - Simple UART "Hello World" (great starting point)
- **gpio_test** - GPIO peripheral testing and control
- **display_demo** - Graphics primitives demo with animations
- **arcade_test** - Arcade controller button testing
- **arcade_display** - Arcade display hardware test
- **ffloat_test** - Fixed-point math library testing
- **led_test_*** - LED controller tests at various speeds (500k, 1M, 2M)
- **sample** - Template demo for creating new projects

### Graphics & Visual Demos
- **donut** - 3D spinning donut using fixed-point 3D math
- **matrix** - Matrix rain animation effect
- **langton** - Langton's ant cellular automaton simulation

### Games
- **snake** - Classic Snake game with arcade controls
- **tetris** - Full Tetris implementation
- **chess** - Chess game with AI opponent
- **spudman** - Pac-Man style arcade game
- **spudracer** - Racing game
- **launcher** - Game launcher/menu system for selecting demos

Each demo includes source code demonstrating specific SpudKit features and can serve as a reference for your own projects.

## Running on Hardware

### Using the Convenience Scripts (Recommended)

From the parent `spud_env` directory:

```bash
# Build and run on FPGA
./run.sh hello_world

# Load to FPGA without console
./load.sh snake
screen /dev/ttyUSB1 1000000  # Connect manually

# Run in Verilator simulation
./sim.sh donut donut_waves

# Just build
./build.sh tetris --uart-display
```

### Direct Hardware Access

If you prefer to work with the ELF files directly:

1. Build your demo: `make display_demo`
2. Load the ELF: `demos/display_demo/build/display_demo.elf`
3. Use the Python scripts in `../spud_riscv_soc/fpga/arty/run/`

### UART Console

For programs with UART output, connect using:

```bash
# With color support (recommended for display demos)
minicom -c -D /dev/ttyUSB1 -b 1000000

# Alternative with screen
screen /dev/ttyUSB1 1000000
```

### Display Mode Selection

Choose the appropriate display mode for your use case:

| Mode | Build Flag | Use Case |
|------|-----------|----------|
| No display | (default) | Hardware-only GPIO/peripheral testing |
| SIM_DISPLAY | `SIM_DISPLAY=1` | Terminal visualization during Verilator simulation |
| UART_DISPLAY | `UART_DISPLAY=1` | ANSI color output over UART on hardware |
| Both | Both flags | Works in both simulation and hardware |

## Troubleshooting

**Toolchain not found**
```bash
make check-toolchain  # Verify installation
which riscv32-unknown-elf-gcc  # Check PATH
```

**Build errors**
- Ensure all source files exist in `demos/<name>/src/`
- Check that SpudKit library is compiling correctly
- Try `make clean && make <demo>` to rebuild from scratch

**Serial port access denied**
```bash
sudo usermod -a -G dialout $USER  # Add to dialout group
# Log out and back in for changes to take effect
```

**UART colors not showing**
- Use `minicom -c` or `screen` for ANSI color support
- Build with `UART_DISPLAY=1` flag
- Verify terminal supports 256-color ANSI

**Display not updating**
- Call `display_update()` after drawing operations
- Check that correct display mode is enabled (SIM_DISPLAY or UART_DISPLAY)

**Program doesn't run on hardware**
- Verify FPGA bitstream is loaded
- Check serial device: try `/dev/ttyUSB0` instead of `/dev/ttyUSB1`
- Ensure baud rate matches (1000000 default)
- Try resetting the FPGA board

## Quick Reference

### Common Make Commands
```bash
make list              # Show all demos
make <demo>            # Build specific demo
make all               # Build all demos
make clean             # Clean build artifacts
make check-toolchain   # Verify toolchain installation
```

### Memory Map
| Address | Peripheral | Headers |
|---------|-----------|---------|
| 0x80000000 | Main Memory (256MB) | - |
| 0x90000000 | IRQ Controller | `irq.h` |
| 0x91000000 | Timer | `timer.h` |
| 0x92000000 | UART | `uart.h` |
| 0x93000000 | SPI | `spi.h` |
| 0x94000000 | GPIO | `gpio.h` |
| 0x95000000 | LED Matrix | `led.h` |
| 0x96000000 | Display Controller | `display.h` |

### SpudKit API Examples
```c
// UART
uart_init();
uart_puts("Hello\r\n");
uart_putc('A');

// Display
display_clear(RGB565(0, 0, 0));
display_draw_pixel(x, y, color);
display_draw_line(x1, y1, x2, y2, color);
display_draw_rect(x, y, w, h, color);
display_fill_rect(x, y, w, h, color);
display_draw_circle(cx, cy, radius, color);
display_update();

// GPIO
gpio_set_direction(pin, GPIO_OUTPUT);
gpio_write(pin, 1);
uint32_t value = gpio_read(pin);

// Timer
timer_init(0, period);
timer_enable(0);
```

### Build Flags
- `SIM_DISPLAY=1` - Enable terminal display output (simulation)
- `UART_DISPLAY=1` - Enable UART ANSI color output (hardware)

## Additional Resources

- [SPUD Environment Documentation](../README.md) - Parent project with convenience scripts
- [RISC-V SoC Documentation](../spud_riscv_soc/README.md) - Hardware details and memory map
- Demo source code - Best reference for using SpudKit features
