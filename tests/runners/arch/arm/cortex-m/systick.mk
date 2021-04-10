COMPONENT_NAME = systick

SRC_FILES = \
	../arch/arm/cortex-m/systick.c \

TEST_SRC_FILES = \
	src/arch/arm/cortex-m/systick_test.cpp \
	src/test_all.cpp \

INCLUDE_DIRS = \
	stubs/overrides \
	../include \
	$(CPPUTEST_HOME)/include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/common/irq.def\" \
	-Darmv7m3 \
	-DUNITTEST \

include MakefileRunner.mk
