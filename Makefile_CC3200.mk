# Makefile for CC3200
# Author: Yubo Zhi (normanzyb@gmail.com)

TOPDIR	?= $(dir $(lastword $(MAKEFILE_LIST)))
TOPDIR	:= $(TOPDIR)

SDK	?= $(TOPDIR)/cc3200-sdk

INCDIRS	+= $(SDK)/inc
INCDIRS	+= $(SDK)/driverlib
INCDIRS	+= $(SDK)/simplelink/include

LIBS	+= $(SDK)/simplelink/gcc/exe/libsimplelink_nonos_opt.a
LIBS	+= $(SDK)/driverlib/gcc/exe/libdriver_opt.a

OBJCOPY	= $(CROSS_COMPILE)objcopy
OBJDUMP	= $(CROSS_COMPILE)objdump

# Cross compile defines
CROSS_COMPILE	= arm-none-eabi-
SUFFIX	= .axf

# Definitions
CPU	= -mcpu=cortex-m4 -mthumb
#FPU	= -mfpu=fpv4-sp-d16 -mfloat-abi=softfp
FPU	= -msoft-float -mfloat-abi=soft

# System clock frequency
DEFS	+= -DSYS_CLK=80000000 -Dgcc

ifdef BAUD
DEFS	+= -DBAUD=$(BAUD)
endif

OPTLEVEL	?= s

FLAGS	+= -ggdb3 $(CPU) $(FPU)

ASFLAG	+= -MD

CFLAG	+= -ffunction-sections \
	   -fdata-sections     \
	   -MD                 \
	   -std=c99

LDFLAG	+= -Wl,--gc-sections --entry ${ENTRY} -ggdb3

# Rules
EXTRA_TARGETS	+= lst bin
PHONYTRGS	+= lst

include $(TOPDIR)/Makefile_generic.mk

# Get the location of libs from the GCC front-end.
LIBGCC:=${shell ${CC} ${CPU} -print-libgcc-file-name}
LIBC:=${shell ${CC} ${CPU} -print-file-name=libc.a}
LIBM:=${shell ${CC} ${CPU} -print-file-name=libm.a}
LIBS	+= -nostartfiles -nostdlib ${LIBC} ${LIBGCC} ${LIBM}

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

openocd:
	openocd -f $(TOPDIR)/platform/cc3200.cfg

gdb: $(TRG)$(SUFFIX)
	$(CROSS_COMPILE)gdb -x $(TOPDIR)/platform/gdbinit $<

# Rules for building the binary image
PHONYTRGS	+= bin

%.bin: %.axf
	$(call verbose,"GEN	$@",\
	$(OBJCOPY) -O binary $< $@)

# For program
.PHONY: flash
flash: bin
	$(PROGRAM)

# PHONY targets
.PHONY: $(PHONYTRGS)
$(PHONYTRGS): %: $(TRG).%
CLEAN_FILES	+= $(PHONYTRGS:%=$(TRG).%)
