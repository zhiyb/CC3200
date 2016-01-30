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

// Local includes
#include "uart0/uart0.h"
#include "common/gpio_if.h"

#define LED	9

static char buffer[128];

//*****************************************************************************
//
//! \brief This function handles HTTP server events
//!
//! \param[in]  pServerEvent - Contains the relevant event information
//! \param[in]    pServerResponse - Should be filled by the user with the
//!                                      relevant response information
//!
//! \return None
//!
//****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse)
{
	// Unused in this application
}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
	;
}

//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
	;
}

//*****************************************************************************
//
//! \brief The Function Handles WLAN Events
//!
//! \param[in]  pWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
	;
}

extern void (* const g_pfnVectors[])(void);
static void init()
{
	MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);

	MAP_IntMasterEnable();
	MAP_IntEnable(FAULT_SYSTICK);

	PRCMCC3200MCUInit();

	MAP_PRCMPeripheralClkEnable(gpio_pad_to_prcm(LED), PRCM_RUN_MODE_CLK);
	gpio_pad_init(LED);
	gpio_pad_dir(LED, GPIO_DIR_MODE_OUT);

	uart0_init();
	uart0_write_string("Initialised.\n");
}

static unsigned long openWlan()
{
	long ret;
	uart0_write_string("Starting device...\n");
	if ((ret = sl_Start(NULL, NULL, NULL)) != ROLE_STA) {
		sprintf(buffer, "Changing device role from %d to STA...\n", ret);
		if ((ret = sl_WlanSetMode(ROLE_STA)) != 0)
			return ret;
		uart0_write_string("Stopping device...\n");
		if ((ret = sl_Stop(0xff)) != 0)
			return ret;
		uart0_write_string("Starting device...\n");
		if ((ret = sl_Start(NULL, NULL, NULL)) != 0)
			return ret;
	}

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

	uart0_write_string("Reset policy.\n");
	if ((ret = sl_WlanPolicySet(SL_POLICY_CONNECTION, SL_CONNECTION_POLICY(0, 0, 0, 0, 0), NULL, 0)) != 0)
		return ret;

	uart0_write_string("Disconnect wlan...\n");
	sl_WlanDisconnect();

	uart0_write_string("Enable DHCP client.\n");
	unsigned char ucVal = 1;
	if ((ret = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal)) != 0)
		return ret;

	return 0;
}

int main()
{
	long ret;
	init();

start:
	if ((ret = openWlan()) != 0)
		goto retry;

	uart0_write_string("Finished, stopping device...\n");
	sl_Stop(0xff);

	uart0_write_string("Done.\n");
	uart0_readline(buffer, sizeof(buffer));
	goto start;

retry:
	sprintf(buffer, "Failed: %d, stopping device and retry...\n", ret);
	uart0_write_string(buffer);
	sl_Stop(0xff);
	UtilsDelay(80000000 * 2);
	goto start;
}
