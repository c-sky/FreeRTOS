/*
 */

 #include "board.h"

extern void hobbit_ioreuse_initial(void);

/*****************************************************************************
 * Private types/enumerations/variables
 ****************************************************************************/

/* Set up and initialize hardware prior to call to main */
void Board_SystemInit(void)
{
    hobbit_ioreuse_initial();
}
