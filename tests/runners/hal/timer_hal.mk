COMPONENT_NAME = timer_hal

SRC_FILES = \
	../hal/timer.c \

TEST_SRC_FILES = \
	src/hal/timer_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-DUNITTEST \

include MakefileRunner.mk
