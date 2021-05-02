COMPONENT_NAME = clk_hal

SRC_FILES = \
	../hal/clk.c \

TEST_SRC_FILES = \
	src/hal/clk_test.cpp \
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
