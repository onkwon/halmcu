DEVICE_SUBDIRS := \
	$(HALMCU_ROOT)/devices/common \
	$(HALMCU_ROOT)/devices/$(VENDOR)/$(DEVICE) \

DEVICE_SRCS := \
	$(foreach d, $(DEVICE_SUBDIRS), \
		$(shell find $(d) -type f -regex ".*\.c")) \

DEVICE_INCS := \

HALMCU_SRCS += $(DEVICE_SRCS)
HALMCU_INCS += $(DEVICE_INCS)
