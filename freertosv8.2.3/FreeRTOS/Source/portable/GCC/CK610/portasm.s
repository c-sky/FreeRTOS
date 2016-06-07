.file	"portasm.s"


.text
//**********************************************************************// 
// Set ISR vector function (r2 contains ISR address, r3 has vector number)
//**********************************************************************// 

.export CKSetVector
.align 4
CKSetVector:
	mfcr	    r1,VBR
	lsli	    r3,2
	addu    r3,r1
	st.w	    r2,(r3)
	rts

	
//**********************************************************************// 
// CPU Save PSR and Disable Interrputs function 
// (no parameter, return value saved in r2)
//**********************************************************************// 
.export CPU_PSR_SAVE
.align 4
CPU_PSR_SAVE:
	mfcr  r2, PSR
	mov r1, r2
	bclri r1, 0x4
	bclri r1, 0x6
	mtcr r1, PSR	
	rts
	

//**********************************************************************// 
// CPU Restore PSR function (SR saved in r2)
//**********************************************************************// 
.export CPU_PSR_STORE
.align 4
CPU_PSR_STORE:
	mtcr  r2, PSR
	rts

