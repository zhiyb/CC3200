TRG	= WS2812B
SRC	= main.c
SRC	+= startup_gcc.c
SUBDIRS	= uart0 rgbled common

LDSCRIPT	= cc3200.ld
OPTLEVEL	= s
ENTRY	= ResetISR
BAUD	= 115200

include ../Makefile_CC3200.mk
