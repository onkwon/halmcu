CROSS_COMPILE ?= arm-none-eabi

CFLAGS += -mthumb
LDFLAGS += -L$(HALMCU_ROOT)/arch/arm/cortex-m

CM_SRCS := \
	$(HALMCU_ROOT)/arch/arm/cortex-m/reset.c \
	$(HALMCU_ROOT)/arch/arm/cortex-m/irq.c \
	$(HALMCU_ROOT)/arch/arm/cortex-m/vector.c \
	$(HALMCU_ROOT)/arch/arm/cortex-m/delay.c \
	$(HALMCU_ROOT)/arch/arm/cortex-m/systick.c \
	$(HALMCU_ROOT)/arch/arm/cortex-m/mutex.c \

CM_INCS := $(HALMCU_ROOT)/arch/arm/include/CMSIS_5/CMSIS/Core/Include

HALMCU_SRCS += $(CM_SRCS)
HALMCU_INCS += $(CM_INCS)
