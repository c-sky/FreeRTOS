#include "bsp.h"


/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function
 * 			void CKEnableInterrupt(uint32_t irq)
 * Description
 * 			Enable interrupt
 * Parameter
 * 			irq				irq number
 * Return
 * 			None
 ****************************************************************************/
void CKEnableInterrupt(uint32_t irq)
{
    if (irq >= 32)
    {
        BSPINTCREG->nier |= (1 << (irq - 32));
    }
}

/****************************************************************************
 * Function
 * 			void CKIntcInit (void)
 * Description
 * 			initialize interrupt controller
 * Parameter
 * 			None
 * Return
 * 			None
 ****************************************************************************/
void CKIntcInit(void)
{
    BSPINTCREG->icr = 0;
    BSPINTCREG->ifr = 0;
    BSPINTCREG->pr0 = 0x00010203;
    BSPINTCREG->pr1 = 0x04050607;
    BSPINTCREG->pr2 = 0x08090a0b;
    BSPINTCREG->pr3 = 0x0c0d0e0f;
    BSPINTCREG->pr4 = 0x10111213;
    BSPINTCREG->pr5 = 0x14151617;
    BSPINTCREG->pr6 = 0x18191a1b;
    BSPINTCREG->pr7 = 0x1c1d1e1f;
}
