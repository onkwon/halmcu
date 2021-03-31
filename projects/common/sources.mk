ifeq ($(DEVICE),)
$(error no device specified)
endif

include $(LIBABOV_ROOT)/projects/$(DEVICE).mk
include $(LIBABOV_ROOT)/projects/common/version.mk
include $(LIBABOV_ROOT)/projects/common/toolchain.mk

ifeq ($(ARCH),)
$(error no cpu architecture specified)
endif

LIBABOV_SRCS_STRIPPED := $(LIBABOV_SRCS:$(LIBABOV_ROOT)/%=%)
LIBABOV_OBJS := $(addprefix $(OUTDIR)/, $(LIBABOV_SRCS_STRIPPED:.c=.o))
LIBABOV_DEFS += \
	$(ARCH) \
	XTAL=$(XTAL) \
	LIBABOV_BUILD_DATE=\""$(shell date)"\" \
	LIBABOV_VERSION=$(LIBABOV_VERSION) \
