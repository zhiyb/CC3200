#include "rom_map.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "prcm.h"
#include "uart.h"
#include "uart0.h"

#include <ctype.h>

void uart0_init()
{
	// Enable Peripheral Clocks
	MAP_PRCMPeripheralClkEnable(PRCM_UARTA0, PRCM_RUN_MODE_CLK);
	// Configure PIN_55 for UART0 UART0_TX
	MAP_PinTypeUART(PIN_55, PIN_MODE_3);
	// Configure PIN_57 for UART0 UART0_RX
	MAP_PinTypeUART(PIN_57, PIN_MODE_3);

	MAP_UARTDisable(UARTA0_BASE);

	MAP_UARTConfigSetExpClk(UARTA0_BASE, MAP_PRCMPeripheralClockGet(PRCM_UARTA0), BAUD, UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
	//MAP_UARTDMADisable(UARTA0_BASE, UART_DMA_RX | UART_DMA_TX);
	//MAP_UARTTxIntModeSet(UARTA0_BASE, UART_TXINT_MODE_FIFO);
	//MAP_UARTIntDisable(UARTA0_BASE, UART_INT_OE | UART_INT_BE | UART_INT_PE | UART_INT_FE | UART_INT_RT | UART_INT_TX | UART_INT_RX | UART_INT_CTS);

	MAP_UARTFIFODisable(UARTA0_BASE);
	MAP_UARTFIFOLevelSet(UARTA0_BASE, UART_FIFO_TX4_8, UART_FIFO_RX4_8);
	MAP_UARTFlowControlSet(UARTA0_BASE, UART_FLOWCONTROL_NONE);
	MAP_UARTFIFOEnable(UARTA0_BASE);

	MAP_UARTEnable(UARTA0_BASE);
}

void uart0_write_data(char *ptr, unsigned long length)
{
	while (length--)
		uart0_write(*ptr++);
}

void uart0_write_string(char *str)
{
	while (*str != '\0') {
		if (*str == '\n')
			uart0_write('\r');
		uart0_write(*str++);
	}
}

unsigned long uart0_readline(char *buffer, unsigned long length)
{
	unsigned long cnt = 0;
	long c;
	length--;

loop:
	switch (c = uart0_read()) {
	case '\n':
	case '\r':
		uart0_write('\r');
		uart0_write('\n');
		*buffer = '\0';
		return cnt;
	case '\x7f':	// Backspace
		if (cnt) {
			uart0_write(c);
			buffer--;
			cnt--;
		}
		break;
	default:
		if (!isprint(c) || cnt == length)
			break;
		uart0_write(c);
		*buffer = c;
		buffer++;
		cnt++;
	}

	goto loop;
}
