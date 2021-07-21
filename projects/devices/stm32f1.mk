VENDOR := st
HSE ?= 8000000
HSI ?= 8000000
VDD ?= 33
AVDD ?= 33
IRQ_DEFINES ?= ../devices/$(VENDOR)/$(DEVICE)/irq.def

include $(HALMCU_ROOT)/arch/arm/cortex-m/m3/m3.mk
include $(HALMCU_ROOT)/devices/$(VENDOR)/$(DEVICE)/device.mk
