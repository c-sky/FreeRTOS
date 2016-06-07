#ifndef __FREERTOS_INTERRUPT_H
#define __FREERTOS_INTERRUPT_H

#include "bsp.h"


#define VECTOR_RESET            	0
#define VECTOR_MISALIGN         	1
#define VECTOR_ACCESS           	2
#define VECTOR_DIV_BY_ZERO      3
#define VECTOR_ILLEGAL          	4
#define VECTOR_PRIVILEGE        	5
#define VECTOR_TRACE            	6
#define VECTOR_BREAK            	7
#define VECTOR_UNRECOV          	8
#define VECTOR_SOFT             		9
#define VECTOR_INT              		10               /* Used by PIT timer only for OS tick */
#define VECTOR_FINT             		11               /* All other controller-based interrupts
                                                    use fast interrupt (FINT) and alternate regs */
#define VECTOR_HW_ACCEL         12

#define VECTOR_TRAP0            16
#define VECTOR_TRAP1            17
#define VECTOR_TRAP2            18
#define VECTOR_TRAP3            19


#define VECTOR_UART0            48
#define VECTOR_TIMER            44
#define VECTOR_MAC              58

/* INT_CONTROLLER memory locations */

#define INTC_BASE               ((volatile uint32_t *) 0x10010000)

#define INTC_ICR                ((volatile uint32_t *) 0x10010000)
#define INTC_IFR                ((volatile uint32_t *) 0x10010008)
#define INTC_NIER               ((volatile uint32_t *) 0x10010010)
#define INTC_FIER               ((volatile uint32_t *) 0x10010018)
#define INTC_PR0                ((volatile uint32_t *) 0x10010040)
#define INTC_PR1                ((volatile uint32_t *) 0x10010044)
#define INTC_PR2                ((volatile uint32_t *) 0x10010048)
#define INTC_PR3                ((volatile uint32_t *) 0x1001004c)
#define INTC_PR4                ((volatile uint32_t *) 0x10010050)
#define INTC_PR5                ((volatile uint32_t *) 0x10010054)
#define INTC_PR6                ((volatile uint32_t *) 0x10010058)
#define INTC_PR7                ((volatile uint32_t *) 0x1001005c)

/*****************************************************************************/

/* Function Prototypes */
void CKSetIsrVector(void *pIsr, uint32_t iVectorNumber);
void CKIntcInit(void);
void CKSetVector(void *pIsr, uint32_t iVectorNumber);

void CKTaskSwitchIsr(void);
void CKTickIsr(void);
void CKUart0Isr(void);
void CKEnableIntFromIsr(void);

#endif //__BSP_INTERRUPT_H
