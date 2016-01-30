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
#include "flash.h"
#include "utils.h"

#include "simplelink.h"

// Standard includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Local includes
#include "uart0/uart0.h"
#include "common/gpio_if.h"
#include "common/escape.h"
#include "global.h"
#include "fs.h"
#include "device.h"
#include "wlan.h"

#define LED	9

#if defined(ccs) || defined (gcc)
extern void (* const g_pfnVectors[])(void);
#endif

char buffer[128];
struct status_t status;

static void init()
{
#if defined(ccs) || defined (gcc)
	MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif

	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();

	MAP_PRCMPeripheralClkEnable(gpio_pad_to_prcm(LED), PRCM_RUN_MODE_CLK);
	gpio_pad_init(LED);
	gpio_pad_dir(LED, GPIO_DIR_MODE_OUT);

	uart0_init();
	uart0_write_string(ESC_DEFAULT ESC_FF ESC_CYAN "CC3200 demo @ " __DATE__ " " __TIME__ "\n");
	uart0_write_string("Initialised.\n");
}

int main()
{
	init();

	uart0_write_string(ESC_YELLOW "Starting device...\n");
	startDevice();

start:
	uart0_write_string(ESC_DEFAULT "> ");
	uart0_readline(buffer, sizeof(buffer));
	if (strncmp(buffer, "fs-", 3) == 0) {
		char *cmd = buffer + 3;
		if (strncmp(cmd, "info ", 5) == 0) {
			cmd += 5;
			while (*cmd == ' ')
				cmd++;
			if (isgraph(*cmd))
				printFileInfo(cmd);
			//printFileInfo("/sys/mcuimg.bin");
			//printFileInfo("/sys/ca.pem");
		} else if (strncmp(cmd, "write", 5) == 0) {
			cmd += 5;
			while (*cmd == ' ')
				cmd++;
			if (isgraph(*cmd))
				receiveFile(cmd);
		}
	} else if (strncmp(buffer, "dev-", 4) == 0) {
		char *cmd = buffer + 4;
		if (strcmp(cmd, "stop") == 0) {
			uart0_write_string(ESC_YELLOW "Stopping device...\n");
			stopDevice();
		} else if (strcmp(cmd, "start") == 0) {
			uart0_write_string(ESC_YELLOW "Starting device... ");
			sprintf(buffer, "%d\n", startDevice());
			uart0_write_string(buffer);
		} else if (strcmp(cmd, "restart") == 0) {
			uart0_write_string(ESC_YELLOW "Restarting device... ");
			stopDevice();
			sprintf(buffer, "%d\n", startDevice());
			uart0_write_string(buffer);
		}
	} else if (strncmp(buffer, "wlan-", 5) == 0) {
		char *cmd = buffer + 5;
		if (strcmp(cmd, "scan") == 0)
			printWlanScan();
	}
	goto start;
}
