/************************************************************************************
 *arch/cskyv1/src/cskyv1/up_cache.c
 *
 * Copyright (C) 2015 The YunOS Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ************************************************************************************/

#include "cache.h"

/****************************************************************************
 * Pre-processor Macros
 ****************************************************************************/

#define dis_icache(tmp) \
    __asm__ __volatile__( \
        "mfcr %0, cr18\n\t" \
        "bclri %0, 2\n\t" \
        "mtcr %0, cr18\n\t" \
        ::"r"(tmp))

#define en_icache(tmp) \
    __asm__ __volatile__( \
        "mfcr %0, cr18\n\t" \
        "bseti %0, 2\n\t" \
        "mtcr %0, cr18\n\t" \
        ::"r"(tmp))

#define set_cr17(value) \
    __asm__ __volatile__( \
        "mtcr   %0, cr17\n\t" \
        ::"r"(value))

#define set_cr22(value) \
    __asm__ __volatile__( \
        "mtcr   %0, cr22\n\t" \
        ::"r"(value))

/****************************************************************************
 * Private functions
 ****************************************************************************/

/****************************************************************************
 * Name: __flush_cache_range
 *
 * Description:
 *   Flush cache by cacheline.
 *
 * Parameters:
 *   start: start address
 *   end:   end address
 *   value: INS_CACHE, DATA_CACHE or both.
 *
 * Return:
 *
 ****************************************************************************/

static inline void __flush_cache_range(
        unsigned long start,
        unsigned long end,
        unsigned long value)
{
    unsigned long i,flags;
    unsigned int tmp = 0;

    if ((end - start) > 0x1000) {
        if (value | INS_CACHE) {
            flush_icache_all();
        }
        if (value | DATA_CACHE) {
            flush_dcache_all();
        }
        return;
    }

    flags = irqsave();

    if (value & INS_CACHE) {
        dis_icache(tmp);
    }

    for (i = start; i < end; i += L1_CACHE_BYTES) {
        set_cr22(i);
        set_cr17(CACHE_OMS | value);
    }

    if (end & (L1_CACHE_BYTES - 1)) {
        set_cr22(end);
        set_cr17(CACHE_OMS | value);
    }

    if (value & INS_CACHE) {
        en_icache(tmp);
    }

    irqrestore(flags);
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: flush_cache_all
 *
 * Description:
 *   Invalid and clear icache and dcache all.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void flush_cache_all(void)
{
    int value = 0x33;
    __asm__ __volatile__("mtcr %0,cr17\n\t"
                         "sync\n\t"
                         : :"r" (value));
}

/****************************************************************************
 * Name: flush_icache_all
 *
 * Description:
 *   Invalid and clear icache all.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void flush_icache_all(void)
{
    int value = 0x11;
    __asm__ __volatile__("mtcr %0,cr17\n\t"
                         "sync\n\t"
                         : :"r" (value));
}

/****************************************************************************
 * Name: flush_dcache_all
 *
 * Description:
 *   Invalid and clear dcache all.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void flush_dcache_all(void)
{
    int value = 0x32;
    __asm__ __volatile__("mtcr %0,cr17\n\t"
                         "sync\n\t"
                         : :"r" (value));
}

/****************************************************************************
 * Name: clear_dcache_all
 *
 * Description:
 *   Clear icache and dcache all.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void clear_dcache_all(void)
{
    int value = 0x22;
    __asm__ __volatile__("mtcr %0,cr17\n\t"
                         "sync\n\t"
                         : :"r" (value));
}

/****************************************************************************
 * Name: __flush_dcache_range
 *
 * Description:
 *   Invalid and Clear dcache by cacheline.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void __flush_dcache_range(
    unsigned long start,
    unsigned long end
    )
{
    __flush_cache_range(
        start, end,
        DATA_CACHE|CACHE_INV|CACHE_CLR);
}

/****************************************************************************
 * Name: __flush_icache_range
 *
 * Description:
 *   Invalid and Clear icache by cacheline.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void __flush_icache_range(
    unsigned long start,
    unsigned long end
    )
{
    __flush_cache_range(
        start, end,
        INS_CACHE|CACHE_INV|CACHE_CLR);
}

/****************************************************************************
 * Name: __flush_idcache_range
 *
 * Description:
 *   Invalid and Clear icache and dcache by cacheline.
 *
 * Parameters:
 *
 * Return:
 *
 ****************************************************************************/

void __flush_idcache_range(
    unsigned long start,
    unsigned long end
    )
{
    __flush_cache_range(
        start, end,
        INS_CACHE|DATA_CACHE|
        CACHE_INV|CACHE_CLR);
}
