#include "bsp.h"
#include "lib_print.h"

static void CKUartTrasmitEmpty(void)
{
    while ((BSPUARTREG->usr & USR_TFE) == 0);
}

uint32_t CKUartInit(uint32_t ulBaudRate, uint32_t ulDataBits, uint32_t ulStopBits, uint32_t ulParity)
{
    /* NOTE: Could perform parameter checking here */
    uint32_t uDivisor;

    /* Calculate the divisor value by dividing the crystal frequency
       by the baud rate, correcting for the remainder if necessary
       and dividing the result by 16 */
    uDivisor = ((UART_FREQ / ulBaudRate) >> UART_DIV);
    /* Set the baud rate generator divisor */
    BSPUARTREG->lcr|= LCR_SEL_DLR;
    BSPUARTREG->rbr_thr_dll = uDivisor & 0xff;
    BSPUARTREG->ier_dlh= (uDivisor >> 8) & 0xff;

    /* CLR the DLAB bit of LSR */
    BSPUARTREG->lcr &= (~LCR_SEL_DLR);

    /* Set FIFO Control Register*/
    BSPUARTREG->iir_fcr= FCR_BYTE_1;

    /* Enable Data Receiver Ready Interrupt */
    BSPUARTREG->ier_dlh |= IER_RDAR_INT_ENABLE;

    /* Set the number of data bits (character length) */
    if (ulDataBits == 8)
        BSPUARTREG->lcr |= LCR_WORD_SIZE_8;

    /* Set the stop bit length */
    if (ulStopBits != 1)
        BSPUARTREG->lcr |= LCR_STOP_BIT_2;
    else
        BSPUARTREG->lcr &= LCR_STOP_BIT_1;

    /* Set the parity mode */
    if (ulParity == LCR_PARITY_NONE)
        BSPUARTREG->lcr &= (~LCR_PARITY_ENABLE);
    else if (ulParity == LCR_PARITY_EVEN)
        BSPUARTREG->lcr |= LCR_PARITY_ENABLE | LCR_PARITY_EVEN;
    else if (ulParity == LCR_PARITY_ODD) {
        BSPUARTREG->lcr |= LCR_PARITY_ENABLE;
        BSPUARTREG->lcr &= LCR_PARITY_ODD;
    }
    /* Return successful */

    CKUartTrasmitEmpty();

    return(0);
}


uint32_t CKUartRead(uint8_t *pucData,uint32_t ulNumBytes, uint32_t *pulBytesRead,uint32_t  ulFlags)
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
            while ((BSPUARTREG->lsr & LSR_DATA_READY) == 0);
            /* Save data byte if character received and no errors */
            *pucData = BSPUARTREG->rbr_thr_dll;
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

uint32_t CKUartWrite(uint8_t*pucData, uint32_t ulNumBytes, uint32_t *pulBytesWritten,uint32_t ulFlags)
{
       /*For uart write time out number setting*/
//  unsigned long timeout   =   UART_WRITE_TIMEOUT;

    *pulBytesWritten = 0;
    /* Check the flags and block if required */
    if (ulFlags & UART_BLOCKING)
    {
            /* Write the requested number of bytes */
        while (*pulBytesWritten < ulNumBytes)
        {
        /* NOTE: Should have timeout here */

        /* Wait for room in FIFO */
            while ((BSPUARTREG->lsr & LSR_TRANS_EMPTY) == 0);
                /* Put the data byte in the FIFO */
            BSPUARTREG->rbr_thr_dll = *pucData;
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






