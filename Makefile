include projects/common/runner.mk

.PHONY: test
test:
	$(Q)$(MAKE) -C $(LIBABOV_ROOT)/tests
.PHONY: coverage
coverage:
	$(Q)$(MAKE) -C $(LIBABOV_ROOT)/tests $@

.PHONY: docs
docs: docs/sphinx
	@-find docs -mindepth 1 -maxdepth 1 '!' -name 'sphinx' '!' -name '.nojekyll' -exec rm -rf {} \;
	@$(MAKE) -C $< clean html
	@-cp -Rf $</build/html/* docs/
