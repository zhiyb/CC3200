#include "utils.h"

#include "simplelink.h"

// Standard includes
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

// Local includes
#include "uart0/uart0.h"
#include "common/escape.h"
#include "global.h"
#include "device.h"

_i16 openWlan()
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
	uart0_write_string(ESC_YELLOW "Enabling system scan...\n");
	_u32 intervalInSeconds = 600;
	sl_WlanPolicySet(SL_POLICY_SCAN, 1, (_u8 *)&intervalInSeconds, sizeof(intervalInSeconds));
	UtilsDelay(SYS_CLK / 2);

	uart0_write_string("Receiving network list...\n");
	Sl_WlanNetworkEntry_t netEntries[20];
	_i16 resultsCount = sl_WlanGetNetworkList(0, 20, netEntries);
	sprintf(buffer, ESC_WHITE "%d", resultsCount);
	uart0_write_string(buffer);
	uart0_write_string(" networks found.\n");

	_u16 i;
	for (i = 0; i < resultsCount; i++) {
		Sl_WlanNetworkEntry_t *entry = netEntries + i;
		sprintf(buffer, ESC_WHITE "\nNetwork %u:\n" ESC_GREY, i);
		uart0_write_string(buffer);
		uart0_write_string("SSID:\t\t");
		uart0_write_data(entry->ssid, entry->ssid_len);
		uart0_write_string("\n");
		sprintf(buffer, "Security:\t%u\n", entry->sec_type);
		uart0_write_string(buffer);
		uart0_write_string("BSSID:\t\t");
		_u16 i;
		for (i = 0; i < SL_BSSID_LENGTH; i++) {
			sprintf(buffer, "%02x%c", entry->bssid[i], i == SL_BSSID_LENGTH - 1 ? '\n' : ':');
			uart0_write_string(buffer);
		}
		sprintf(buffer, "rssi:\t\t%d\n", entry->rssi);
		uart0_write_string(buffer);
	}
}
