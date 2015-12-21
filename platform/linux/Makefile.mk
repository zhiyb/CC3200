# Binary programming tool taken from energia-0101E0017
# If using CC3200 kits from redbearlab.com,
# add following line to Makefile_platform.mk before including:
#
# REDBEARLAB	= yes

PROGCOM	?= /dev/ttyACM0
PROGDIR	:= $(dir $(lastword $(MAKEFILE_LIST)))
ifeq ($(REDBEARLAB), yes)
PROGRAM	= cd $(PROGDIR); ./cc3200prog $(PROGCOM) $(shell pwd)/$(TRG).bin h
else
PROGRAM	= cd $(PROGDIR); ./cc3200prog $(PROGCOM) $(shell pwd)/$(TRG).bin n
endif

uart:
	microcom -s $(BAUD) -p $(PROGCOM)
