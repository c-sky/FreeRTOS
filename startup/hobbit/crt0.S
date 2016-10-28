/****************************************************************************
 *
 * Copyright (C) 2015 YunOS Project. All rights reserved.
 *
 ****************************************************************************
 FILE_NAME           : crt0.S
 FUNCTION            : the file is startup codes for csky cpu:
                       1, initial PSR
                       2, initial VBR
                       3, initial sp(stack pointer)
                       4, initial MMU
                       5, initial cache
                       6, initial bss section
                       7, goto main function
 ******************************************************************************/

#include "cklddata.h"
#include "ckbss.h"
#include "ckcpu.h"

.file "crt0.S"

#define CONFIG_SECURE_STACKSIZE     0x1000
#define CONFIG_NSECURE_STACKSIZE    0x1000
#define CONFIG_MM_KERNEL_HEAPSIZE   0
#define CONFIG_ARCH_INTERRUPTSTACK  0

trap:
alignment:
vectorirq_handler:
autoirq_handler:

/*
 * For importing variable or functions from script file "ckcpu.ld".
 */
/* Under normal circumstances,  it should not be opened */
#ifdef CONFIG_MD_SECURITY
.import _s_stack                                /* The stack pointer from linking script file */
#else
.import _ns_stack                             /* The stack pointer from linking script file */
#endif

/*
 * For importing variable or functions from other c or assemble files.
 */
.import trap
.import alignment
.import autoirq_handler
.import vectorirq_handler
.import main                               /* The c codes entry */

/* For exporting varialbe or functions */
.export ckcpu_vsr_table                        /* Vector table base address. */
.export ckcpu_nsvsr_table                        /* Vector table base address. */
.export __start                                /* The system entry. */

/* Under normal circumstances,  it should not be opened */
#ifdef CONFIG_MD_SECURITY
.section .exp_table
/* vector table space */
.align 10
ckcpu_vsr_table:
    .long __start
    .long alignment
.rept 8
    .long trap
.endr
    .long autoirq_handler
.rept 21
    .long trap
.endr
.rept 32
    .long vectorirq_handler
.endr
#else
.section .sexp_table
/* vector table space */
.align 10
ckcpu_nsvsr_table:
        .long __start
        .long alignment
.rept 8
        .long trap
.endr
        .long autoirq_handler
.rept 21
        .long trap
.endr
.rept 32
        .long vectorirq_handler
.endr
#endif

/* the ckcpu startup codes */
.text

#ifdef CONFIG_RUN_IN_FLASH
     .long __start
#endif

__start:
/* Under normal circumstances,  it should not be opened */
#ifdef CONFIG_MD_SECURITY
    csky_cpu_initpsr
    /* Step 2:
     * Setup initial vector base table for interrupts and exceptions
     */
    csky_cpu_initvec ckcpu_vsr_table

    /* Step 3:
     *Initialize the normal stack pointer from the linker definition.
     */
    csky_cpu_initstack _s_stack
#else
    csky_cpu_initnspsr
    csky_cpu_initnsvec g_pfnVectors
    csky_cpu_init_ns_stack _ns_stack
#endif

//#ifdef CONFIG_RUN_IN_FLASH
    /* Step 4:
     * copy .data section to ram.
     * NOTE: .data must align 4, include start address and length
     */
    csky_load_data _sdata _edata _erodata
//#endif

    /* Step 6:
     * Zero out the bss region.
     * NOTE: _sbss and _ebss must align 4
     */
    csky_bss_init _sbss _ebss

    /* Step 7:
     * Assember start up done, C codes start here.
     */
__goto_c:
    /* goto main */
    jbsr   main
    /* Should never get here. */
__exit:
    br      __exit

    .section .s_stack.sec
    .align 2
.rept (CONFIG_SECURE_STACKSIZE >> 2)
    .long 0
.endr

    .section .ns_stack.sec
    .align 2
.rept (CONFIG_NSECURE_STACKSIZE >> 2)
    .long 0
.endr

    .section .heap.sec
    .align 2
.rept (CONFIG_MM_KERNEL_HEAPSIZE>>2)
    .long 0
.endr

    .data
    .align  2
    .globl  g_idle_topstack
    .type   g_idle_topstack, object
g_idle_topstack:
    .long   _ns_stack
    .size   g_idle_topstack, .-g_idle_topstack

    .align  2
    .globl  g_intstackalloc
    .global g_intstackbase
g_intstackalloc:
    .skip   (CONFIG_ARCH_INTERRUPTSTACK & ~7)
    .align  3
g_intstackbase:
    .size   g_intstackalloc, .-g_intstackalloc
    .end
