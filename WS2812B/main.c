// Standard includes
#include <stdio.h>

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

// Local includes
#include <uart0.h>
#include <rgbled.h>
#include <gpio_if.h>

#define LED	30
#define DELAY	(F_CPU / 10)

static inline void LEDBlinkyRoutine()
{
	unsigned long i = 20 / 2;	// 20s
	// The LED is iverted
	while (i--) {
		gpio_pad_clear(LED);
		uart0_write_string("Hello, world! LED is now ON.\n");
		MAP_UtilsDelay(DELAY);

		gpio_pad_set(LED)
		uart0_write_string("Hello, world! LED is now OFF.\n");
		MAP_UtilsDelay(DELAY);
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

	//LEDBlinkyRoutine();
	for (;;) {
		rgbLED_refresh();
		MAP_UtilsDelay(DELAY);
		uint8_t i = RGBLED_NUM - 1;
		while (i--) {
			uint32_t tmp = rgbLED[i];
			rgbLED[i] = rgbLED[(i + 1) % RGBLED_NUM];
			rgbLED[(i + 1) % RGBLED_NUM] = tmp;
		}
	}
	return 0;
}
