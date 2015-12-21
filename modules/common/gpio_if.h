#ifndef GPIO_IF_H
#define GPIO_IF_H

// IO pad to register pin
#define gpio_pad_to_pin(io)	((io) % 8)

// Device pin to IO pad
static inline unsigned long gpio_dpin_to_pad(unsigned long pin)
{
	//*****************************************************************************
	// PIN to PAD matrix
	//*****************************************************************************
	static const unsigned long g_ulPinToPadMap[64] = {
		10,11,12,13,14,15,16,17,255,255,18,
		19,20,21,22,23,24,40,28,29,25,255,
		255,255,255,255,255,255,255,255,255,255,255,
		255,255,255,255,255,255,255,255,255,255,255,
		31,255,255,255,255,0,255,32,30,255,1,
		255,2,3,4,5,6,7,8,9
	};
	return g_ulPinToPadMap[pin];
}

// IO pad to device pin
static inline unsigned long gpio_pad_to_dpin(unsigned long pad)
{
	//*****************************************************************************
	// PAD to PIN matrix
	//*****************************************************************************
	static const unsigned long g_ulPadToPinMap[] =
	{
		49, 54, 56, 57, 58, 59, 60, 61, 62, 63,
		0, 1, 2, 3, 4, 5, 6, 7, 10, 11,
		12, 13, 14, 15, 16, 20, 255, 255, 18, 19,
		52, 44, 51, 255, 255, 255, 255, 255, 255, 255,
		17,
	};
	return g_ulPadToPinMap[pad];
}

// IO pad to register base
#define gpio_pad_to_base(pad)	(	\
	(pad) / 8 == 0 ? GPIOA0_BASE :	\
	(pad) / 8 == 1 ? GPIOA1_BASE :	\
	(pad) / 8 == 2 ? GPIOA2_BASE :	\
	(pad) / 8 == 3 ? GPIOA3_BASE :	\
	(pad) / 8 == 4 ? GPIOA4_BASE : 0)

// Register base to PRCM peripheral
static inline unsigned long gpio_base_to_prcm(unsigned long base)
{
	switch (base) {
	case GPIOA0_BASE:
		return PRCM_GPIOA0;
	case GPIOA1_BASE:
		return PRCM_GPIOA1;
	case GPIOA2_BASE:
		return PRCM_GPIOA2;
	case GPIOA3_BASE:
		return PRCM_GPIOA3;
	case GPIOA4_BASE:
		return PRCM_GPIOA4;
	};
	return 0;
}

// IO pad to PRCM peripheral
#define gpio_pad_to_prcm(pad)	gpio_base_to_prcm(gpio_pad_to_base(pad))

// IO pad set as GPIO
#define gpio_pad_init(pad)	MAP_PinTypeGPIO(gpio_pad_to_dpin(pad), PIN_MODE_0, false)

// IO pad set direction
// param: GPIO_DIR_MODE_OUT, GPIO_DIR_MODE_IN
#define gpio_pad_dir(pad, dir)	MAP_GPIODirModeSet(gpio_pad_to_base(pad), 1UL << gpio_pad_to_pin(pad), dir);

// IO pad clear (LOW)
#define gpio_pad_clear(pad)	MAP_GPIOPinWrite(gpio_pad_to_base(pad), 1UL << gpio_pad_to_pin(pad), 0);

// IO pad set (HIGH)
#define gpio_pad_set(pad)	MAP_GPIOPinWrite(gpio_pad_to_base(pad), 1UL << gpio_pad_to_pin(pad), 1UL << gpio_pad_to_pin(pad));

#endif
