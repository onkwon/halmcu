PROJECT = a33g526
export DEVICE ?= a33g
export HSE ?= 8000000

LD_SCRIPT = ports/abov/a33g526.ld
LDFLAGS = -specs=nano.specs #-specs=nosys.specs

SRCS = src/main.c

#NDEBUG = true

LIBABOV_ROOT ?= ../../
include $(LIBABOV_ROOT)/projects/runner.mk

.PHONY: flash erase gdbserver
flash: build/a33g/$(PROJECT).hex
	$(Q)pyocd $@ -t a33g526 --pack=ports/abov/ABOV.A33G52x_Series.1.0.10.pack $<
erase:
	$(Q)pyocd $@ -t a33g526 --pack=ports/abov/ABOV.A33G52x_Series.1.0.10.pack --chip
gdbserver:
	$(Q)pyocd $@ -t a33g526 --pack=ports/abov/ABOV.A33G52x_Series.1.0.10.pack
