//******************************************************************************
//* File:        CRT0.S
//*
//* Description: Startup file for embedded environment.
//*
//* Comments:
//*
//* $File: $
//* $Revision: 1.2 $
//* $Date: 2013/08/21 03:25:28 $
//* $Log: crt0.s,v $
//* Revision 1.2  2013/08/21 03:25:28  quxm
//* *** empty log message ***
//*
//* Revision 1.1  2013/07/09 06:53:03  wanghb
//* first version
//*
//* Revision 1.1  2011/07/19 08:46:52  hujunshan
//* rm board ck510evb and add board ck5a6evb.
//*
//* Revision 1.2  2006/08/08 02:11:33  licq
//* Modified from ck510evb to ckm5108mevb
//*
//* Revision 1.1.1.1  2006/07/07 07:03:38  licq
//*   uCosii ported for CKM5108EVB based on the ucosii codes for CK510EVB
//*   The codes were finished but didn't debuged.
//*
//*
//*****************************************************************************/

#include "ckcache.h"
#include "ckmgu.h"
.file     "crt0.s"
.text
.import   _crt0init

//**********************************************************************//
// Setup the initial interrupt vector table
//**********************************************************************//
//----------------------------------------------------------------------
// Interrupt Vectors (0-31)
//----------------------------------------------------------------------
.section .exp_table
.align    10
vbr_base:
.long   _start
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_Breakpoint_Exception
.long   ISR_Hardware_Exceptioin
.long   ISR_Hardware_Exceptioin
.long   ISR_INT
.long   ISR_FINT            // Set LSB for alternate registers
.long   ISR_Hardware_Exceptioin

.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved

.long   CKPendSVIsr
.long   ISR_Trap1
.long   ISR_Trap2
.long   ISR_Trap3
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved

.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved
.long   ISR_Reserved

//----------------------------------------------------------------------
// User Interrupts (32 - 127)
//----------------------------------------------------------------------
.long   ISR_Vectored    // 0 - Not implemented
.long   ISR_Vectored    // 1 - Not implemented
.long   ISR_Vectored    // 2 - Not implemented
.long   ISR_Vectored    // 3 - Not implemented
.long   ISR_Vectored    // 4 - Not implemented
.long   ISR_Vectored    // 5 - Not implemented
.long   ISR_Vectored    // 6 - Not implemented
.long   ISR_Vectored    // 7 - Not implemented
.long   ISR_Vectored    // 8 - Not implemented
.long   ISR_Vectored    // 9 - Not implemented
.long   ISR_Vectored    // 10 - Not implemented
.long   ISR_Vectored    // 11 - Not implemented
.long   CKTimer1Isr    // 12 - Not implemented
.long   ISR_Vectored    // 13 - Not implemented
.long   ISR_Vectored    // 14 - Not implemented
.long   ISR_Vectored    // 15 - Not implemented
.long   CKUart1Isr    // 16 - Not implemented
.long   ISR_Vectored    // 17 - Not implemented
.long   ISR_Vectored    // 18 - Not implemented
.long   ISR_Vectored    // 19 - Not implemented
.long   ISR_Vectored    // 20 - Not implemented
.long   ISR_Vectored    // 21 - Not implemented
.long   ISR_Vectored    // 22 - Not implemented
.long   ISR_Vectored    // 23 - Not implemented
.long   ISR_Vectored    // 24 - Not implemented
.long   ISR_Vectored    // 25 - Not implemented
.long   ISR_Vectored    // 26 - Not implemented
.long   ISR_Vectored    // 27 - Not implemented
.long   ISR_Vectored    // 28 - Not implemented
.long   ISR_Vectored    // 29 - Not implemented
.long   ISR_Vectored    // 30 - Not implemented
.long   ISR_Vectored    // 31 - Not implemented

//**********************************************************************//
// Code Start Section
//**********************************************************************//
 .text
//----------------------------------------------------------------------
// Initial interrupt service routines (ISRs)
//----------------------------------------------------------------------

ISR_Hardware_Exceptioin:
    br ISR_Hardware_Exceptioin
ISR_Breakpoint_Exception:
    rte
ISR_Reserved:
    br  ISR_Reserved
ISR_INT:            // Not set at initialization, reserved for RTOS
    br  ISR_INT     // timer tick using the PIT
ISR_FINT:           // All fast interrupts use alternate regs (see above)
    br  ISR_FINT
ISR_Trap0:
    br  ISR_Trap0
ISR_Trap1:
    br  ISR_Trap1
ISR_Trap2:
    br  ISR_Trap2
ISR_Trap3:
    br  ISR_Trap3
ISR_Vectored:
    lrw r1, 0x80000100
    mtcr r1, epsr
    rte
    br  ISR_Vectored

//**********************************************************************//
// Reset Vector 0 "start" point
//**********************************************************************//
.export   _START
.export   _start
.align 4
_start:
_START:
    //sdhc
    movi    r1, 0x20
    mtcr    r1, cr11

    /* Init psr value */
    lrw r1, 0x80000100
    mtcr    r1, psr
    lrw     r7,__stack
    mov     sp,r7

    //write VBR
    lrw     r7, vbr_base
    mtcr    r7, vbr

    /*** Initial CKCPU MGU & Cache ***/

    /*
     * Step 1: Disable MGU & Cache.
     */
    csky_mgu_disable

    /*
     * Initial CKCPU MGU & Cache
     * Step 2: set MGU block
     */

    /* The first block */
    /* csky_mgu_region regionNum baseAddr regionSize */
    csky_mgu_region 0 0 REGION_4G MGU_REGION_RW MGU_REGION_UNCACHE

    /* The second block */
    /* csky_mgu_region regionNum baseAddr regionSize */
     csky_mgu_region 1 0x08000000 REGION_256K MGU_REGION_RW MGU_REGION_CACHE


    /*
     * Step 3: enable MGU
     */
    csky_mgu_enable

    /* special functional */
    mfcr    a5, cr18
    bclri   a5, 4      // WB: write back disable
    bseti   a5, 5      // RS: return stack
    bseti   a5, 6      // Z:  jumping predict
    mtcr    a5, cr18

    /*
     * Step 5: enable instruction & data cache
     */
    csky_cache_init

    lrw     r7, __bss_start /* Get start of bss from linking script file */
    lrw     r6, __end   /* Get end of bss from linking script file */
    subu    r6, r7      /* Calculate size of bss */
    lsri    r6, 2       /* Size of whole words */
    cmpnei  r6, 0
    bf      __goto_c
    movi    r5, 0       /* Set zero value to write */
2:
    stw     r5, (r7)    /* Zero next word */
    addi    r7, 4       /* Increase bss pointer */
    decne   r6          /* Decrease counter */
    bt      2b          /* Repeat for all bss */

__goto_c:

    jbsr    main

    // NOTE: Breakpoint if we return from main()
exit:
    bkpt

//**********************************************************************//
// Routine for initialization of C++ constructors and destructors
//**********************************************************************//
.align 4
.global __main
__main:
    rts
