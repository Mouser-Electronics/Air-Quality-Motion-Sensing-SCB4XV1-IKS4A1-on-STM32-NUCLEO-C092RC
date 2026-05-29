# Air Quality & Motion Sensing — SCB4XV1 + IKS4A1 on STM32 NUCLEO-C092RC

Firmware (STM32CubeIDE) and a Python GUI for reading **environmental and motion sensor data** from a **Sensirion SENSEVAL-SCB4XV1** evaluation board and an **ST X-NUCLEO-IKS4A1** expansion board, driven by a **NUCLEO-C092RC** development board.

## About

This project explains how to extract meaningful data from the sensors on the SENSEVAL-SCB4XV1 board and motion data from ST's IKS4A1 expansion module, combining environmental and movement measurements in one design. It's aimed at industrial, IIoT, automation, and robotics applications, including autonomous mobile robots (AMRs), where sensing temperature, humidity, air quality, and pressure helps systems adapt to dynamic environments.

The Nucleo MCU reads the sensors over I²C and sends a single serial/USB stream to the PC. The IKS4A1 and SENSEVAL-SCB4XV1 act as expansion/evaluation sensor boards that interface to the Nucleo via board connectors.

## Project Materials

**Hardware:**
- [STMicroelectronics NUCLEO-C092RC Development Board](https://eu.mouser.com/new/stmicroelectronics/stm-nucleo-c092rc-dev-board/)
- [STMicroelectronics X-NUCLEO-IKS4A1 Expansion Board](https://eu.mouser.com/new/stmicroelectronics/stmicroelectronics-x-nucleo-iks4a1-board/)
- [Sensirion SENSEVAL-SCB4XV1 Evaluation Board](https://eu.mouser.com/new/sensirion/sensirion-senseval-scb4xv1-board/)
- [Adafruit USB Type-C cable](https://www.mouser.com/ProductDetail/Adafruit/4472?qs=CUBnOrq4ZJxdyof1rGiiGQ%3D%3D)

**Software & Tools:**
- STM32CubeIDE
- STM32CubeMX
- X-CUBE-MEMS1 expansion software package for STM32Cube
- Sensirion Gas Index Algorithm
- PC running Windows

---

## Hardware

Plug the Sensirion SENSEVAL-SCB4XV1 into the DIL24 connector on the IKS4A1 expansion board. Then plug the IKS4A1 into the Nucleo's Arduino-compatible headers, and connect the Nucleo to your PC via USB Type-C.

> ⚠️ **I²C address conflict:** Both the IKS4A1 and the SCB4XV1 carry an SHT40 temperature/humidity sensor, which has a single fixed I²C address. To resolve the conflict, on the IKS4A1 desolder the 0Ω resistors on **SB37** and **SB39** (SCL and SDA traces) and on **R22** (VDD trace). The LPS22DF pressure sensors don't conflict — the IKS4A1's SA0 is tied to VDD (0x5D / 0xBA) and the SCB4XV1's to GND (0x5C / 0xB8).

## CubeIDE Code

In `Cube IDE Code/` you'll find `main.c` and `main.h`. Replace the generated files in your project:

- `main.c` → `/Core/Src/`
- `main.h` → `/Core/Inc/`

> ⚠️ Replacing these files will produce errors if you haven't followed the step-by-step setup (CubeMX configuration and sensor drivers) in the Mouser guide linked below.

## GUI

Because the PuTTY terminal isn't ideal for displaying values as they update, the included Python GUI formats them as graphs. Two options:

**Run the script** — edit the COM port at the top of `GUI.py`:

​```python
PORT = "COM7"      # change to your COM port
BAUD = 115200
MAX_POINTS = 500
​```

**Or run `GUI.exe`** — select the board's COM port and set the baud rate to `115200.`

---

> 📖 Full step-by-step build guide: **[Mouser guide](https://resources.mouser.com/hardware-projects/air-quality-sensing-sensirion-environmental-sensing-module)** 
