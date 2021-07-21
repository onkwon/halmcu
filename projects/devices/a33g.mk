VENDOR := abov
HSE ?= 8000000
IRQ_DEFINES ?= ../devices/$(VENDOR)/$(DEVICE)/irq.def

include $(HALMCU_ROOT)/arch/arm/cortex-m/m3/m3.mk
include $(HALMCU_ROOT)/devices/$(VENDOR)/$(DEVICE)/device.mk
