#ifndef TIMER_IF_H
#define TIMER_IF_H

#include "debug.h"
#include "hw_timer.h"

// Check if timer is enabled
static inline tBoolean timer_enabled(unsigned long ulBase, unsigned long ulTimer)
{
    // Check the arguments.
    ASSERT(TimerBaseValid(ulBase));
    ASSERT((ulTimer == TIMER_A) || (ulTimer == TIMER_B) ||
           (ulTimer == TIMER_BOTH));

    // Check timer enable status
    return HWREG(ulBase + TIMER_O_CTL) & ulTimer & (TIMER_CTL_TAEN | TIMER_CTL_TBEN);
}

#endif
