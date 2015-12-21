// Author: Yubo Zhi (normanzyb@gmail.com)

#ifndef UART0_H
#define UART0_H

#ifdef __cplusplus
extern "C" {
#endif

#include "hw_types.h"
#include "rom_map.h"
#include "hw_memmap.h"
#include "uart.h"

// Initialise UART 0
void uart0_init();

// If characters available for read
#define uart0_available()	MAP_UARTCharsAvail(UARTA0_BASE)

// If UART tx buffer is available
#define uart0_ready()	MAP_UARTSpaceAvail(UARTA0_BASE)

// Read 1 character, unblocking
// Return false if not available
#define uart0_read_unblocked()	MAP_UARTCharGetNonBlocking(UARTA0_BASE)

// Read 1 character, blocking
#define uart0_read()	MAP_UARTCharGet(UARTA0_BASE)

// Write 1 character, unblocking
// Return false if space not available
#define uart0_write_unblocked(c)	MAP_UARTCharPutNonBlocking(UARTA0_BASE, (c))

// Write 1 character, blocking
#define uart0_write(c)	MAP_UARTCharPut(UARTA0_BASE, c)

// Write data
void uart0_write_data(char *ptr, unsigned long length);

// Write string
void uart0_write_string(char *str);

#ifdef __cplusplus
}
#endif

#endif
