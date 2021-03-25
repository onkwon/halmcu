# Design and Architecture

## Directory Structure

```
.
├── arch/
│   └── arm/
│       ├── armv7-m/
│       │   ├── m0plus/
│       │   ├── m3/
│       │   └── m4/
│       └── include/
│           └── CMSIS/
├── docs/
├── drivers/
│   ├── a31/
│   ├── a33/
│   └── common/
├── examples/
├── include
│   └── abov
│       ├── drivers
│       └── sys
├── projects/
│   └── common/
└── tests/
```

## Build System
[Make](https://www.gnu.org/software/make/manual/make.html) build automation tool is used.

1. You shoud specify MCU you want to build for, something like: `make MCU=a33g`
2. It will automatically include a project Makefile according to the MCU. e.g. `projects/a33g.mk`
3. In the project Makefile, all drivers it supports are included by `drivers/a33g/driver.mk`
4. Then it decides what cpu architecture is of the MCU
5. And build based on the architecture with its Makefile, `arch/arm/armv7-m/m3/m3.mk`
