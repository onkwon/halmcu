DEVICE_SUBDIRS := \
	$(LIBABOV_ROOT)/devices/common \
	$(LIBABOV_ROOT)/devices/$(VENDOR)/$(DEVICE) \

DEVICE_SRCS := \
	$(foreach d, $(DEVICE_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

DEVICE_INCS := \

LIBABOV_SRCS += $(DEVICE_SRCS)
LIBABOV_INCS += $(DEVICE_INCS)
