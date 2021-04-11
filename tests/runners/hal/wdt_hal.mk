COMPONENT_NAME = watchdog_driver

SRC_FILES = \
	../hal/wdt.c \

TEST_SRC_FILES = \
	src/hal/wdt_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks/ll \
	mocks \

INCLUDE_DIRS = \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUNITTEST \

include MakefileRunner.mk
