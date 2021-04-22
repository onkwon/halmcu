COMPONENT_NAME = uart_stm32f1

SRC_FILES = \
	../devices/st/stm32f1/uart.c \

TEST_SRC_FILES = \
	src/devices/st/stm32f1/uart_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	stubs/overrides \
	stubs/overrides/assert \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/st/stm32f1/irq.def\" \
	-DDEVICE=stm32f1 \
	-Darmv7m3 \
	-DSTM32F1_H \
	-DUNITTEST \
	-include stm32f1.h

include MakefileRunner.mk
