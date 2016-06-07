#include "bsp_uart.h"
#include "lib_print.h"
#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


static void CKUartTrasmitEmpty(void)
{
	while ((UART[UART_USR] & USR_TFE) == 0) ;
}


uint32_t CKUartInit(	uint32_t ulBaudRate,
	    				uint32_t ulDataBits, 
	    			 	uint32_t ulStopBits,
	   			 	uint32_t ulParity)
{
	/* NOTE: Could perform parameter checking here */
	uint32_t uDivisor;
	
	/* Calculate the divisor value by dividing the crystal frequency
	   by the baud rate, correcting for the remainder if necessary
	   and dividing the result by 16 */
	uDivisor = ((UART_FREQ / ulBaudRate) >> UART_DIV);
	/* Set the baud rate generator divisor */
	UART[UART_LCR] |= LCR_SEL_DLR;
	UART[UART_DLL] = uDivisor & 0xff;
	UART[UART_DLH] = (uDivisor >> 8) & 0xff;	
	
	
	/* CLR the DLAB bit of LSR */
	UART[UART_LCR] &= (~LCR_SEL_DLR); 
	
	/* Set FIFO Control Register*/	
	UART[UART_FCR] = FCR_BYTE_1;
	
	/* Enable Data Receiver Ready Interrupt */	
	UART[UART_IER] |= IER_RDAR_INT_ENABLE;
	
	/* Set the number of data bits (character length) */
	if (ulDataBits == 8)
		UART[UART_LCR] |= LCR_WORD_SIZE_8;
	
	/* Set the stop bit length */
	if (ulStopBits != 1)
		UART[UART_LCR] |= LCR_STOP_BIT_2;
    else
		UART[UART_LCR] &= LCR_STOP_BIT_1;

	/* Set the parity mode */
	if (ulParity == LCR_PARITY_NONE)
		UART[UART_LCR] &= (~LCR_PARITY_ENABLE);
	else if (ulParity == LCR_PARITY_EVEN)
		UART[UART_LCR] |= LCR_PARITY_ENABLE | LCR_PARITY_EVEN;
	else if (ulParity == LCR_PARITY_ODD) {
		UART[UART_LCR] |= LCR_PARITY_ENABLE;
		UART[UART_LCR] &= LCR_PARITY_ODD;
	}
	/* Return successful */
	
	CKUartTrasmitEmpty();
	
	return(0);
}


uint32_t CKUartRead(	uint8_t *pucData,
					uint32_t ulNumBytes, 
					uint32_t *pulBytesRead,
					uint32_t  ulFlags)
{
	*pulBytesRead = 0;
	
		/* Check the flags and block if required */
	 if (ulFlags & UART_BLOCKING)
	 {
		/* Read the requested number of bytes */
		while (*pulBytesRead < ulNumBytes)
		{
	  		/* NOTE: Should have timeout here */
	
		       /* Get the data byte and status */
		    while ((UART[UART_LSR] & LSR_DATA_READY) == 0);
			/* Save data byte if character received and no errors */
	        *pucData = UART[UART_RBR];
	                 /* Increment the total */
	        (*pulBytesRead)++;
	                 /* Stop at the NUL character if requested */
	        if ( (*pulBytesRead ==ulNumBytes) || (*pucData == 0))
	            break;
	        pucData++;
		}
	 }
	
	/* Return successful */
	return(*pulBytesRead);
}

uint32_t CKUartWrite(	uint8_t*pucData,
    				uint32_t ulNumBytes, 
    				uint32_t *pulBytesWritten,
				uint32_t ulFlags)
{
       /*For uart write time out number setting*/
//	unsigned long timeout	=   UART_WRITE_TIMEOUT;
	   
	*pulBytesWritten = 0;
	/* Check the flags and block if required */
    if (ulFlags & UART_BLOCKING)
    {
        	/* Write the requested number of bytes */
	    while (*pulBytesWritten < ulNumBytes)
		{
		/* NOTE: Should have timeout here */
        
		/* Wait for room in FIFO */
      	   	while ((UART[UART_LSR] & LSR_TRANS_EMPTY) == 0);
                /* Put the data byte in the FIFO */
        	UART[UART_THR] = *pucData;
		/* Increment the total */
        	(*pulBytesWritten)++;
	        /* Stop at the NUL character if requested */
        	if ( (ulNumBytes == *pulBytesWritten) || (*pucData == 0))
             	break;
              /* Move the pointer */
        	pucData++;
		}
    }
    /* Return successful */
    return (*pulBytesWritten);
}

extern void write_console(char c);
xSemaphoreHandle s_xsem_serial = NULL;

char shellcmd[SHELLNAME_LEN];
uint8_t shellidx = 0;

void CKUart0handler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
	char tmp;

	if (( UART[UART_LSR] & LSR_OVERRUN_ERROR ) || (UART[UART_LSR] & LSR_RFE))
	{
		tmp = UART[UART_LSR];
		tmp = UART[UART_RBR];
	}

	if ( UART[UART_LSR] & LSR_THR_EMPTY)
	{
		if ( s_xsem_serial == NULL)
		{
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		}

		tmp = UART[UART_RBR];
		if ((tmp == '\n') || (tmp == '\r'))
		{
			xSemaphoreGiveFromISR(s_xsem_serial, &xHigherPriorityTaskWoken );
		}
		else if ((tmp == 0x7F) || (tmp == 0x8))
		{
			if (shellidx > 0)
			{
				shellidx --;
				shellcmd[shellidx] = '\0';
				write_console('\b');
				write_console(' ');
				write_console('\b');
			}
		}
		else
		{
			if (shellidx < SHELLNAME_LEN)
			{
				shellcmd[shellidx] = (tmp);
				shellidx++;
			}
			write_console(tmp);
		}
	}
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}





