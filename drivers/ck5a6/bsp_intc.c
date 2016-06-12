#include <bsp_intc.h>

void CKSetIsrVector(void *pIsr, uint32_t iVectorNumber)
{
	if (pIsr != 0)
	{
		/* Set interrupt vector for interrupt handler */
		CKSetVector(pIsr, iVectorNumber);
		/* Enable appropriate bit in interrupt controller if necessary */
		if (iVectorNumber >= 32)
		{
			*(INTC_NIER) |= (1 << (iVectorNumber - 32)); 
		}
	}
}


void CKIntcInit(void)
{
	*(INTC_ICR) = 0;
  	*(INTC_IFR) = 0;
  	*(INTC_PR0) = 0x00010203;
  	*(INTC_PR1) = 0x04050607;
  	*(INTC_PR2) = 0x08090a0b;
  	*(INTC_PR3) = 0x0c0d0e0f;
  	*(INTC_PR4) = 0x10111213;
  	*(INTC_PR5) = 0x14151617;
  	*(INTC_PR6) = 0x18191a1b;
  	*(INTC_PR7) = 0x1c1d1e1f;

  	return;
}


