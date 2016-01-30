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
#include "common/escape.h"

#define LED	9

#if defined(ccs) || defined (gcc)
extern void (* const g_pfnVectors[])(void);
#endif

static char buffer[128];
static struct {
	_i16 role;
} status;

static void startDevice()
{
	uart0_write_string(ESC_YELLOW "Starting device...\n");
	status.role = sl_Start(NULL, NULL, NULL);
}

static unsigned long openWlan()
{
	long ret;
	uart0_write_string(ESC_YELLOW "Starting device...\n");
	if ((ret = sl_Start(NULL, NULL, NULL)) != ROLE_STA) {
		sprintf(buffer, ESC_YELLOW "Changing device role from %d to STA...\n", ret);
		if ((ret = sl_WlanSetMode(ROLE_STA)) != 0)
			return ret;
		uart0_write_string(ESC_YELLOW "Stopping device...\n");
		if ((ret = sl_Stop(0xff)) != 0)
			return ret;
		uart0_write_string(ESC_YELLOW "Starting device...\n");
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

static void printFileInfo(const char *path)
{
	SlFsFileInfo_t info;
	_i16 ret = sl_FsGetInfo(path, 0, &info);
	if (ret != 0) {
		uart0_write_string(ESC_GREY "Getting file info of ");
		uart0_write_string(path);
		uart0_write_string(" failed with: ");
		sprintf(buffer, "%d\n", ret);
		uart0_write_string(buffer);
		return;
	}

	sprintf(buffer, ESC_WHITE "Flags: 0x%04x\n", info.flags);
	uart0_write_string(buffer);
	sprintf(buffer, "Length: %lu\n", info.FileLen);
	uart0_write_string(buffer);
	sprintf(buffer, "Allocated length: %lu\n", info.AllocatedLen);
	uart0_write_string(buffer);
	uart0_write_string("Token: ");
	_u32 i;
	for (i = 0; i < 4; i++) {
		sprintf(buffer, "0x%08lx ", info.Token[i]);
		uart0_write_string(buffer);
	}
	uart0_write_string("\n");
}

// This function handles HTTP server events
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent, SlHttpServerResponse_t *pHttpResponse)
{
	// Unused in this application
}

// This function handles network events such as IP acquisition, IP leased, IP released etc.
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
	switch (pNetAppEvent->Event) {
	case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_NETAPP_IPV4_IPACQUIRED_EVENT\n");
		break;
	case SL_NETAPP_IP_LEASED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_NETAPP_IP_LEASED_EVENT\n");
		break;
	case SL_NETAPP_IP_RELEASED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_NETAPP_IP_RELEASED_EVENT\n");
		break;
	default:
		uart0_write_string(ESC_MAGENTA "SL_NETAPP_IP_UNKNOWN_EVENT\n");
		break;
	};
}

// This function handles socket events indication
void SimpleLinkSockEventHandler(SlSockEvent_t *pSockEvent)
{
	switch (pSockEvent->Event) {
	case SL_SOCKET_TX_FAILED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_SOCKET_TX_FAILED_EVENT");
		break;
	case SL_SOCKET_ASYNC_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_SOCKET_ASYNC_EVENT");
		break;
	default:
		uart0_write_string(ESC_MAGENTA "SL_SOCKET_UNKNOWN_EVENT");
		break;
	};
}

// The Function Handles WLAN Events
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
	switch (pWlanEvent->Event) {
#if 0
	case SL_DEVICE_FATAL_ERROR_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_DEVICE_FATAL_ERROR_EVENT");
		break;
#endif
	case SL_WLAN_CONNECT_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_CONNECT_EVENT");
		break;
	case SL_WLAN_DISCONNECT_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_DISCONNECT_EVENT");
		break;
	case SL_WLAN_STA_CONNECTED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_STA_CONNECTED_EVENT");
		break;
	case SL_WLAN_STA_DISCONNECTED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_STA_DISCONNECTED_EVENT");
		break;
	case SL_WLAN_SMART_CONFIG_COMPLETE_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_SMART_CONFIG_COMPLETE_EVENT");
		break;
	case SL_WLAN_SMART_CONFIG_STOP_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_SMART_CONFIG_STOP_EVENT");
		break;
	case SL_WLAN_P2P_DEV_FOUND_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_P2P_DEV_FOUND_EVENT");
		break;
	case SL_WLAN_P2P_NEG_REQ_RECEIVED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_P2P_NEG_REQ_RECEIVED_EVENT");
		break;
	case SL_WLAN_CONNECTION_FAILED_EVENT:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_CONNECTION_FAILED_EVENT");
		break;
	default:
		uart0_write_string(ESC_MAGENTA "SL_WLAN_UNKNOWN_EVENT");
		break;
	};
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

	startDevice();

start:
	printFileInfo("/sys/mcuimg.bin");
	printFileInfo("/sys/ca.pem");

	if ((ret = openWlan()) != 0)
		goto retry;

	uart0_write_string(ESC_YELLOW "Finished, stopping device...\n");
	sl_Stop(0xff);

	uart0_write_string(ESC_CYAN "Done.\n");
	uart0_readline(buffer, sizeof(buffer));
	goto start;

retry:
	sprintf(buffer, ESC_GREY "Failed: %d, stopping device and retry...\n", ret);
	uart0_write_string(buffer);
	sl_Stop(0xff);
	UtilsDelay(80000000 * 2);
	goto start;
}
