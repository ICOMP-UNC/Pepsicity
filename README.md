# Pepsicity

Pepsicity is a project designed to control a conveyor belt system using the LPC1769 development board. The code is written in C and is intended to be compiled and loaded onto the board using the MCUXpresso IDE.

## Table of Contents

- [Features](#features)
- [Requirements](INSTALL.md#requisitos-previos)
- [Installation](INSTALL.md#pasos-para-compilar-y-cargar-el-proyecto)
- [Usage](#usage)
- [Contributers](#contributers)
- [License](#license)

## Features

- Temperature monitoring
- Conveyor belt motor speed control
- External communication interface
- Object counting capabity
- Three Operation Modes:
    - Mode A: Get counter value everytime it goes up.
    - Mode B: Get counter value when you ask for it.
    - Mode C: Set a counter value and the system stops when it reaches it.

## Usage

1. Install dependencies.
1. Open Putty:
    - Send desired command.
    - Wait for response.

## Commands 

| Command | Description |
|---------|-------------|
| `M`   | Select mode. |
| `A`   | Start mode A (only after M command).  |
| `B`   | Start mode B (only after M command). |
| `C`   | Start mode C (only after M command) |
| `T`   | Returns the current temperature value. |
| `C*`  | Returns the current object counter value (only in mode B). |
| `V`   | Set motor speed. |

## Contributers

- @FabioGazzoni
- @FedeArnaudo
- @FrancoPerotti
- @TomasAlejo

## License

MIT License