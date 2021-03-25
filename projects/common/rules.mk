OUTDIR := $(BUILDIR)/$(MCU)

$(info building    $(MCU))

include $(LIBABOV_ROOT)/projects/common/sources.mk

OBJS += $(addprefix $(OUTDIR)/, $(SRCS:.c=.o))
DEPS += $(LIBABOV_OBJS:.o=.d) $(OBJS:.o=.d)
DEFS += $(LIBABOV_DEFS)
INCS += $(LIBABOV_INCS)

OUTCOM := $(OUTDIR)/$(PROJECT)
OUTLIB := $(OUTCOM).a
OUTELF := $(OUTCOM).elf
OUTBIN := $(OUTCOM).bin
OUTHEX := $(OUTCOM).hex
OUTSHA := $(OUTCOM).sha256
OUTSRC := $(OUTDIR)/sources.txt
OUTINC := $(OUTDIR)/includes.txt
OUTDEF := $(OUTDIR)/defines.txt

OUTPUT := $(OUTBIN) $(OUTHEX) $(OUTSHA) $(OUTSRC) $(OUTINC) $(OUTDEF) \
	$(OUTCOM).size $(OUTCOM).sym $(OUTCOM).lst $(OUTCOM).dump

all: $(OUTPUT)
	$(Q)$(SZ) -t --common $(sort $(LIBABOV_OBJS) $(OBJS))
	$(Q)$(SZ) $(OUTELF)

$(OUTCOM).size: $(OUTELF)
	$(info generating  $@)
	$(Q)$(NM) -S --size-sort $< > $@
$(OUTCOM).sym: $(OUTELF)
	$(info generating  $@)
	$(Q)$(OD) -t $< | sort > $@
$(OUTCOM).lst: $(OUTELF)
	$(info generating  $@)
	$(Q)$(OD) -d $< > $@
$(OUTCOM).dump: $(OUTELF)
	$(info generating  $@)
	$(Q)$(OD) -x $< > $@

$(OUTSRC): $(OUTELF)
	$(info generating  $@)
	$(Q)echo $(sort $(LIBABOV_SRCS) $(SRCS)) | tr ' ' '\n' > $@
$(OUTINC): $(OUTELF)
	$(info generating  $@)
	$(Q)echo $(subst -I,,$(sort $(INCS))) | tr ' ' '\n' > $@
$(OUTDEF): $(OUTELF)
	$(info generating  $@)
	$(Q)echo $(sort $(DEFS)) | tr ' ' '\n' > $@
$(OUTSHA): $(OUTELF)
	$(info generating  $@)
	$(Q)openssl dgst -sha256 $< > $@
$(OUTHEX): $(OUTELF)
	$(info generating  $@)
	$(Q)$(OC) -O ihex $< $@
$(OUTBIN): $(OUTELF)
	$(info generating  $@)
	$(Q)$(OC) -O binary $< $@
$(OUTELF): $(LIBABOV_OBJS) $(OBJS) $(LD_SCRIPT)
	$(info linking     $@)
	$(Q)$(CC) -o $@ $(LIBABOV_OBJS) $(OBJS) \
		-Wl,-Map,$(OUTCOM).map \
		$(addprefix -T, $(LD_SCRIPT)) \
		$(CFLAGS) \
		$(LDFLAGS) \
		$(LIBDIRS) \
		$(LIBS)

.PHONY: lib
lib: $(OUTLIB)
$(OUTLIB): $(LIBABOV_OBJS)
	$(info archiving   $@)
	$(Q)rm -f $@
	$(Q)$(AR) $(ARFLAGS) $@ $^ 1> /dev/null 2>&1
	$(Q)$(SZ) -t --common $(sort $^)
$(LIBABOV_OBJS): $(OUTDIR)/%.o: $(LIBABOV_ROOT)/%.c $(MAKEFILE_LIST)
	$(info compiling   $(<:$(LIBABOV_ROOT)/%=%))
	@mkdir -p $(@D)
	$(Q)$(CC) -o $@ -c $< -MMD \
		$(addprefix -D, $(DEFS)) \
		$(addprefix -I, $(INCS)) \
		$(CFLAGS)

$(OBJS): $(OUTDIR)/%.o: %.c $(MAKEFILE_LIST)
	$(info compiling   $<)
	@mkdir -p $(@D)
	$(Q)$(CC) -o $@ -c $< -MMD \
		$(addprefix -D, $(DEFS)) \
		$(addprefix -I, $(INCS)) \
		$(CFLAGS)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
-include $(DEPS)
endif
endif
