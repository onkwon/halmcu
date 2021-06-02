COMPONENT_NAME = spi_hal

SRC_FILES = \
	../hal/spi.c \

TEST_SRC_FILES = \
	src/hal/spi_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks/ll \
	mocks \

INCLUDE_DIRS = \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \
	../include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUNITTEST \

include MakefileRunner.mk
