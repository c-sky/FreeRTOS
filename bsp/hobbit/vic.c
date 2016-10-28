/*
 *  Copyright (C) 2016 YunOS Project. All rights reserved.
 */

#include <vic.h>
#include <stdint.h>
#include "portmacro.h"

void vic_enable_irq(int irq)
{
    uint32_t flags;

    flags = portSET_INTERRUPT_MASK_FROM_ISR();
    *(VIC_ISER) |= (1 << (irq - 32));
    *(VIC_ISNR) |= (1 << (irq - 32));
    *(VIC_ISFR) &= ~(1 << (irq - 32));
    portCLEAR_INTERRUPT_MASK_FROM_ISR(flags);
}

/***********************************************************************
Function:       CKIntcInit

Description:    Set interrupt controller control register.

Parameters:     None.

Returns:        None.
***********************************************************************/
void vic_init(void)
{
    int i;
    *(VIC_ICR) = 0;
    for (i = 0; i < 56; i++) {
        VIC_PR[i] = 0x0;
    }

    return;
}
