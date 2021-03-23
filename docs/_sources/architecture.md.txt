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
├── docs/
├── drivers/
│   ├── a31/
│   ├── a33/
│   └── common/
├── include/
│   └── abov/
├── projects/
│   └── common/
└── tests/
```

## Build System
[Make](https://www.gnu.org/software/make/manual/make.html) build automation tool is used.

1. You shoud specify a target you want to build for, something like: `make a33`
2. It will include a project Makefile according to the target. e.g. `projects/a33.mk`
3. In the project Makefile, all drivers it supports are included by `drivers/a33/driver.mk`
4. Then it decides what cpu architecture is of the MCU
5. And build based on the architecture with its Makefile, `arch/arm/armv7-m/m3/arch.mk`
