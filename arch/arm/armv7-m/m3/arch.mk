ARCH := armv7m3
ARCH_SUBDIRS := arch/arm/armv7-m/m3

ARCH_SRCS := \
	$(foreach d, $(ARCH_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \
	arch/arm/armv7-m/reset.c \

ARCH_INCS := \
	arch/arm/include/CMSIS_5/Device/ARM/ARMCM3/Include \
	arch/arm/include/CMSIS_5/CMSIS/Core/Include \

SRCS += $(ARCH_SRCS)
INCS += $(ARCH_INCS)
