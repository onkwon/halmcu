COMPONENT_NAME = clock

SRC_FILES = \
	../devices/abov/a33g/clk.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/clk_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks \

INCLUDE_DIRS = \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \
	../include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/abov/a33g/irq.def\" \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DF_HSE=8000000 \
	-DUNITTEST \
	-include a33g.h

include MakefileRunner.mk
