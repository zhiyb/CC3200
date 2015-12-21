# Binary programming tool taken from energia-0101E0017
# If using CC3200 kits from redbearlab.com,
# add following line to Makefile_platform.mk before including:
#
# REDBEARLAB	= yes

PROGCOM	?= 10
PROGDIR	:= $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(REDBEARLAB), yes)
PROGRAM	= cd $(PROGDIR); ./cc3200prog.exe $(PROGCOM) $(shell pwd)/$(TRG).bin h
else
PROGRAM	= cd $(PROGDIR); ./cc3200prog.exe $(PROGCOM) $(shell pwd)/$(TRG).bin n
endif

uart:
	putty.exe -serial COM$(PROGCOM) -sercfg $(BAUD)
