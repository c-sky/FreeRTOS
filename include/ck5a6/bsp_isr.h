#ifndef __BSP_ISR_H__
#define __BSP_ISR_H__

#define SHELLNAME_LEN 	512


void CKTimer1Isr(void);
void CKUart1Isr(void);
void CKPendSVIsr(void);

#endif

