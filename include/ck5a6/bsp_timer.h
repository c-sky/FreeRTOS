#ifndef __FREERTOS_TIMER_H__
#define __FREERTOS_TIMER_H__
#include "bsp.h"

#define TIMER1      1
#define TIMER2      2
#define TIMER3      3


struct NODE_TMR
{
    volatile uint32_t cntr;
    volatile uint32_t cur;
    volatile uint32_t ctrl;
    volatile uint32_t eoi;
    volatile uint32_t intstat;
};

struct BSP_TMR
{
    struct NODE_TMR tmr[TIMER3];
};

#define CK_TIMER_BASE        (0x10011000)
#define BSPTIMERREG         ((struct BSP_TMR *)CK_TIMER_BASE)

void CKTimerInit(uint32_t timer_id, uint32_t freq);
void CKTimerClear (uint32_t timer_id);

#endif
