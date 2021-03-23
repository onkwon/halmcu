DRV_SUBDIRS := \
	drivers/common \
	drivers/$(target) \

DRV_SRCS := \
	$(foreach d, $(DRV_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

DRV_INCS := \

SRCS += $(DRV_SRCS)
INCS += $(DRV_INCS)
