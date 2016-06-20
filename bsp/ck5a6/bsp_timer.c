#include "bsp.h"

void CKTimerInit(uint32_t timer_id, uint32_t freq)
{
    BSPTIMERREG->tmr[timer_id-1].cntr = AHB_DEFAULT_FREQ/freq-1;
    BSPTIMERREG->tmr[timer_id-1].ctrl = 0x3;
}

void CKTimerClear(uint32_t timer_id)
{
    (void)BSPTIMERREG->tmr[timer_id-1].eoi;
}
