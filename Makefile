export PROJECT := libabov
export BASEDIR := $(shell pwd)
export BUILDIR := build

VERBOSE ?= 0
V ?= $(VERBOSE)
ifeq ($(V), 0)
	Q = @
else
	Q =
endif
export Q

static_targets := clean test coverage docs
targets := $(filter-out $(static_targets), $(MAKECMDGOALS))
ifneq ($(targets), )
$(targets):
	$(foreach target, $@, \
		$(Q)$(MAKE) -f projects/common/rules.mk $(target))
endif

.PHONY: test
test:
	$(Q)$(MAKE) -C tests
.PHONY: coverage
coverage:
	$(Q)$(MAKE) -C tests $@
.PHONY: clean
clean:
	$(Q)$(MAKE) -C tests clean
	$(Q)rm -rf $(BUILDIR)

.PHONY: docs
docs: docs/sphinx
	@-find $@ -mindepth 1 -name sphinx -prune -o -exec rm -rf {} \;
	@$(MAKE) -C $< clean html
	@-cp -Rf $</build/html/* docs/
