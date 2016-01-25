// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "hw_apps_rcm.h"
#include "interrupt.h"
#include "pin.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Standard includes
#include <stdio.h>

// Local includes
#include <uart0.h>
#include <rgbled.h>
#include <colours.h>
#include <gpio_if.h>

#define LED	30
#define DELAY	256

static inline void demo()
{
	// Initialise
	unsigned long led = 0;
	unsigned long i = 0;
	uint8_t h[RGBLED_NUM] = {0};
	uint8_t s = 255;
	uint8_t v = 127;

	i = RGBLED_NUM;
	while (i--)
		h[i] = (uint16_t)255 * i / RGBLED_NUM;

	for (;;) {
		// RGB LED
		i = RGBLED_NUM;
		while (i--)
			rgbLED[i] = colour_hsv_to_rgb(COLOUR_888(h[i]++, s, v));

		// RGB LED refresh
		rgbLED_refresh();

		// LED & UART
		if (led >= DELAY) {
			gpio_pad_set(LED);
			if (led == DELAY)
				uart0_write_string("Hello, world! LED is now OFF.\n");
		} else {
			gpio_pad_clear(LED);
			if (led == 0)
				uart0_write_string("Hello, world! LED is now ON.\n");
		}
		led = (led + 1) % (DELAY << 1);

		MAP_UtilsDelay(SYS_CLK / 10 / DELAY);
	}
}

static inline void BoardInit()
{
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();

	MAP_PRCMPeripheralClkEnable(gpio_pad_to_prcm(LED), PRCM_RUN_MODE_CLK);
	gpio_pad_init(LED);
	gpio_pad_dir(LED, GPIO_DIR_MODE_OUT);

	uart0_init();
	rgbLED_init();
	rgbLED_refresh();
}

int main()
{
	BoardInit();

	demo();
	return 0;
}
