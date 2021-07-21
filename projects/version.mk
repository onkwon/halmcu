HALMCU_VERSION ?= $(shell git describe --long --tags --dirty --always)
version-list := $(subst -, , $(HALMCU_VERSION))
HALMCU_VERSION_TAG ?= $(strip $(word 1, $(version-list))).$(word 2, $(version-list))

version-tmp := $(subst ., , $(subst v,, $(HALMCU_VERSION_TAG)))
VERSION_MAJOR := $(strip $(word 1, $(version-tmp)))
VERSION_MINOR := $(strip $(word 2, $(version-tmp)))
VERSION_PATCH := $(strip $(word 3, $(version-tmp)))

export HALMCU_VERSION
export HALMCU_VERSION_TAG
