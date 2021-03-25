DRV_SUBDIRS := \
	$(LIBABOV_ROOT)/drivers/common \
	$(LIBABOV_ROOT)/drivers/$(MCU) \

DRV_SRCS := \
	$(foreach d, $(DRV_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

DRV_INCS := \

LIBABOV_SRCS += $(DRV_SRCS)
LIBABOV_INCS += $(DRV_INCS)
