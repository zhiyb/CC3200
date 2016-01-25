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

//#include <simplelink.h>

// Standard includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// Local includes
#include <uart0.h>
#include <rgbled.h>
#include <colours.h>
#include <gpio_if.h>

#define LED	9

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

	uart0_write_string("Started.\n");
}

int main()
{
	static char buffer[64] = "Initial data";
	unsigned long i;

	BoardInit();

start:
	if ((i = uart0_readline(buffer, 64)) == 0)
		goto start;
	if (strncmp(buffer, "LED-ON", i) == 0)
		gpio_pad_set(LED);
	else if (strncmp(buffer, "LED-OFF", i) == 0)
		gpio_pad_clear(LED);
	goto start;

	return 0;
}
