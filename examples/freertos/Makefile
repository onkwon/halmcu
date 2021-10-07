PROJECT = stm32f1
export DEVICE ?= stm32f1

LD_SCRIPT = ports/stm32f1.ld
LDFLAGS = -specs=nano.specs
CFLAGS = \
	-Wno-error=sign-conversion \
	-Wno-error=float-equal \
	-Wno-error=unused-macros \
	-Wno-error=c++-compat \
	-Wno-error=cast-align \
	-Wno-error=cast-qual \

LIBMCU_ROOT ?= external/libmcu
include $(LIBMCU_ROOT)/projects/components.mk

FREERTOS_ROOT ?= external/FreeRTOS-Kernel
FREERTOS_PORT_ROOT ?= ports/freertos
include $(FREERTOS_PORT_ROOT)/sources.mk

SRCS = \
	external/printf/printf.c \
	src/main.c \
	ports/console.c \
	$(LIBMCU_COMPONENTS_SRCS) \
	$(FREERTOS_SRCS) \

INCS = \
	external/printf \
	include \
	$(LIBMCU_COMPONENTS_INCS) \
	$(FREERTOS_INCS) \

HALMCU_ROOT ?= external/halmcu
include $(HALMCU_ROOT)/projects/runner.mk

.PHONY: flash erase gdbserver
flash: build/$(PROJECT)/$(PROJECT).hex
	$(Q)pyocd $@ -t stm32f103c8 $<
