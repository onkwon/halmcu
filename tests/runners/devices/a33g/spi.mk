COMPONENT_NAME = spi_a33g

SRC_FILES = \
	../devices/abov/a33g/spi.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/spi_test.cpp \
	src/test_all.cpp \

MOCKS_SRC = \
	mocks/assert_mock.cpp \

INCLUDE_DIRS = \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \
	../include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/abov/a33g/irq.def\" \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DUNITTEST \
	-include a33g.h

include MakefileRunner.mk
