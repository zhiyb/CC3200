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

#define LED	9

#if defined(ccs) || defined (gcc)
extern void (* const g_pfnVectors[])(void);
#endif

char buffer[128];
struct status_t status;

static _i16 openWlan()
{
	if (setDeviceRole(ROLE_STA) != ROLE_STA) {
		uart0_write_string(ESC_GREY "Unable to change device role: ");
		sprintf(buffer, "%d\n", status.role);
		uart0_write_string(buffer);
		return -1;
	}

	_i16 ret;
#if 0
	// Get the device's version-information
	SlVersionFull ver = {0};
	unsigned char ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
	unsigned char ucConfigLen = sizeof(ver);
	if ((ret = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt, &ucConfigLen, (unsigned char *)(&ver))) != 0)
		return ret;
	sprintf(buffer, "Host Driver Version: %s\n\r", SL_DRIVER_VERSION);
	uart0_write_string(buffer);
	sprintf(buffer, "Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r",
		ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
			ver.ChipFwAndPhyVersion.FwVersion[0],ver.ChipFwAndPhyVersion.FwVersion[1],
			ver.ChipFwAndPhyVersion.FwVersion[2],ver.ChipFwAndPhyVersion.FwVersion[3],
			ver.ChipFwAndPhyVersion.PhyVersion[0],ver.ChipFwAndPhyVersion.PhyVersion[1],
			ver.ChipFwAndPhyVersion.PhyVersion[2],ver.ChipFwAndPhyVersion.PhyVersion[3]);
	uart0_write_string(buffer);
#endif

	uart0_write_string(ESC_YELLOW "Reset policy.\n");
	if ((ret = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0)) != 0)
		return ret;

	uart0_write_string(ESC_YELLOW "Disconnect wlan...\n");
	sl_WlanDisconnect();

	uart0_write_string(ESC_YELLOW "Enable DHCP client.\n");
	unsigned char ucVal = 1;
	if ((ret = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal)) != 0)
		return ret;

	return 0;
}

void printWlanScan()
{
}

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
	long ret;
	init();

	uart0_write_string(ESC_YELLOW "Starting device...\n");
	startDevice();

start:
	uart0_write_string(ESC_WHITE "> " ESC_DEFAULT);
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
