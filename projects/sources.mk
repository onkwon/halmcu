ifeq ($(DEVICE),)
$(error no device specified)
endif

include $(HALMCU_ROOT)/projects/devices/$(DEVICE).mk
include $(HALMCU_ROOT)/projects/version.mk
include $(HALMCU_ROOT)/projects/toolchain.mk

ifeq ($(ARCH),)
$(error no cpu architecture specified)
endif

DRV_SUBDIRS := $(HALMCU_ROOT)/hal
DRV_SRCS := $(foreach d, $(DRV_SUBDIRS), $(shell find $(d) -type f -regex ".*\.c"))
HALMCU_SRCS += $(DRV_SRCS)

HALMCU_SRCS_STRIPPED := $(HALMCU_SRCS:$(HALMCU_ROOT)/%=%)
HALMCU_OBJS := $(addprefix $(OUTDIR)/, $(HALMCU_SRCS_STRIPPED:.c=.o))
HALMCU_INCS += $(HALMCU_ROOT)/include
HALMCU_DEFS += \
	$(ARCH) \
	F_HSE=$(HSE) \
	F_HSI=$(HSI) \
	IRQ_DEFINES=\"$(IRQ_DEFINES)\" \
	HALMCU_BUILD_DATE=\""$(shell date)"\" \
	HALMCU_VERSION=$(HALMCU_VERSION) \
