![Build Status](https://github.com/onkwon/libabov/workflows/build/badge.svg)
[![Security Rating](https://sonarcloud.io/api/project_badges/measure?project=onkwon_libabov&metric=security_rating)](https://sonarcloud.io/dashboard?id=onkwon_libabov)
[![Reliability Rating](https://sonarcloud.io/api/project_badges/measure?project=onkwon_libabov&metric=reliability_rating)](https://sonarcloud.io/dashboard?id=onkwon_libabov)
[![Coverage](https://sonarcloud.io/api/project_badges/measure?project=onkwon_libabov&metric=coverage)](https://sonarcloud.io/dashboard?id=onkwon_libabov)

*libabov* provides a developing environment for ABOV devices by including low
level hardware drivers, libraries, and examples for peripherals.

See online documentation at [libabov.readthedocs.io](https://libabov.readthedocs.io/)

# Getting Started
## Installing prerequisites
### GNU Arm Embedded Toolchain
1. Download the [GNU Arm Embedded Toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads).
2. Locate it where desired
3. Add path to environment variable
4. Then you can check that ARM GCC is in your path:

```bash
$ arm-none-eabi-gcc --version
arm-none-eabi-gcc (GNU Arm Embedded Toolchain 10-2020-q4-major) 10.2.1 20201103 (release)
Copyright (C) 2020 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

Alternatively you can do it with package manager:

* [MacOSX](https://formulae.brew.sh/cask/gcc-arm-embedded)
  - `$ brew install --cask gcc-arm-embedded`
* Linux
  - `$ sudo apt-get install -y gcc-arm-none-eabi`
* Windows
  - Download and run [the installer](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)

## Setting up the development environment
### Visual Studio Code
1. Install [Visual Studio Code](https://code.visualstudio.com/)
2. Install Visual Studio Code Plugins
  * [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
3. Press `F1` to display the command palette
4. At the command palette prompt, enter `gitcl`, select the **Git: Clone (Recursive)**
5. When prompted for the **Repository URL**, enter `https://github.com/onkwon/libabov-example.git`
6. And press `F1` again. At the command palette prompt enter `task`, select the
   **Tasks: Run Build Task**

### Embedded Studio
### IAR
### Keil
### Other
*libabov* is a self-contained C library that you will need to include into your
project.

#### 1. Get *libabov* into your project

```bash
$ cd ${YOUR_PROJECT_DIR}
$ git submodule add https://github.com/onkwon/libabov.git ${YOUR_THIRD_PARTY_DIR}/libabov
```

#### 2. Add *libabov* to your existing build system
##### Make
Please refer to a [build template](examples/build-template/Makefile).
##### Other
Please refer to a [custom build example](examples/custom-build).

1. Add the sources under `${YOUR_THIRD_PARTY_DIR}/libabov/drivers/*.c` to your
   project
2. Add the sources under `${YOUR_THIRD_PARTY_DIR}/libabov/devices/common/*.c` to
   your project
3. Add the sources under
   `${YOUR_THIRD_PARTY_DIR}/libabov/devices/${VENDOR}/${DEVICE}/*.c` to your
   project
4. Add cpu architecture specific sources under
   `${YOUR_THIRD_PARTY_DIR}/libabov/arch/${YOUR_ARCH}` to your project
5. Add `${YOUR_THIRD_PARTY_DIR}/libabov/include` to the include paths for your
   project
6. Add CMSIS include path to the include paths for your project
7. Pass `DEVICE`, `ARCH`, and `HSE` definitions to your compiler

## Running an example

## Supported devices and peripherals

* ✅ Supported
* ➖ Not supported
* ❌ Unavailable in device
* <code>&nbsp;</code> Planned

|            | ABOV   |      |      |      | STM32 |    |    |
| ---------- | ----   | ---- | ---- | ---- | --    | -- | -- |
| Peripheral | A31G1x | A33G | A33M | A34M | F1    | F4 | F7 |
| ADC        |        | ✅   |      |      | ✅    |    |    |
| CAN        |        | ❌   |      |      |       |    |    |
| CLK        |        | ✅   |      |      | ✅    |    |    |
| CRC        |        | ❌   |      |      |       |    |    |
| DAC        |        | ❌   |      |      |       |    |    |
| DMA        |        | ❌   |      |      |       |    |    |
| ETH        |        | ❌   |      |      |       |    |    |
| FLASH      |        |      |      |      |       |    |    |
| FSMC       |        | ❌   |      |      |       |    |    |
| GPIO       |        | ✅   |      |      | ✅    |    |    |
| I2C        |        | ✅   |      |      |       |    |    |
| PWR        |        | ✅   |      |      |       |    |    |
| RTC        |        | ❌   |      |      |       |    |    |
| SDIO       |        | ❌   |      |      |       |    |    |
| SPI        |        | ✅   |      |      |       |    |    |
| Timer      |        | ✅   |      |      |       |    |    |
| UART       |        | ✅   |      |      | ✅    |    |    |
| USB        |        | ❌   |      |      |       |    |    |
| WDT        |        | ✅   |      |      | ✅    |    |    |
