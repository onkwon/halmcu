LIBABOV_VERSION ?= $(shell git describe --long --tags --dirty --always)
version-list := $(subst -, , $(LIBABOV_VERSION))
LIBABOV_VERSION_TAG ?= $(strip $(word 1, $(version-list))).$(word 2, $(version-list))

version-tmp := $(subst ., , $(subst v,, $(LIBABOV_VERSION_TAG)))
VERSION_MAJOR := $(strip $(word 1, $(version-tmp)))
VERSION_MINOR := $(strip $(word 2, $(version-tmp)))
VERSION_PATCH := $(strip $(word 3, $(version-tmp)))

export LIBABOV_VERSION
export LIBABOV_VERSION_TAG
