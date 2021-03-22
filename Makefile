
.PHONY: docs
docs: docs/sphinx
	@-find $@ -mindepth 1 -name sphinx -prune -o -exec rm -rf {} \;
	@$(MAKE) -C $< clean html
	@-cp -Rf $</build/html/* docs/
