CROSS_COMPILE ?= arm-none-eabi

CFLAGS += -mthumb
LDFLAGS += -L$(LIBABOV_ROOT)/arch/arm/armv7-m

CM_SRCS := \
	$(LIBABOV_ROOT)/arch/arm/armv7-m/reset.c \
	$(LIBABOV_ROOT)/arch/arm/armv7-m/irq.c \
	$(LIBABOV_ROOT)/arch/arm/armv7-m/vector.c \

CM_INCS := $(LIBABOV_ROOT)/arch/arm/include/CMSIS_5/CMSIS/Core/Include

LIBABOV_SRCS += $(CM_SRCS)
LIBABOV_INCS += $(CM_INCS)
