// Author: Yubo Zhi (normanzyb@gmail.com)
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "rom_map.h"
#include "interrupt.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"
#include "utils.h"

#include <gpio_if.h>
#include <colours.h>
#include "rgbled.h"

uint32_t rgbLED[RGBLED_NUM] = {
	0x00ff0000, 0x0000ff00, 0x000000ff,
	0x00ffff00, 0x00ff00ff, 0x0000ffff,
	0x00ffffff,
	0x00000000,
};

static const unsigned long base = gpio_pad_to_base(RGBLED_PAD);
static const unsigned long pin = 1UL << gpio_pad_to_pin(RGBLED_PAD);

static void send(uint8_t data)
{
	uint8_t cnt = 8;
	do {
		gpio_pad_set(RGBLED_PAD);
		if (!(data & 0x80))
			gpio_pad_clear(RGBLED_PAD);
		MAP_UtilsDelay(F_CPU / 10 * 4 / 10 / 1000 / 1000);
		gpio_pad_clear(RGBLED_PAD);
		MAP_UtilsDelay(F_CPU / 10 * 4 / 10 / 1000 / 1000);
		data <<= 1;
	} while (--cnt);
}

void rgbLED_init()
{
	MAP_PRCMPeripheralClkEnable(gpio_pad_to_prcm(RGBLED_PAD), PRCM_RUN_MODE_CLK);
	gpio_pad_init(RGBLED_PAD);
	gpio_pad_dir(RGBLED_PAD, GPIO_DIR_MODE_OUT);
	gpio_pad_clear(RGBLED_PAD);

	MAP_UtilsDelay(F_CPU / 10 * 10 / 1000);
}

void rgbLED_refresh()
{
	uint8_t cnt = 8;
	uint32_t *p = &rgbLED[0];
	// Time sensitive
	MAP_IntMasterDisable();
	do {
		uint32_t clr = *p++;
		send(GREEN_888(clr));
		send(RED_888(clr));
		send(BLUE_888(clr));
	} while (--cnt);
	MAP_IntMasterEnable();
}
