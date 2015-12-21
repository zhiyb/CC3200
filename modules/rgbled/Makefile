# RGB LED connection
RGBLED_PAD	?= 14
# Number of RGB LEDs cascaded
RGBLED_NUM	?= 8
CONFS	+= RGBLED_PAD RGBLED_NUM

# File rules
NSRC	:= rgbled.c
SRC	+= $(NSRC:%=$(DIR)/%)
INCDIRS	+= $(DIR)
