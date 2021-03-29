COMPONENT_NAME = watchdog

SRC_FILES = \
	../devices/abov/a33g/wdt.c \

TEST_SRC_FILES = \
	src/devices/abov/a33g/wdt_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	stubs/overrides \

CPPUTEST_CPPFLAGS = \
	-DDEVICE=a33g \
	-Darmv7m3 \
	-DA33G52x_H \
	-DUNITTEST \
	-include a33g.h

MOCKS_SRC_DIRS = mocks

include MakefileRunner.mk
