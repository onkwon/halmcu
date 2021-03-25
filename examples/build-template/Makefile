PROJECT := a33g
export MCU ?= a33g

LD_SCRIPT := ports/abov/a33g526.ld
LDFLAGS += -specs=nano.specs #-specs=nosys.specs

SRCS += src/main.c

NDEBUG := true

#include ../../projects/common/runner.mk
LIBABOV_ROOT := ../../
include $(LIBABOV_ROOT)/projects/common/runner.mk
