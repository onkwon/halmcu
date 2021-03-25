export LIBABOV_ROOT ?= $(subst projects/common,,$(abspath $(dir $(lastword $(MAKEFILE_LIST)))))
export BUILDIR ?= build
export PROJECT ?= libabov

ifdef NDEBUG
export NDEBUG
DEFS += NDEBUG
endif
export CFLAGS LDFLAGS LD_SCRIPT SRCS INCS DEFS LIBS LIBDIRS

VERBOSE ?= 0
V ?= $(VERBOSE)
ifeq ($(V), 0)
	Q = @
else
	Q =
endif
export Q

static_goals := clean test coverage docs
goals := $(filter-out $(static_goals), $(MAKECMDGOALS))
ifeq ($(goals),)
goals := all
endif
$(goals):
	$(foreach goal, $@, \
		$(Q)$(MAKE) -f $(LIBABOV_ROOT)/projects/common/rules.mk $(goal))

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILDIR)
