#ifndef __FREERTOS_INTERRUPT_H
#define __FREERTOS_INTERRUPT_H

#include "bsp.h"


#define VECTOR_RESET            0
#define VECTOR_MISALIGN         1
#define VECTOR_ACCESS           2
#define VECTOR_DIV_BY_ZERO      3
#define VECTOR_ILLEGAL          4
#define VECTOR_PRIVILEGE        5
#define VECTOR_TRACE            6
#define VECTOR_BREAK            7
#define VECTOR_UNRECOV          8
#define VECTOR_SOFT             9
#define VECTOR_INT              10               /* Used by PIT timer only for OS tick */
#define VECTOR_FINT             11               /* All other controller-based interrupts
                                                    use fast interrupt (FINT) and alternate regs */
#define VECTOR_HW_ACCEL         12

#define VECTOR_TRAP0            16
#define VECTOR_TRAP1            17
#define VECTOR_TRAP2            18
#define VECTOR_TRAP3            19


#define VECTOR_UART1            48
#define VECTOR_TIMER1           44
#define VECTOR_MAC              58

struct BSP_INT
{
    volatile uint32_t icr;
    volatile uint32_t rsvr0;
    volatile uint32_t ifr;
    volatile uint32_t rsvr1;
    volatile uint32_t nier;
    volatile uint32_t rsvr2;
    volatile uint32_t fier;
    volatile uint32_t rsvr3;
    volatile uint32_t rsvr[8];
    volatile uint32_t pr0;
    volatile uint32_t pr1;
    volatile uint32_t pr2;
    volatile uint32_t pr3;
    volatile uint32_t pr4;
    volatile uint32_t pr5;
    volatile uint32_t pr6;
    volatile uint32_t pr7;
};
#define CK_INTC_BASE                (0x10010000)
#define BSPINTCREG                  ((struct BSP_INT *)CK_INTC_BASE)

void CKIntcInit(void);
void CKEnableInterrupt(uint32_t irq);

#endif
