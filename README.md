# RISC-V RV32I Tools

This repository contains tools and demo projects for creating ELF files to run on the [RISC-V SoC](../vivado_proj/spud_riscv_soc) FPGA project.

## Project Structure

```
spud_rv32i-tools/
├── demos/                  # Demo projects
│   └── hello_world/        # Example demo project
│       ├── src/            # Source code
│       │   ├── main.c      # Main C program
│       │   ├── start.s     # Assembly startup code
│       │   └── rv32i.ld    # Linker script
│       ├── build/          # Build artifacts (generated)
│       └── elf/            # Final ELF files (generated)
├── Makefile               # Build system
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

**macOS (with Homebrew):**
```bash
brew tap riscv-software-src/riscv
brew install riscv-tools
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

## Usage

### Build a Specific Demo

```bash
make hello_world
```

This will:
1. Compile C source files
2. Assemble assembly files  
3. Link everything together
4. Generate ELF file in `demos/hello_world/elf/`
5. Create binary and disassembly files in `demos/hello_world/build/`

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

## Hardware Integration

The generated ELF files are designed to work with the RISC-V SoC FPGA project located at `../vivado_proj/spud_riscv_soc`. 

To load and run programs on the FPGA:
1. Build your demo: `make hello_world`
2. Use the ELF file from `demos/hello_world/elf/hello_world.elf` with the SoC's debug bridge
3. The program will output to UART at 1Mbaud

## Troubleshooting

**Error: "riscv32-unknown-elf-gcc not found"**
- Install the RISC-V toolchain (see setup instructions above)
- Make sure the toolchain is in your PATH

**Linking errors:**
- Check that all required source files exist
- Verify the linker script matches your target memory layout

**Program doesn't run on hardware:**
- Verify the memory map matches your SoC configuration
- Check that the reset vector is correctly set
- Ensure UART baud rate matches hardware configuration