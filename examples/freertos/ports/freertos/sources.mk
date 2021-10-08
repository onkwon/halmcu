FREERTOS_SRCS := \
	$(FREERTOS_ROOT)/croutine.c \
	$(FREERTOS_ROOT)/event_groups.c \
	$(FREERTOS_ROOT)/list.c \
	$(FREERTOS_ROOT)/queue.c \
	$(FREERTOS_ROOT)/stream_buffer.c \
	$(FREERTOS_ROOT)/tasks.c \
	$(FREERTOS_ROOT)/timers.c \
	$(FREERTOS_ROOT)/portable/MemMang/heap_4.c \
	$(FREERTOS_ROOT)/portable/GCC/ARM_CM3/port.c \
	$(FREERTOS_PORT_ROOT)/system.c \

FREERTOS_INCS := \
	$(FREERTOS_ROOT)/include \
	$(FREERTOS_ROOT)/portable/GCC/ARM_CM3 \
	$(FREERTOS_PORT_ROOT) \
