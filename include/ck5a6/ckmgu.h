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
 FILE_NAME           : csky_mgu.h
 AUTHOR              : lin lin
 MODIFY              : xialf   2015.08.3
 FUNCTION            : the file is about set mgu of csky cpu
 ******************************************************************************/
#ifndef _MGU_H_
#define _MGU_H_

/* defines of region size */
#define REGION_4K   0x16
#define REGION_8K   0x18
#define REGION_16K  0x1a
#define REGION_32K  0x1c
#define REGION_64K  0x1e
#define REGION_128K 0x20
#define REGION_256K 0x22
#define REGION_512K 0x24
#define REGION_1M   0x26
#define REGION_2M   0x28
#define REGION_4M   0x2a
#define REGION_8M   0x2c
#define REGION_16M  0x2e
#define REGION_32M  0x30
#define REGION_64M  0x32
#define REGION_128M 0x34
#define REGION_256M 0x36
#define REGION_512M 0x38
#define REGION_1G   0x3a
#define REGION_2G   0x3c
#define REGION_4G   0x3e

/* defines of region permission */
#define MGU_REGION_RW               0x300     //user RW & super RW
#define MGU_REGION_RO               0x200     //user RO & super RW
#define MGU_REGION_NOTACCESSIBLE    0x100     //user not accessible & super RW
#define MGU_REGION_SNOTACCESSIBLE   0x000     //both user and super not accessible

/* defines of region cacheable */
#define MGU_REGION_UNCACHE          0x0
#define MGU_REGION_CACHE            0x1

.macro  csky_mgu_region region addr len permission cache
        /* set the mgu permission and cache */
        mfcr    r4, cr19
        lrw     r7, \permission
        movi    r6, \cache
        movi    r5, \region
        lsl     r6, r5
        lsli    r5, 1
        lsl     r7, r5
        or      r7, r6
        or      r7, r4
        mtcr    r7, cr19
        
        /* set region */
        movi    r5, \region
        mtcr    r5, cr21
        movi    r5, \len
        bseti   r5, 0
        lrw     r7, \addr
        or      r7, r5
        mtcr    r7, cr20

.endm 

.macro  csky_mgu_enable
        /* MGU enable */
        mfcr    r7, cr18
        bseti   r7, 0
        bseti   r7, 4
        bseti   r7, 5
        bseti   r7, 6
        mtcr    r7, cr18
.endm

.macro  csky_mgu_disable
        /* MGU enable */
        mfcr    r7, cr18
        bclri   r7, 0
        mtcr    r7, cr18
.endm

#endif
