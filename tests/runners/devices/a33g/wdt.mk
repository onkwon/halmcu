COMPONENT_NAME = watchdog_a33g

SRC_FILES = \
	../devices/abov/a33g/wdt.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/wdt_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/abov/a33g/irq.def\" \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DUNITTEST \
	-include a33g.h

MOCKS_SRC_DIRS = mocks

include MakefileRunner.mk
