COMPONENT_NAME = adc_hal

SRC_FILES = \
	../hal/adc.c \

TEST_SRC_FILES = \
	src/hal/adc_test.cpp \
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
