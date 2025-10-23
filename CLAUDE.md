# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is firmware for the ABOV A31L12x microcontroller (Cortex-M0+ based), implementing Seoul Digital Water Meter Protocol V1.3 over LPUART. The project uses HAL drivers (v1.5.0) and supports both Keil MDK and IAR EWARM toolchains.

## Build System

### Supported IDEs
- **Keil MDK**: Use `.uvprojx` files in `Examples/*/KEIL/` directories
- **IAR EWARM**: Use `.ewp` files in `Examples/*/IAR/` directories

### Building Projects
1. Navigate to example directory (e.g., `Examples/LPUART/LPUART_Interrupt/`)
2. Open project file in either `KEIL/` or `IAR/` subdirectory
3. Build target using IDE's build command
4. Output files are generated in respective `Objects/` or `Output/` directories

### No Command-Line Build
This project does not have Makefile-based builds. All builds must be done through Keil or IAR IDE.

## Architecture

### Directory Structure
```
Core/
├── CMSIS/          # ARM CMSIS core files
└── Device/         # Device-specific startup and system files
    └── Source/
        └── system_A31L12x.c  # System initialization

Drivers/
├── Include/        # HAL driver headers (A31L12x_hal_*.h)
└── Source/         # HAL driver implementations (A31L12x_hal_*.c)

Examples/           # Application examples
└── LPUART/
    ├── LPUART_Interrupt/  # Water meter protocol implementation
    └── LPUART_Polling/

Projects/           # Template project files
├── IAR/
└── KEIL/
```

### Example Code Structure
Each example follows this pattern:
```
ExampleName/
├── main.c                    # Main application code
├── main_conf.h               # Peripheral includes and configuration
├── A31L12x_SystemClock.c     # Clock configuration
├── A31L12x_PortInit.c        # GPIO initialization
├── A31L12x_it.c              # Interrupt handlers
├── A31L12x_it.h              # Interrupt handler prototypes
├── KEIL/                     # Keil project files
└── IAR/                      # IAR project files
```

## HAL Driver Patterns

### Peripheral Initialization
```c
// 1. Configure structure
<Peripheral>_CFG_Type config;
config.field = value;

// 2. Initialize peripheral
HAL_<Peripheral>_Init(&config);
```

### Naming Convention
- **Headers**: `A31L12x_hal_<peripheral>.h`
- **Functions**: `HAL_<Peripheral>_<Action>()`
- **Interrupt Handlers**: `<Peripheral>_IRQHandler()` (defined in `A31L12x_it.c`)

### Common Peripherals
- **LPUART**: Low power UART (used for 1200 bps water meter protocol)
- **UART**: Standard UART (typically used for debug output)
- **I2C**: I2Cn_Init(), use I2Cn_Mwait() for status checking
- **GPIO**: HAL_GPIO_ConfigOutput() - note special handling in v1.4.0+
- **Power**: HAL_PWR_EnterSleepMode(), HAL_PWR_EnterPowerDownMode()

## Seoul Digital Water Meter Protocol Implementation

### Protocol Specifications
- **Baud Rate**: 1200 bps
- **Format**: 8-N-1 (8 data bits, No parity, 1 stop bit)
- **Physical Layer**: TTL Level (High: 1.5V-3.6V, Low: 0V-0.7V)
- **Preamble**: 20ms High Level required before each transmission

### Frame Structure
**TX Frame (MCU → Meter)**:
```
[HEADER] [CMD] [LENGTH] [DATA] [CHECKSUM]
  0x10    1B      1B       nB       1B
```

**RX Frame (Meter → MCU)**:
```
[START] [L] [L] [START] [DATA] [CHECKSUM] [END]
  0x68   1B  1B   0x68     nB       1B      0x16
```

### Key Implementation Files
- `Examples/LPUART/LPUART_Interrupt/meter_protocol.h` - Protocol API and constants
- `Examples/LPUART/LPUART_Interrupt/meter_protocol.c` - Protocol implementation
- `Examples/LPUART/LPUART_Interrupt/main.c` - Application integration
- `Examples/LPUART/LPUART_Interrupt/README_METER_PROTOCOL.md` - Detailed protocol documentation

### Protocol Usage Pattern
```c
// 1. Initialize LPUART at 1200 bps
LPUART_Configure();

// 2. Initialize meter protocol
Meter_Init();

// 3. Register callbacks
Meter_SetResponseCallback(OnMeterResponseReceived);
Meter_SetErrorCallback(OnMeterError);

// 4. In main loop, call task periodically
while(1) {
    Meter_Task();  // Process RX, handle timeouts

    // Send commands as needed
    Meter_SendCommand(addr, cmd, data, length);
}
```

### Hardware Connections
- **LPUART TX**: PB3 (LPTXD) → Meter RX
- **LPUART RX**: PB4 (LPRXD) → Meter TX
- **Debug UART**: Separate UART (typically 38400 bps) for diagnostic output

## Standard Example Code Pattern

```c
void main(void)
{
    // 1. System clock configuration
    SystemClock_Config();

    // 2. GPIO initialization
    PortInit();

    // 3. Peripheral initialization
    <Peripheral>_Init();

    // 4. Main loop
    while(1)
    {
        // Application logic
    }
}
```

## Important Configuration Files

### main_conf.h
Central header that includes all HAL drivers. To use a peripheral:
1. Ensure corresponding `#include "A31L12x_hal_<peripheral>.h"` is uncommented
2. Configure peripheral-specific settings

### A31L12x_SystemClock.c
Contains `SystemClock_Config()` function. Modify clock sources, PLL settings, and peripheral clocks here.

### A31L12x_PortInit.c
Contains `PortInit()` function. Add all GPIO configurations here before peripheral initialization.

## Version-Specific Notes (v1.5.0)

### Critical Fixes from Previous Versions
- **v1.4.0**: Fixed `HAL_GPIO_ConfigOutput()` - verify GPIO configuration if upgrading from older versions
- **v1.4.0**: Fixed `I2Cn_Mwait()` implementation - now uses local variable instead of direct register access
- **v1.3.0**: Fixed power mode functions (`HAL_PWR_EnterSleepMode()`, `HAL_PWR_EnterPowerDownMode()`)

### Power Management
For SRAM retention during power-down, refer to `PMU_SRAMRetentionPower` example.

## Development Guidelines

### Adding New Peripheral Support
1. Add GPIO configuration in `A31L12x_PortInit.c`
2. Add peripheral-specific defines to `main_conf.h`
3. Call initialization in `main.c` after `PortInit()`
4. Add interrupt handler to `A31L12x_it.c` if needed

### Interrupt Handling
- All interrupt handlers must be declared in `A31L12x_it.h`
- All interrupt handlers must be implemented in `A31L12x_it.c`
- Follow naming: `<PERIPHERAL>_IRQHandler(void)`

### Working with Water Meter Protocol
- Always send 20ms preamble before transmitting (handled automatically by `Meter_SendCommand()`)
- Call `Meter_Task()` frequently (every few milliseconds) to process received data and handle timeouts
- Maximum retry count is 3 on timeout/error
- Response timeout is 1000ms, byte timeout is 100ms
- Checksum uses XOR algorithm across entire frame

## Protocol Documentation References
- Seoul Digital Water Meter Protocol V1.3 (2023.01.31) - Korean HWP document in root
- Protocol versions V1.1-V1.4 - PDF files in root directory
- Implementation details: `Examples/LPUART/LPUART_Interrupt/README_METER_PROTOCOL.md`
