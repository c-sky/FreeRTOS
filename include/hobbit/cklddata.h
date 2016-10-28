/* ****************************************************************************
 *                                                                          *
 * C-Sky Microsystems Confidential                                          *
 * -------------------------------                                          *
 * This file and all its contents are properties of C-Sky Microsystems. The *
 * information contained herein is confidential and proprietary and is not  *
 * to be disclosed outside of C-Sky Microsystems except under a             *
 * Non-Disclosured Agreement (NDA).                                         *
 *                                                                          *
 ****************************************************************************
 FILE_NAME           : cklddata.h
 AUTHOR              : Menghan Jia
 DATE                : 2015-10-23
 FUNCTION            : this file is to load data from flash to sram if 
                       the program is run in norflash
 ******************************************************************************/

#ifndef _CKLDDATA_H_
#define _CKLDDATA_H_


.macro csky_load_data saddr eaddr eeaddr
	lrw     a3, \saddr      /* Get start of data from linking script file */
	lrw     a1, \eaddr      /* Get end of data from linking script file */
	cmphs   a3, a1          /* Calculate size of data */
	lrw     a2, \eeaddr     /* Get end of rodata from linking script file */
.L_load_data:
	ld.w    a0, (a2, 0)     /* Load data from flash */
	st.w    a0, (a3, 0)     /* Store data to SSRAM */
	addi    a3, 4           /* Increase data pointer of flash */
	addi    a2, 4           /* Increase data pointer of SSRAM */
	cmphs   a3, a1
	bf      .L_load_data    /* Repeat for all data */

.endm

#endif
