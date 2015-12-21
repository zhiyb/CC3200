# Makefile for CC3200
# Author: Yubo Zhi (normanzyb@gmail.com)

TOPDIR	?= $(dir $(lastword $(MAKEFILE_LIST)))
TOPDIR	:= $(TOPDIR)

SDK	?= $(TOPDIR)/cc3200-sdk
INCDIRS	+= $(SDK)/inc $(SDK)/driverlib
LIBS	+= $(SDK)/driverlib/gcc/exe/libdriver.a

OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

# Cross compile defines
CROSS_COMPILE	= arm-none-eabi-
SUFFIX	= .axf

# Definitions
CPU	= -mcpu=cortex-m4
#FPU	= -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
FPU	= -msoft-float -mfloat-abi=soft

MCU_FREQ	= 80000000
DEFS	+= -DF_CPU=$(MCU_FREQ)

ifdef BAUD
DEFS	+= -DBAUD=$(BAUD)
endif

OPTLEVEL	?= 0

# Rules
ifndef LIBTRG
EXTRA_TARGETS	+= lst bin
PHONYTRGS	+= lst
endif

include $(TOPDIR)/Makefile_generic.mk

# To choose a programming tool for specific platform,
# write a Makefile_platform.mk file.
# However this has no effect on compiling,
# so if flashing is not required then this can be ignored.
# Platform dependent Makefile targets may also be defined,
#
# * PROGCOM: COM port connected to CC3200 UART interface
#
# e.g. on Windows:
# PROGCOM	= 10
# include $(TOPDIR)/platform/windows/Makefile.mk
#
# e.g. on Linux:
# PROGCOM	= /dev/ttyACM0
# include $(TOPDIR)/platform/linux/Makefile.mk
#
-include $(TOPDIR)/Makefile_platform.mk

run: flash
	$(MAKE) uart

#
# Get the location of libgcc.a from the GCC front-end.
#
LIBGCC:=${shell ${CC} -mthumb -print-libgcc-file-name}

#
# Get the location of libc.a from the GCC front-end.
#
LIBC:=${shell ${CC} -print-file-name=libc.a}

#
# Get the location of libm.a from the GCC front-end.
#
LIBM:=${shell ${CC} -print-file-name=libm.a}

ASFLAG	+= -mthumb \
	   ${CPU}  \
	   ${FPU}  \
	   -MD

CFLAG	+= -mthumb             \
	   ${CPU}              \
	   ${FPU}              \
	   -ffunction-sections \
	   -fdata-sections     \
	   -MD                 \
	   -std=c99            \
	   -g

LDFLAG	+= ${CPU} ${FPU} -Wl,--gc-sections --entry ${ENTRY}
LIBS	+= '${LIBM}' '${LIBC}' '${LIBGCC}'

# Following are not needed when building libraries
ifndef LIBTRG

# Rules for building the binary image
PHONYTRGS	+= bin

%.bin: %.axf
	$(call verbose,"GEN	$@",\
	$(OBJCOPY) -O binary $< $@)

# For program
.PHONY: flash

flash: bin
	$(PROGRAM)

endif

# PHONY targets
.PHONY: $(PHONYTRGS)
$(PHONYTRGS): %: $(TRG).%
CLEAN_FILES	+= $(PHONYTRGS:%=$(TRG).%)
