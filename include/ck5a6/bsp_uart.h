#ifndef __FREERTOS_UART_H__
#define __FREERTOS_UART_H__
#include "bsp.h"



#define CK_UART_BASE                    (0x10015000)

#define UART_RBR                        0x00
#define UART_THR                        0x00
#define UART_DLL                        0x00
#define UART_IER                        0x01
#define UART_DLH                        0x01
#define UART_IIR                        0x02
#define UART_FCR                        0x02
#define UART_LCR                        0x03
#define UART_MCR                        0x04
#define UART_LSR                        0x05
#define UART_MSR                        0x06
#define UART_USR                        0x1f

struct BSP_UART
{
    volatile uint32_t rbr_thr_dll;
    volatile uint32_t ier_dlh;
    volatile uint32_t iir_fcr;
    volatile uint32_t lcr;
    volatile uint32_t mcr;
    volatile uint32_t lsr;
    volatile uint32_t msr;
    volatile uint32_t rsvr[24];
    volatile uint32_t usr;
};
#define BSPUARTREG                      ((struct BSP_UART *)CK_UART_BASE)

#define UART_FREQ                       APB_DEFAULT_FREQ


#define UART_DIV                        0x4

#define UART_BAUD                       19200

#define LCR_SEL_DLR                     0x80
#define LCR_BC_ENABLE                   0x40
#define LCR_STICK_PARITY_ENABLE         0x20
#define LCR_PARITY_ENABLE               0x08
#define LCR_PARITY_NONE                 0x00
#define LCR_PARITY_EVEN                 0x10
#define LCR_PARITY_ODD                  0xef
#define LCR_WORD_SIZE_5                 0X00
#define LCR_WORD_SIZE_6                 0x01
#define LCR_WORD_SIZE_7                 0x02
#define LCR_WORD_SIZE_8                 0x03
#define LCR_STOP_BIT_2                  0x04
#define LCR_STOP_BIT_1                  0xfb

#define IER_THRE_INT                    0x80
#define IER_MDM_INT_ENABLE              0x08
#define IER_RCVR_SAT_INT_ENABLE         0x04
#define IER_THER_INT_ENABLE             0x02
#define IER_RDAR_INT_ENABLE             0x01

#define LSR_RFE                         0x80
#define LSR_TRANS_EMPTY                 0x40
#define LSR_THR_EMPTY                   0x20
#define LSR_BREAK_INT                   0x10
#define LSR_FRAME_ERROR                 0x08
#define LSR_PARITY_ERROR                0x04
#define LSR_OVERRUN_ERROR               0x02
#define LSR_DATA_READY                  0x01

#define FCR_BYTE_1                      0x00
#define FCR_BYTE_4                      0x40
#define FCR_BYTE_8                      0x80
#define FCR_BYTE_14                     0xc0

#define UART_BLOCKING                   0x0001
#define UART_STOP_AT_NUL                0x0002

#define USR_RFF                         0x0010
#define USR_RFNE                        0x0008
#define USR_TFE                         0x0004
#define USR_TFNE                        0x0002
#define USR_BUSY                        0x0001



#define UART_WRITE_TIMEOUT  0x100


#define UART_PARITY_NONE    0x0000
#define UART_PARITY_ODD     0x0001
#define UART_PARITY_EVEN    0x0002


uint32_t CKUartInit(uint32_t ulBaudRate, uint32_t ulDataBits,uint32_t ulStopBits, uint32_t ulParity);
uint32_t CKUartRead(uint8_t *pucData,uint32_t ulNumBytes,uint32_t *pulBytesRead,uint32_t ulFlags);
uint32_t CKUartWrite(uint8_t *pucData,uint32_t ulNumBytes,uint32_t *pulBytesWritten,uint32_t ulFlags);

#endif

