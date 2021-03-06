// Standard includes
#include <stdio.h>

// Driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "hw_apps_rcm.h"
#include "prcm.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "gpio.h"
#include "utils.h"

// Local includes
#include "pin_mux_config.h"
#include "uart0.h"

#define LED	30
#define DELAY	(SYS_CLK / 10)

static void LEDBlinkyRoutine()
{
	// The LED is iverted
	MAP_GPIOPinWrite(GPIOA3_BASE, 1UL << (LED % 8), 1UL << (LED % 8));
	for (;;) {
		MAP_UtilsDelay(DELAY);
		MAP_GPIOPinWrite(GPIOA3_BASE, 1UL << (LED % 8), 0);
		uart0_write_string("Hello, world! LED is now ON.\n");
		MAP_UtilsDelay(DELAY);
		MAP_GPIOPinWrite(GPIOA3_BASE, 1UL << (LED % 8), 1UL << (LED % 8));
		uart0_write_string("Hello, world! LED is now OFF.\n");
	}

}

static void BoardInit()
{
	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();

	uart0_init();
}

int main()
{
	BoardInit();
	// https://dev.ti.com/pinmux/app.html
	PinMuxConfig();

	LEDBlinkyRoutine();
	return 0;
}
