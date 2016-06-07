#ifndef __FREERTOS_TIMER_H
#define __FREERTOS_TIMER_H
#include <stdint.h>

#define TIMER1      1
#define TIMER2      2
#define TIMER3      3


#define CK_TIMER_BASE        ((volatile uint32_t *)(0x10011000))

#define TMR_CNTR_REG      0x00
#define TMR_CUR_VAL       0x01
#define TMR_CTRL_REG      0x02
#define TMR_EOI_REG       0x03
#define TMR_INT_STAT      0x04


void CKTimerInit (uint32_t timer_id, uint32_t freq);
void CKTimerClear (uint32_t timer_id);



#endif 
	
