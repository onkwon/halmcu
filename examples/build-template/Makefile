PROJECT := a33g
export DEVICE ?= a33g

LD_SCRIPT := ports/abov/a33g526.ld
LDFLAGS += -specs=nano.specs #-specs=nosys.specs
CFLAGS += -Wno-error=unused-macros

SRCS += src/main.c

NDEBUG := true

#include ../../projects/runner.mk
LIBABOV_ROOT := ../../
include $(LIBABOV_ROOT)/projects/runner.mk
