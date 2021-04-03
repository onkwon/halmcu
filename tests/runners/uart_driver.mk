COMPONENT_NAME = uart_driver

SRC_FILES = \
	../drivers/uart.c \

TEST_SRC_FILES = \
	src/drivers/uart_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUART_MAX_DRIVER_HANDLE=3 \
	-DUNITTEST \

include MakefileRunner.mk
