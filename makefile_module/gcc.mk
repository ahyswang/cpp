#
# Common rules for GCC Makefile
#

# ----------------------------------------------------------------------------
# cross compiler
# ----------------------------------------------------------------------------
CC_DIR := /home/yswang/toolchain/android-ndk-r12b_arm/bin
CC_PREFIX := $(CC_DIR)/arm-linux-androideabi-

AS      := $(CC_PREFIX)as
CC      := $(CC_PREFIX)gcc
CXX     := $(CC_PREFIX)g++
LD      := $(CC_PREFIX)ld
NM      := $(CC_PREFIX)nm
AR      := $(CC_PREFIX)ar
OBJCOPY := $(CC_PREFIX)objcopy
OBJDUMP := $(CC_PREFIX)objdump
SIZE    := $(CC_PREFIX)size
STRIP   := $(CC_PREFIX)strip

# ----------------------------------------------------------------------------
# tools
# ----------------------------------------------------------------------------
CP := cp

# ----------------------------------------------------------------------------
# global configuration
# ----------------------------------------------------------------------------
include $(ROOT_PATH)/config.mk

# ----------------------------------------------------------------------------
# options
# ----------------------------------------------------------------------------
QUIET ?= n
OPTIMIZE := y
HARDFP := n

# building display
ifeq ($(QUIET), y)
  Q := @
  S := -s
endif

ifeq ($(OPTIMIZE), y)
  OPTIMIZE_FLAG := -Os -DNDEBUG
else
  OPTIMIZE_FLAG := -O0 -DDEBUG
endif

ifeq ($(HARDFP), y)
  FLOAT_ABI := hard
else
  FLOAT_ABI := softfp
endif

# ----------------------------------------------------------------------------
# flags for compiler and linker
# ----------------------------------------------------------------------------
# CPU/FPU options
ifeq ($(__CONFIG_CPU_ARMV7), y)
  CPU := -march=armv7-a -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=$(FLOAT_ABI)
else
  CPU := -march=armv7-a -mcpu=cortex-a9
endif

CC_FLAGS = $(CPU) \
	$(OPTIMIZE_FLAG)

LD_FLAGS = $(CPU) -Wl,--gc-sections --specs=nano.specs \
	-Wl,-Map=$(basename $@).map,--cref

# config symbols
CC_SYMBOLS = $(CONFIG_SYMBOLS)
AS_SYMBOLS = $(CONFIG_SYMBOLS)

# standard libraries
LD_SYS_LIBS := -lstdc++ -lsupc++ -lm -lc -lgcc

# include path
INCLUDE_ROOT_PATH := $(ROOT_PATH)/include
INCLUDE_PATHS = -I$(INCLUDE_ROOT_PATH)

# ----------------------------------------------------------------------------
# common makefile for library and project
# ----------------------------------------------------------------------------
LIB_MAKE_RULES := $(ROOT_PATH)/src/lib.mk
PRJ_MAKE_RULES := $(ROOT_PATH)/project/project.mk

# ----------------------------------------------------------------------------
# common rules of compiling objects
# ----------------------------------------------------------------------------
%.o: %.asm
	$(Q)$(CC) $(CPU) $(AS_SYMBOLS) -c -x assembler-with-cpp -o $@ $<

%.o: %.s
	$(Q)$(CC) $(CPU) $(AS_SYMBOLS) -c -x assembler-with-cpp -o $@ $<

%.o: %.S
	$(Q)$(CC) $(CPU) $(AS_SYMBOLS) -c -x assembler-with-cpp -o $@ $<

%.o: %.c
	$(Q)$(CC) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu99 $(INCLUDE_PATHS) -o $@ $<

%.o: %.cpp
	$(Q)$(CPP) $(CC_FLAGS) $(CC_SYMBOLS) -std=gnu++98 -fno-rtti $(INCLUDE_PATHS) -o $@ $<
