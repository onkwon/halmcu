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
├── devices/
│   └── abov
│       ├── a31/
│       ├── a33/
│       └── common/
├── drivers/
│   ├── adc/
│   └── uart/
├── examples/
├── include
│   └── abov
│       ├── drivers
│       └── sys
├── projects/
│   └── common/
└── tests/
```

| Directory | Description                                                     |
| --------- | -----------                                                     |
| devices   | low level and hardware dependent code for the specific device   |
| drivers   | device abstraction code for drivers. no hardware dependency     |


## Build System
[Make](https://www.gnu.org/software/make/manual/make.html) build automation tool is used.

Additional tools like Kbuild or scripts are intentionally avoided not to
increase complexity.

1. You shoud specify device you want to build for, something like: `make DEVICE=a33g`
2. It will automatically include a project Makefile according to the device. e.g. `projects/a33g.mk`
3. In the project Makefile, all drivers it supports are included by `drivers/a33g/driver.mk`
4. Then it decides what cpu architecture is of the device
5. And build based on the architecture with its Makefile, `arch/arm/armv7-m/m3/m3.mk`

## Software Layers
### Low Level Layer
### Hardwar Abstraction Layer
### Driver Layer
