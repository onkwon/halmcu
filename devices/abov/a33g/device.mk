HAL_SUBDIRS := \
	$(LIBABOV_ROOT)/devices/common \
	$(LIBABOV_ROOT)/devices/$(VENDOR)/$(DEVICE) \

HAL_SRCS := \
	$(foreach d, $(HAL_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

HAL_INCS := \

LIBABOV_SRCS += $(HAL_SRCS)
LIBABOV_INCS += $(HAL_INCS)
