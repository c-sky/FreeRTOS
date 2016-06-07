#include "bsp_timer.h"
#include "bsp.h"


void CKTimerInit(uint32_t timer_id, uint32_t freq) 
{
	uint32_t cnt_value;
	uint32_t *CK_Timer_Base;
	
	cnt_value = AHB_DEFAULT_FREQ/freq-1;
	CK_Timer_Base = (uint32_t *)(CK_TIMER_BASE+(timer_id-1)*5); //calculate timer base address
	
	CK_Timer_Base[TMR_CNTR_REG] = cnt_value;
	CK_Timer_Base[TMR_CTRL_REG] = 0x3;//timer int not masked , user-defined running,timer enable
	
}

void CKTimerClear(uint32_t timer_id)
{
    	volatile uint32_t tmp;
	uint32_t *CK_Timer_Base;

	CK_Timer_Base = (uint32_t *)(CK_TIMER_BASE+(timer_id-1)*5); //calculate timer base address

	tmp = CK_Timer_Base[TMR_EOI_REG];      // Read timer EOI register
}


