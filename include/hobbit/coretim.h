/******************************************************************************
 **  File: coretim.h
 **
 **  Descirption: this file contains the functions support timer  operations.
 **
 ******************************************************************************/


#ifndef _CORETIM_H_
#define _CORETIM_H_
#include <stdint.h>

#define CORETIM_BASE     (0xE000E000)

#define CORET_CSR         (volatile uint32_t *)(CORETIM_BASE + 0x10)
#define CORET_RVR         (volatile uint32_t *)(CORETIM_BASE + 0x14)
#define CORET_CVR         (volatile uint32_t *)(CORETIM_BASE + 0x18)
#define CORET_CALIB       (volatile uint32_t *)(CORETIM_BASE + 0x1c)

/*
 *  define the bits for TxControl
 */
#define CORETIM_TXCONTROL_ENABLE      (1UL << 0)
#define CORETIM_TXCONTROL_INTMASK     (1UL << 1)
#define CORETIM_TXCONTROL_MODE        (1UL << 16)

/*****************************************************************************/
/* Function Prototypes */
void coretim_init (uint32_t hz);
void coretim_clr_irq (void);
uint32_t coretim_get_currval(void);

#endif /* _CORETIM_H_ */
