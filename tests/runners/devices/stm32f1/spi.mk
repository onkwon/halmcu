COMPONENT_NAME = spi_stm32f1

SRC_FILES = \
	../devices/st/stm32f1/spi.c \

TEST_SRC_FILES = \
	src/devices/st/stm32f1/spi_test.cpp \
	src/test_all.cpp \

MOCKS_SRC_DIRS = \
	mocks \

INCLUDE_DIRS = \
	stubs/overrides \
	$(CPPUTEST_HOME)/include \
	../include \

CPPUTEST_CPPFLAGS = \
	-DIRQ_DEFINES=\"../devices/st/stm32f1/irq.def\" \
	-DDEVICE=stm32f1 \
	-Darmv7m3 \
	-DF_HSI=8000000 \
	-DF_HSE=8000000 \
	-DSTM32F1_H \
	-DUNITTEST \
	-include stm32f1.h

include MakefileRunner.mk
