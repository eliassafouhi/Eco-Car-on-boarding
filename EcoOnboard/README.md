# EcoCar Onboarding 2026-27: STM32F103 Dev Board

A 4-layer, 90 × 60 mm STM32F103C8T6 board built for the EcoCar onboarding task (Phil's Lab
STM32 base design plus the required additions).

## What's on the board

| Block | Details |
|---|---|
| MCU | STM32F103C8T6 (LQFP-48), 16 MHz HSE crystal → 72 MHz, USB full-speed |
| Power | USB-C (5.1k CC pull-downs) → 500 mA polyfuse → AMS1117-3.3, USBLC6-2SC6 ESD on D+/D− |
| Programming | 1×5 SWD header (3V3, SWDIO, SWCLK, NRST, GND), reset button, BOOT0 slide switch |
| Potentiometer | 10k Bourns 3386P on **PA0 / ADC1_IN0** |
| Buttons | BTN1 **PB3**, BTN2 **PB4**, BTN3 **PB5** (to GND, internal pull-ups) |
| LEDs (14) | 7 on timer channels: **TIM2 CH2-4** (PA1 red, PA2 yellow, PA3 green), **TIM3 CH1-4** (PA6 blue, PA7 white, PB0 red, PB1 yellow). 5 GPIO: PA4 red, PA5 yellow, PA8 green, PB8 blue, PB9 red. PC13 status (active-low), green power LED |
| OLED (creative touch) | 4-pin socket for a 0.96" SSD1306 I2C module (GND, VCC, SCL, SDA) on I2C1 (PB6/PB7) |
| Expansion header J4 (2×10) | 2× 3V3, 2× 5V, 4× GND, I2C2 (PB10/PB11), SPI2 (PB12-PB15), USART1 (PA9/PA10), I2C1 (PB6/PB7), PA15, PC14. 4.7k pull-ups on both I2C buses |

### Layout rules followed
- Stackup: F.Cu signals (**horizontal**) / In1.Cu **GND plane** / In2.Cu **+3V3 plane** / B.Cu signals (**vertical**); vias change layers.
- GND pours on top and bottom, with GND stitching vias across the board and around the edge.
- 0603 minimum for R/C/LED/ferrite, no BGAs.
- Rules: 0.25 mm tracks, 0.15 mm clearance, 0.45/0.2 mm vias (within JLCPCB 4-layer capability).
- DRC: 0 errors, 0 unconnected pads (only silkscreen-overlap warnings).

## Files
```
EcoOnboard.kicad_pro / .kicad_sch / .kicad_pcb   KiCad project (open in KiCad 9/10; it upgrades the file format on save)
fab/EcoOnboard_gerbers_JLCPCB.zip                Gerbers + drill files for ordering (4 layers, 1.6 mm)
fab/BOM.csv                                      Bill of materials
fab/Schematic.pdf                                Schematic print
firmware/EcoOnboard.ioc                          STM32CubeMX configuration
firmware/Core/Src/app.c, ssd1306.c               Application code + OLED driver
firmware/Core/Inc/app.h, ssd1306.h
```

## Firmware: build and flash
1. In **STM32CubeIDE**: *File → New → STM32 Project from an Existing STM32CubeMX Configuration File (.ioc)* and pick `firmware/EcoOnboard.ioc`. Let it generate the code.
   (Or open the `.ioc` in STM32CubeMX and click **Generate Code**.)
   Check the pinout view matches the table above; if anything is missing, set it by hand (see *CubeMX settings* below).
2. Copy `app.c`, `ssd1306.c` into `Core/Src` and `app.h`, `ssd1306.h` into `Core/Inc`.
3. In `Core/Src/main.c`, add these three lines inside the USER CODE blocks:
   ```c
   /* USER CODE BEGIN Includes */
   #include "app.h"
   /* USER CODE BEGIN 2 */
   App_Init();
   /* USER CODE BEGIN 3 */   (inside while(1))
   App_Loop();
   ```
4. Plug an ST-Link into J2 (3V3, SWDIO, SWCLK, NRST, GND), build (hammer) and run/debug.
5. Open the USB-C port as a serial terminal (macOS: `screen /dev/tty.usbmodem* 115200`).

### What it does
- **BTN1**: cycles LED modes: chase (Knight Rider along the LED row), pot dimmer (PWM brightness from the pot), breathing, binary counter. The pot sets speed in the animated modes.
- **BTN2**: prints `mode / pot` over the USB COM port and USART1.
- **BTN3**: cycles OLED pages: status, pot bar graph, logo.
- PC13 LED blinks as a heartbeat; the pot value is reported every 2 s.

### CubeMX settings (if setting up by hand)
- RCC: HSE crystal. Clock: HSE 16 MHz → /2 → PLL ×9 = 72 MHz SYSCLK, APB1 /2, USB /1.5 = 48 MHz, ADC /6
- SYS: Debug = Serial Wire
- ADC1: IN0. TIM2: CH2-4 PWM Generation. TIM3: CH1-4 PWM Generation (prescaler 71, period 999; app.c also sets these)
- I2C1 + I2C2 (I2C1 at 400 kHz), SPI2 full-duplex master, USART1 asynchronous 115200
- USB: Device (FS); Middleware USB_DEVICE: Communication Device Class (Virtual Port Com)
- GPIO outputs: PA4, PA5, PA8, PB8, PB9, PC13 (initial high), PB12. GPIO inputs with pull-up: PB3, PB4, PB5

## Putting it on GitHub
```bash
cd ~/Documents/EcoCar/EcoOnboard
git init
git add .
git commit -m "EcoCar onboarding board: schematic, 4-layer layout, firmware"
git branch -M main
git remote add origin https://github.com/<your-username>/EcoOnboard.git   # create the empty repo on github.com first
git push -u origin main
```
Then share the repo with the Electrical Lead.
