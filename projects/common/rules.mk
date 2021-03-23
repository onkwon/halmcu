target := $(firstword $(MAKECMDGOALS))
OUTDIR := $(BUILDIR)/$(target)

$(info building    $(MAKECMDGOALS))

include projects/common/version.mk
include projects/common/toolchain.mk
include projects/$(target).mk

ifeq ($(ARCH),)
$(error no cpu architecture specified)
endif

DEFS += \
	$(ARCH) \
	LIBABOV_BUILD_DATE=\""$(shell date)"\" \
	LIBABOV_VERSION=$(VERSION) \

OBJS += $(addprefix $(OUTDIR)/, $(SRCS:.c=.o))
DEPS += $(OBJS:.o=.d)

OUTCOM := $(OUTDIR)/$(PROJECT)
OUTLIB := $(OUTCOM).a
OUTSHA := $(OUTCOM).sha256
OUTPUT := $(OUTSHA) $(OUTDIR)/sources.txt $(OUTDIR)/includes.txt

$(MAKECMDGOALS): all
all: $(OUTPUT)
	$(Q)$(SZ) -t --common $(sort $(OBJS))

$(OUTDIR)/sources.txt: $(OUTLIB)
	$(info generating  $@)
	$(Q)echo $(sort $(SRCS)) | tr ' ' '\n' > $@
$(OUTDIR)/includes.txt: $(OUTLIB)
	$(info generating  $@)
	$(Q)echo $(subst -I,,$(sort $(INCS))) | tr ' ' '\n' > $@

$(OUTSHA): $(OUTLIB)
	$(info generating  $@)
	$(Q)openssl dgst -sha256 $< > $@
$(OUTLIB): $(OBJS)
	$(info archiving   $@)
	$(Q)rm -f $@
	$(Q)$(AR) $(ARFLAGS) $@ $^ 1> /dev/null 2>&1

$(OBJS): $(OUTDIR)/%.o: %.c $(MAKEFILE_LIST)
	$(info compiling   $<)
	@mkdir -p $(@D)
	$(Q)$(CC) -o $@ -c $*.c -MMD \
		$(addprefix -D, $(DEFS)) \
		$(addprefix -I, $(INCS)) \
		$(CFLAGS)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
-include $(DEPS)
endif
endif
