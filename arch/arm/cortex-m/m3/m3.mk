ARCH := armv7m3

include $(HALMCU_ROOT)/arch/arm/cortex-m/cortex-m.mk

CFLAGS += -mcpu=cortex-m3

M3_SUBDIRS := $(HALMCU_ROOT)/arch/arm/cortex-m/m3
M3_SRCS := $(foreach d, $(M3_SUBDIRS), $(shell find $(d) -type f -regex ".*\.c"))
M3_INCS := $(HALMCU_ROOT)/arch/arm/include/CMSIS_5/Device/ARM/ARMCM3/Include

HALMCU_SRCS += $(M3_SRCS)
HALMCU_INCS += $(M3_INCS)
