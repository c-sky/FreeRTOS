#include <bsp.h>
#include "bsp_intc.h"
#include "bsp_timer.h"
#include "bsp_uart.h"


void  BSP_Init (void)
{
   	CKIntcInit();
	
	CKSetIsrVector((void *)CKTaskSwitchIsr, VECTOR_TRAP0);
	CKSetIsrVector((void *)CKTickIsr, VECTOR_TIMER);
	CKSetIsrVector((void *)CKUart0Isr, VECTOR_UART0);

    	CKTimerInit(TIMER1, OS_TICK_FREQ);
   	CKUartInit(UART_BAUD, 8, 1, LCR_PARITY_NONE);

}


