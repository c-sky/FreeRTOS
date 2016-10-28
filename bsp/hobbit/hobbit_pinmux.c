/****************************************************************************
 *
 * Copyright (C) 2016 The YunOS Open Source Project
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>

#include "hobbit_pinmux.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>


#define readl(addr) \
    ({ unsigned int __v = (*(volatile unsigned int *) (addr)); __v; })

#define writel(b,addr) (void)((*(volatile unsigned int *) (addr)) = (b))


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: hobbit_ioreuse_inital
 *
 * Description:
 *   initial hobbit_pinmux
 *
 ****************************************************************************/
void hobbit_ioreuse_initial(void)
{
    unsigned int value;

    /*gpio data source select*/
    value = readl(HOBBIT_GIPO0_PORTCTL_REG);
    value |= GPIO0_REUSE_EN;
    writel(value, HOBBIT_GIPO0_PORTCTL_REG);

    value = readl(HOBBIT_GIPO1_PORTCTL_REG);
    value |= GPIO1_REUSE_EN;
    writel(value, HOBBIT_GIPO1_PORTCTL_REG);

    /*reuse function select*/
    value = readl(HOBBIT_IOMUX0L_REG);
    value |= IOMUX0L_FUNCTION_SEL;
    writel(value, HOBBIT_IOMUX0L_REG);

    value = readl(HOBBIT_IOMUX0H_REG);
    value |= IOMUX1L_FUNCTION_SEL;
    writel(value, HOBBIT_IOMUX0H_REG);

    value = readl(HOBBIT_IOMUX1L_REG);
    value |= IOMUX1L_FUNCTION_SEL;
    writel(value, HOBBIT_IOMUX1L_REG);

    writel(PA5_ETB_TRIG1, HOBBIT_GIPO0_PORTCTL_REG);
}

