ARCH := armv7m3

include $(LIBABOV_ROOT)/arch/arm/armv7-m/cortex-m.mk

CFLAGS += -mcpu=cortex-m3

M3_SUBDIRS := $(LIBABOV_ROOT)/arch/arm/armv7-m/m3
M3_SRCS := $(foreach d, $(M3_SUBDIRS), $(shell find $(d) -type f -regex ".*\.c"))
M3_INCS := $(LIBABOV_ROOT)/arch/arm/include/CMSIS_5/Device/ARM/ARMCM3/Include

LIBABOV_SRCS += $(M3_SRCS)
LIBABOV_INCS += $(M3_INCS)
