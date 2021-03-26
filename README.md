*libabov* provides a developing environment for ABOV devices by including low
level hardware drivers, libraries, and examples for peripherals.

All documentation including API reference for *libabov* is at [libabov.readthedocs.io](https://libabov.readthedocs.io/)

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
### IAR
### Keil
### Other
## Building a project
### Using libabov Build System
Please refer to [build template](https://github.com/onkwon/libabov/tree/master/examples/build-template).
### Custom Build
Please refer to [custom build example](https://github.com/onkwon/libabov/tree/master/examples/custom-build).
## Running an example
