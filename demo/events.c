#include "simplelink.h"

#include <uart0.h>
#include <escape.h>

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
