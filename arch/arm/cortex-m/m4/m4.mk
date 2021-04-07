ARCH := armv7m4

include $(LIBABOV_ROOT)/arch/arm/cortex-m/cortex-m.mk

CFLAGS += -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16

M4_SUBDIRS := $(LIBABOV_ROOT)/arch/arm/cortex-m/m4
M4_SRCS := $(foreach d, $(M4_SUBDIRS), $(shell find $(d) -type f -regex ".*\.c"))
M4_INCS := $(LIBABOV_ROOT)/arch/arm/include/CMSIS_5/Device/ARM/ARMCM4/Include

LIBABOV_SRCS += $(M4_SRCS)
LIBABOV_INCS += $(M4_INCS)
