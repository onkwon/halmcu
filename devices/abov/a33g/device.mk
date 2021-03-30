DRV_SUBDIRS := \
	$(LIBABOV_ROOT)/devices/common \
	$(LIBABOV_ROOT)/devices/$(VENDOR)/$(DEVICE) \

DRV_SRCS := \
	$(foreach d, $(DRV_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

DRV_INCS := \

LIBABOV_SRCS += $(DRV_SRCS)
LIBABOV_INCS += $(DRV_INCS)
