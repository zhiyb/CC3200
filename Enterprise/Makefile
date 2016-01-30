TRG	= Enterprise
SRC	= main.c events.c fs.c device.c
SRC	+= startup_gcc.c
SUBDIRS	= uart0 rgbled common

LDSCRIPT	= cc3200.ld
OPTLEVEL	= s
ENTRY	= ResetISR

DEFS	+= -DUART0_BAUD=115200

include ../Makefile_CC3200.mk
