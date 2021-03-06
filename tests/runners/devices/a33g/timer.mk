COMPONENT_NAME = timer_a33g

SRC_FILES = \
	../devices/abov/a33g/timer.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/timer_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks \

INCLUDE_DIRS = \
	stubs/overrides \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/abov/a33g/irq.def\" \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DUNITTEST \
	-include a33g.h

include MakefileRunner.mk
