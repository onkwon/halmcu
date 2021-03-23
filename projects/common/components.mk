COMPONENTS_DIRS := \
	drivers/common \

ifeq ($(ARCH), armv7m0plus)
COMPONENTS_DIRS += arch/arm/armv7-m/m0plus
else ifeq ($(ARCH), armv7m3)
COMPONENTS_DIRS += arch/arm/armv7-m/m3
else ifeq ($(ARCH), armv7m4)
COMPONENTS_DIRS += arch/arm/armv7-m/m4
else
$(error not supported $(ARCH) cpu architecture provided)
endif

COMPONENTS_SRCS := \
	$(foreach d, $(COMPONENTS_DIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \
	arch/arm/armv7-m/reset.c \

COMPONENTS_INCS := \
	arch/arm/include/CMSIS_5/Device/ARM/ARMCM0plus/Include \
	arch/arm/include/CMSIS_5/Device/ARM/ARMCM1/Include \
	arch/arm/include/CMSIS_5/Device/ARM/ARMCM3/Include \
	arch/arm/include/CMSIS_5/Device/ARM/ARMCM4/Include \
	arch/arm/include/CMSIS_5/CMSIS/Core/Include \
	include
