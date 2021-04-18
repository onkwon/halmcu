export LIBABOV_ROOT ?= $(subst projects,,$(abspath $(dir $(lastword $(MAKEFILE_LIST)))))
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

tooling := flash erase monitor gdbserver
static-goals := clean test coverage docs $(tooling)
goals := $(filter-out $(static-goals), $(MAKECMDGOALS))
ifeq ($(goals), all_devices)
device-list := $(shell find $(LIBABOV_ROOT)/projects/devices -name "*.mk")
device-list := $(basename $(notdir $(device-list)))
goals :=
else ifeq ($(goals),)
goals := all
endif
$(goals):
	$(foreach goal, $@, \
		$(Q)$(MAKE) -f $(LIBABOV_ROOT)/projects/rules.mk $(goal))
.PHONY: all_devices $(device-list)
all_devices: $(device-list)
$(device-list):
	$(Q)$(MAKE) -f $(LIBABOV_ROOT)/projects/rules.mk lib DEVICE=$@

.PHONY: $(tooling)
$(tooling): all

.PHONY: clean
clean:
	$(Q)rm -rf $(BUILDIR)
