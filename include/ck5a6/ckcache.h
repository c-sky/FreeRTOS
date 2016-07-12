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
 FILE_NAME           : ckcache.h
 AUTHOR              : lin lin
 MODIFIED BY         : xialf
 FUNCTION            : the file is icache init operation for csky cpu:
 ******************************************************************************/
#ifndef _CKCACHE_H_
#define _CKCACHE_H_

.macro  csky_cache_init
/* Invalid Instruction and data cache content */
mfcr    r4, cr17
bseti   r4, 1
bseti   r4, 2
bseti   r4, 4
mtcr    r4, cr17

/* Enable instruction and data cache */
mfcr    r3, cr18
bseti   r3, 2
bseti   r3, 3
mtcr    r3, cr18

.endm

#endif
