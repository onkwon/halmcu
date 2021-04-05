ifeq ($(DEVICE),)
$(error no device specified)
endif

include $(LIBABOV_ROOT)/projects/$(DEVICE).mk
include $(LIBABOV_ROOT)/projects/common/version.mk
include $(LIBABOV_ROOT)/projects/common/toolchain.mk

ifeq ($(ARCH),)
$(error no cpu architecture specified)
endif

DRV_SUBDIRS := $(LIBABOV_ROOT)/drivers
DRV_SRCS := $(foreach d, $(DRV_SUBDIRS), $(shell find $(d) -type f -regex ".*\.c"))
LIBABOV_SRCS += $(DRV_SRCS)

LIBABOV_SRCS_STRIPPED := $(LIBABOV_SRCS:$(LIBABOV_ROOT)/%=%)
LIBABOV_OBJS := $(addprefix $(OUTDIR)/, $(LIBABOV_SRCS_STRIPPED:.c=.o))
LIBABOV_INCS += $(LIBABOV_ROOT)/include
LIBABOV_DEFS += \
	$(ARCH) \
	XTAL=$(XTAL) \
	IRQ_DEFINES=\"$(IRQ_DEFINES)\" \
	LIBABOV_BUILD_DATE=\""$(shell date)"\" \
	LIBABOV_VERSION=$(LIBABOV_VERSION) \
