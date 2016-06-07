
.file     "portasmos.S"
.text

.import vPortYieldHandler
.import pxCurrentTCB
.import CKUart0handler

.macro portSAVE_CONTEXT
	subi	sp,32					// Decrement SP to save registers
	subi	sp,28					// Decrement SP to save registers
	stm r1-r15,(sp) 				// Save all registers to the stack
		
	subi	sp,8						// Decrement SP to save PC and PSR
		
	mfcr	r1,EPC					// Save the PC for the current task
	st.w r1,(sp,0)
		
	mfcr	r1,EPSR 				// Save the PSR for the current task
	st.w r1,(sp,4)
		
	lrw    r3, pxCurrentTCB 			// save current SP
	ld.w  r4, (r3)
	st.w  sp, (r4)
.endm

.macro portSTORE_CONTEXT
	ldw 	r1,(sp,4)					// Get the PSR for the task
	mtcr	r1,EPSR
			
	ldw 	r1,(sp,0)					// Get the PC for the task
	mtcr	r1,EPC
		
	addi	sp,8						// Increment SP past the PC and PSR
		
	ldm r1-r15,(sp) 				// Load all registers from the stack
	addi	sp,32					// Increment SP past the registers
	addi	sp,28					// Increment SP past the registers
.endm


.macro portSTARTTASK
	lrw   r4, pxCurrentTCB
	ld.w r4, (r4)					// the current task stack pointer is the first member 
	ld.w sp, (r4)	

	ld.w r1, (sp,0)					// Get the PC for the task
	mtcr	r1, EPC

	ld.w r1, (sp,4)					// Get the PSR for the task
	mtcr	r1, EPSR

	addi	sp, 8					// Increment SP past the PC and PSR

	ldm	r1-r15,(sp) 				// Load R0-R13 from the stack
	addi	sp, 32					// Increment SP past the registers
	addi	sp, 28					// Increment SP past the registers
.endm

.macro portSWITCHTASK
	subi	sp,32					// Decrement SP to save registers
	subi	sp,28					// Decrement SP to save registers
	stm r1-r15,(sp) 				// Save all registers to the stack

	subi	sp,8						// Decrement SP to save PC and PSR

	mfcr	r1,EPC					// Save the PC for the current task
	addi r1,2						// Add 2 to PC to get past TRAP
								// instruction when returning
	st.w r1,(sp,0)

	mfcr	r1,EPSR					// Save the PSR for the current task
	st.w r1,(sp,4)

	lrw	 r2,pxCurrentTCB			// Save the current task SP in the TCB
	ld.w  r3,(r2)
	st.w  sp,(r3)
.endm

.export CKEnableIntFromIsr
.align 4
CKEnableIntFromIsr:
	addi	sp,8						// Increment SP past the PC and PSR
		
	ldm r1-r15,(sp) 				// Load all registers from the stack
	addi	sp,32					// Increment SP past the registers
	addi	sp,28					// Increment SP past the registers
	
   	rte

.export  CKTaskSwitchIsr
.align 4
CKTaskSwitchIsr:

	portSWITCHTASK
	
	jbsr	vTaskSwitchContext		// Call task switch hook

	br  CKStartTask

.export CKStartTask
.align 4
CKStartTask:

	portSTARTTASK
	
  	rte							// Return to new task

.export  CKTickIsr
.align 4
CKTickIsr:

	portSAVE_CONTEXT
	
	movi    r2, 0x1
	jbsr  CKTimerClear            	//Clear the timer interrupt
	jbsr	xPortSysTickHandler    	//Handle the timer interrupt

	portSTORE_CONTEXT
	
	rte

.export CKUart0Isr
.align 4
CKUart0Isr:	

	portSAVE_CONTEXT
	
	jbsr	CKUart0handler 	//Handle the timer interrupt

	portSTORE_CONTEXT
		
	rte


