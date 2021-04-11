COMPONENT_NAME = uart_hal

SRC_FILES = \
	../hal/uart.c \

TEST_SRC_FILES = \
	src/hal/uart_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks/ll \
	mocks \

INCLUDE_DIRS = \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUART_MAX_DRIVER_HANDLE=3 \
	-DUNITTEST \

include MakefileRunner.mk
