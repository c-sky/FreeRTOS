/****************************************************************************
 * arch/csky/src/cskyv1/cache.h
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
 ****************************************************************************/

#ifndef __ARCH_CSKY_SRC_CSKYV1_CACHE_H
#define __ARCH_CSKY_SRC_CSKYV1_CACHE_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

/****************************************************************************
 * Pre-processor Defintiions
 ****************************************************************************/

#define    L1_CACHE_SHIFT    4      /* 16 Bytes */
#define    L1_CACHE_BYTES    (1 << L1_CACHE_SHIFT)

/* for cr17 */
#define INS_CACHE       (1 << 0)
#define DATA_CACHE      (1 << 1)
#define CACHE_INV       (1 << 4)
#define CACHE_CLR       (1 << 5)
#define CACHE_OMS       (1 << 6)
#define CACHE_ITS       (1 << 7)
#define CACHE_LICF      (1 << 31)

/* for cr22 */
#define CR22_LEVEL_SHIFT     (1)
#define CR22_SET_SHIFT       (7)
#define CR22_WAY_SHIFT       (30)
#define CR22_WAY_SHIFT_L2    (29)

/****************************************************************************
 * Public Functions
 ****************************************************************************/

void flush_cache_all(void);
void flush_icache_all(void);
void flush_dcache_all(void);
void clear_dcache_all(void);
#define yunos_csp_flush_cache_all  flush_cache_all
#define yunos_csp_flush_icache_all flush_icache_all
#define yunos_csp_flush_dcache_all flush_dcache_all
#define yunos_csp_clear_dcache_all clear_dcache_all

#define flush_cache_range(start, end)   __flush_idcache_range(start, end)
#define flush_icache_range(start, end)  __flush_icache_range(start, end)
#define flush_dcache_range(start, end)  __flush_dcache_range(start, end)
#define clear_dcache_range(start, end)  clear_dcache_all()
#define yunos_csp_flush_cache_range(start, end)   __flush_idcache_range(start, end)
#define yunos_csp_flush_icache_range(start, end)  __flush_icache_range(start, end)
#define yunos_csp_flush_dcache_range(start, end)  __flush_dcache_range(start, end)
#define yunos_csp_clear_dcache_range(start, end)  clear_dcache_all()

#endif /* __ARCH_CSKY_SRC_CSKYV1_CACHE_H */

