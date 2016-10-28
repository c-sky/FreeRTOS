/*
 *  Copyright (C) 2016 YunOS Project. All rights reserved.
 */

#ifndef __VIC_H_
#define __VIC_H_

/*****************************************************************************/

#define VECTOR_RESET            0
#define VECTOR_MISALIGN         1
#define VECTOR_ACCESS           2
#define VECTOR_DIV_BY_ZERO      3
#define VECTOR_ILLEGAL          4
#define VECTOR_PRIVILEGE        5
#define VECTOR_TRACE            6
#define VECTOR_BREAK            7
#define VECTOR_UNRECOV          8
#define VECTOR_SOFT             9
#define VECTOR_INT              10               /* Used by PIT timer only for OS tick */
#define VECTOR_FINT             11               /* All other controller-based interrupts
                                                    use fast interrupt (FINT) and alternate regs */
#define VECTOR_HW_ACCEL         12

#define VECTOR_TRAP0            16
#define VECTOR_TRAP1            17
#define VECTOR_TRAP2            18
#define VECTOR_TRAP3            19


#define VECTOR_UART0            32
#define VECTOR_TIMER            33

/* INT_CONTROLLER memory locations */

#define VIC_BASE                (0xE000E000)
#define VIC_ICR                 ((volatile uint32_t *) (VIC_BASE + 0x4))
#define VIC_ISER                ((volatile uint32_t *) (VIC_BASE + 0x100))
#define VIC_IWER                ((volatile uint32_t *) (VIC_BASE + 0x140))
#define VIC_ICER                ((volatile uint32_t *) (VIC_BASE + 0x180))
#define VIC_IWDR                ((volatile uint32_t *) (VIC_BASE + 0x1C0))
#define VIC_ISPR                ((volatile uint32_t *) (VIC_BASE + 0x200))
#define VIC_ICPR                ((volatile uint32_t *) (VIC_BASE + 0x280))
#define VIC_ISFR                ((volatile uint32_t *) (VIC_BASE + 0x300))
#define VIC_ISNR                ((volatile uint32_t *) (VIC_BASE + 0x380))

#define VIC_PR                  ((volatile uint32_t *) (VIC_BASE + 0x400))

/*****************************************************************************/

#endif /* __VIC_H_ */
