COMPONENT_NAME = systick

SRC_FILES = \
	../arch/arm/armv7-m/systick.c \

TEST_SRC_FILES = \
	src/arch/arm/cortex-m/systick_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	stubs/overrides \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-Darmv7m3 \
	-DUNITTEST \
	-Iassert.h \

include MakefileRunner.mk