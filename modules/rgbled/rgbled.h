// Author: Yubo Zhi (normanzyb@gmail.com)

#ifndef _RGBLED_H
#define _RGBLED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t rgbLED[RGBLED_NUM];

void rgbLED_init();
void rgbLED_refresh();

#ifdef __cplusplus
}
#endif

#endif
