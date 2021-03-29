ifndef SILENCE
SILENCE = @
endif

export TEST_TARGET = $(BUILDIR)/$(COMPONENT_NAME)_tests
export CPPUTEST_OBJS_DIR = $(BUILDIR)/objs
export CPPUTEST_LIB_DIR = $(BUILDIR)/lib

export CPPUTEST_USE_EXTENSIONS=Y
export CPPUTEST_USE_MEM_LEAK_DETECTION=Y
export CPPUTEST_USE_GCOV=Y
export GCOV_ARGS=-b -c # branch coverage report
export CPPUTEST_EXE_FLAGS = "-c" # colorize output

export CPPUTEST_WARNINGFLAGS = \
	-Wall \
	-Wextra \
	-Wformat=2 \
	-Wmissing-prototypes \
	-Wstrict-prototypes \
	-Wmissing-declarations \
	-Wcast-align \
	-Wpointer-arith \
	-Wbad-function-cast \
	-Wnested-externs \
	-Wcast-qual \
	-Wmissing-format-attribute \
	-Wmissing-include-dirs \
	-Wformat-nonliteral \
	-Wdouble-promotion \
	-Wfloat-equal \
	-Winline \
	-Wundef \
	-Wunused-macros \
	-Wshadow \
	-Wwrite-strings \
	-Waggregate-return \
	-Wredundant-decls \
	-Wconversion \
	-Wstrict-overflow=5 \
	-Werror \
	\
	-Wswitch-default \
	-Wswitch-enum \
	-Wredundant-decls \

ifeq ($(shell uname), Darwin)
CPPUTEST_WARNINGFLAGS += -Wno-error=poison-system-directories
else
#TARGET_PLATFORM ?= $(shell gcc -dumpmachine)
endif

include $(CPPUTEST_HOME)/build/MakefileWorker.mk
