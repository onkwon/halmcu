PROJECT := a33g
BUILDIR := build
export DEVICE ?= a33g

NDEBUG := true

LIBABOV_ROOT := ../..
include $(LIBABOV_ROOT)/projects/sources.mk

SRCS := src/main.c \
	$(LIBABOV_SRCS)
INCS := $(LIBABOV_INCS)
DEFS := $(LIBABOV_DEFS)
OBJS := $(addprefix $(BUILDIR)/, $(SRCS:.c=.o))
DEPS := $(OBJS:.o=.d)

LD_SCRIPT := ports/abov/a33g526.ld
LDFLAGS += -specs=nano.specs #-specs=nosys.specs

OUTCOM := $(BUILDIR)/$(PROJECT)
OUTELF := $(OUTCOM).elf

all: $(OUTELF)

%.elf : $(OBJS) $(LD_SCRIPT)
	$(CC) -o $@ \
		-Wl,-Map,$(OUTCOM).map \
		$(OBJS) \
		$(addprefix -T, $(LD_SCRIPT)) \
		$(CFLAGS) \
		$(LDFLAGS) \
		$(LIBS) \

$(BUILDIR)/%.o: %.c $(MAKEFILE_LIST)
	@mkdir -p $(@D)
	$(CC) -o $@ $< -c -MMD \
		$(addprefix -D, $(DEFS)) \
		$(addprefix -I, $(INCS)) \
		$(CFLAGS)

ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), depend)
-include $(DEPS)
endif
endif

.PHONY: clean
clean:
	$(Q)rm -fr $(BUILDIR)
