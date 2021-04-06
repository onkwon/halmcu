COMPONENT_NAME = watchdog_driver

SRC_FILES = \
	../hal/wdt.c \

TEST_SRC_FILES = \
	src/hal/wdt_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUNITTEST \

include MakefileRunner.mk
