#include "bsp.h"

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function
 * 			void CKTimerInit(uint32_t timer_id, uint32_t freq)
 * Description
 * 			Initialize timer 
 * Parameter
 * 			timer_id				timer id
 * 			freq					time period 
 * Return
 * 			None
 ****************************************************************************/
void CKTimerInit(uint32_t timer_id, uint32_t freq)
{
    BSPTIMERREG->tmr[timer_id-1].cntr = AHB_DEFAULT_FREQ/freq-1;
    BSPTIMERREG->tmr[timer_id-1].ctrl = 0x3;
}

/****************************************************************************
 * Function
 * 			void CKTimerClear(uint32_t timer_id)
 * Description
 * 			clear timer interrupt flag
 * Parameter
 * 			timer_id				timer id
 * Return
 * 			None
 ****************************************************************************/
void CKTimerClear(uint32_t timer_id)
{
    (void)BSPTIMERREG->tmr[timer_id-1].eoi;
}
