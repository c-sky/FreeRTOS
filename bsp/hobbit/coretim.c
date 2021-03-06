/*
 *  Copyright (C) 2016 YunOS Project. All rights reserved.
 */

#include <coretim.h>
#include <chip.h>

static inline uint32_t readreg32(volatile uint32_t *addr)
{
    return *(volatile uint32_t*)addr;
}

static inline void writereg32(uint32_t val, volatile uint32_t *addr)
{
    *(volatile uint32_t*)addr = val;
}

/**********************************************************************
 * Function:        coretim_init
 * Description:     Initial timer interrupt status bit.
 * Parameters:      freq --- timer counting value.
 * Returns:         void
 **********************************************************************/

void coretim_init(uint32_t hz)
{
    uint32_t reg;

    writereg32(APB_DEFAULT_FREQ/hz, CORET_RVR);
    writereg32(0, CORET_CVR);

    reg = readreg32(CORET_CSR);
    reg |= CORETIM_TXCONTROL_ENABLE;
    reg |= CORETIM_TXCONTROL_INTMASK;
    writereg32(reg, CORET_CSR);

    return;
}

/**********************************************************************
 * Function:        coretim_clear
 * Description:     Clear timer interrupt status bit.
 * Parameters:      void
 * Returns:         void
 **********************************************************************/
void coretim_clr_irq(void)
{
    uint32_t reg;

    reg = readreg32(CORET_CSR);
    reg |= ~CORETIM_TXCONTROL_MODE;
    writereg32(reg, CORET_CSR);

    /* FIXME: clear active */
    writereg32(0, (volatile uint32_t *)0xE000E300);
}

/**********************************************************************
 * Function:        coretim_current_value
 * Description:     Get Current Value.
 * Parameters:      void
 * Returns:         uint32
 **********************************************************************/

uint32_t coretim_get_currval(void)
{
    return readreg32(CORET_CVR);
}
