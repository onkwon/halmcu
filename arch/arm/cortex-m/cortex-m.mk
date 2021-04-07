CROSS_COMPILE ?= arm-none-eabi

CFLAGS += -mthumb
LDFLAGS += -L$(LIBABOV_ROOT)/arch/arm/cortex-m

CM_SRCS := \
	$(LIBABOV_ROOT)/arch/arm/cortex-m/reset.c \
	$(LIBABOV_ROOT)/arch/arm/cortex-m/irq.c \
	$(LIBABOV_ROOT)/arch/arm/cortex-m/vector.c \
	$(LIBABOV_ROOT)/arch/arm/cortex-m/delay.c \
	$(LIBABOV_ROOT)/arch/arm/cortex-m/systick.c \

CM_INCS := $(LIBABOV_ROOT)/arch/arm/include/CMSIS_5/CMSIS/Core/Include

LIBABOV_SRCS += $(CM_SRCS)
LIBABOV_INCS += $(CM_INCS)
