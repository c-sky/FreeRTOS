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
.long     _start
.long 	ISR_Misaligned_Access
.long 	ISR_Access_Error
.long 	ISR_Div_By_Zero
.long 	ISR_Illegal
.long 	ISR_Privlege_Violation
.long 	ISR_Trace_Exection
.long 	ISR_Breakpoint_Exception
.long 	ISR_Unrecoverable_Error
.long 	ISR_Soft_Reset
.long 	ISR_INT
.long 	ISR_FINT 			// Set LSB for alternate registers
.long 	ISR_Hardware_Accelerator
     
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
     
.long   ISR_Trap0
.long	ISR_Trap1
.long	ISR_Trap2
.long	ISR_Trap3
.long	ISR_Reserved
.long	ISR_Reserved
.long	ISR_Reserved
.long	ISR_Reserved
     
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
.long 	ISR_Reserved
     
//----------------------------------------------------------------------
// User Interrupts (32 - 127)     
//----------------------------------------------------------------------
.long 	ISR_Vectored    // 0 - Not implemented
.long 	ISR_Vectored    // 1 - Not implemented
.long 	ISR_Vectored    // 2 - Not implemented
.long 	ISR_Vectored    // 3 - Not implemented
.long 	ISR_Vectored    // 4 - Not implemented
.long 	ISR_Vectored    // 5 - Not implemented
.long 	ISR_Vectored    // 6 - Not implemented
.long 	ISR_Vectored    // 7 - Not implemented
.long	ISR_Vectored    // 8 - Not implemented
.long 	ISR_Vectored    // 9 - Not implemented
.long 	ISR_Vectored    // 10 - Not implemented
.long 	ISR_Vectored    // 11 - Not implemented
.long 	ISR_Vectored    // 12 - Not implemented
.long 	ISR_Vectored    // 13 - Not implemented
.long 	ISR_Vectored    // 14 - Not implemented
.long 	ISR_Vectored    // 15 - Not implemented
.long 	ISR_Vectored    // 16 - Not implemented
.long 	ISR_Vectored    // 17 - Not implemented
.long 	ISR_Vectored    // 18 - Not implemented
.long 	ISR_Vectored    // 19 - Not implemented
.long 	ISR_Vectored    // 20 - Not implemented
.long 	ISR_Vectored    // 21 - Not implemented
.long 	ISR_Vectored    // 22 - Not implemented   
.long 	ISR_Vectored    // 23 - Not implemented  
.long 	ISR_Vectored    // 24 - Not implemented
.long 	ISR_Vectored    // 25 - Not implemented
.long 	ISR_Vectored    // 26 - Not implemented   
.long 	ISR_Vectored    // 27 - Not implemented
.long 	ISR_Vectored    // 28 - Not implemented
.long 	ISR_Vectored    // 29 - Not implemented
.long 	ISR_Vectored    // 30 - Not implemented
.long 	ISR_Vectored    // 31 - Not implemented

//**********************************************************************// 
// Code Start Section
//**********************************************************************// 
 .text
//----------------------------------------------------------------------
// Initial interrupt service routines (ISRs)
//----------------------------------------------------------------------
ISR_Misaligned_Access:
  	br ISR_Misaligned_Access 
ISR_Access_Error:
  	br  ISR_Access_Error
ISR_Div_By_Zero:
	br  ISR_Div_By_Zero
ISR_Reserved:
  	br  ISR_Reserved
ISR_Illegal:
  	br  ISR_Illegal
ISR_Privlege_Violation:
  	br  ISR_Privlege_Violation
ISR_Trace_Exection:
  	br  ISR_Trace_Exection
ISR_Breakpoint_Exception:
  	rte
ISR_Unrecoverable_Error:
  	br  ISR_Unrecoverable_Error
ISR_Soft_Reset:
  	br  ISR_Soft_Reset
ISR_INT:			// Not set at initialization, reserved for RTOS
  	br  ISR_INT		// timer tick using the PIT
ISR_FINT:			// All fast interrupts use alternate regs (see above)
	br  ISR_FINT
ISR_Hardware_Accelerator:
  	br  ISR_Hardware_Accelerator
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
    // NOTE: Initialize hardware and perform any destructive memory
    //       test here
    // Initialize the stack pointer from the linker definition
    lrw		r7,__stack
    mov		sp,r7

    //write VBR
    lrw r7,vbr_base
    mtcr r7, vbr
    // Perform CRT initialization in C
    //    jsri	_crt0init
    
    /*
	 * Zero out the bss region.
	 * NOTE: __sbss must align 4
	 */
	lrw		r7, __bss_start	/* Get start of bss from linking script file */
	lrw		r6, __end	/* Get end of bss from linking script file */
	subu	r6, r7		/* Calculate size of bss */
	lsri	r6, 2		/* Size of whole words */
	cmpnei	r6, 0
	bf		__goto_c
	movi	r5, 0		/* Set zero value to write */
2:
	stw		r5, (r7)	/* Zero next word */
	addi	r7, 4		/* Increase bss pointer */
	decne	r6			/* Decrease counter */
	bt		2b			/* Repeat for all bss */

    // Jump to main() routine
__goto_c:
    jbsr	main

    // NOTE: Breakpoint if we return from main()
exit:
    bkpt

//**********************************************************************// 
// Routine for initialization of C++ constructors and destructors
//**********************************************************************// 
.align 4
.global	__main
__main:
	rts
