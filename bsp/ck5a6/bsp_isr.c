#include "bsp.h"


void CKTimer1Handler(void)
{
    CKTimerClear(TIMER1);
}

extern void write_console(char c);
xSemaphoreHandle g_xsem_serial = NULL;

char g_shellcmd[SHELLNAME_LEN];
uint8_t g_shellidx = 0;


void CKUart1Handler(void)
{
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    char tmp;

    if (( BSPUARTREG->lsr & LSR_OVERRUN_ERROR ) || (BSPUARTREG->lsr & LSR_RFE))
    {
        tmp = BSPUARTREG->lsr;
        tmp = BSPUARTREG->rbr_thr_dll;
    }

    if ( BSPUARTREG->lsr & LSR_THR_EMPTY)
    {
        if ( g_xsem_serial == NULL)
        {
            portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        }

        tmp = BSPUARTREG->rbr_thr_dll;
        if ((tmp == '\n') || (tmp == '\r'))
        {
            xSemaphoreGiveFromISR(g_xsem_serial, &xHigherPriorityTaskWoken );
        }
        else if ((tmp == 0x7F) || (tmp == 0x8))
        {
            if (g_shellidx > 0)
            {
                g_shellidx --;
                g_shellcmd[g_shellidx] = '\0';
                write_console('\b');
                write_console(' ');
                write_console('\b');
            }
        }
        else
        {
            if (g_shellidx < SHELLNAME_LEN)
            {
                g_shellcmd[g_shellidx] = (tmp);
                g_shellidx++;
            }
            write_console(tmp);
        }
    }
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

}

__attribute__((naked)) void CKTimer1Isr(void)
{
    __asm__ __volatile__(
        "subi   sp, 32  \n"                 // Decrement SP to save registers
        "subi   sp, 28  \n"                 // Decrement SP to save registers
        "stm    r1-r15, (sp)    \n"         // Save all registers to the stack

        "subi   sp, 8   \n"                 // Decrement SP to save PC and PSR
        "mfcr   r1, EPC \n"                 // Save the PC for the current task
        "st.w   r1, (sp,0)  \n"
        "mfcr   r1, EPSR    \n"             // Save the PSR for the current task
        "st.w   r1, (sp,4)  \n"

        "lrw    r3, pxCurrentTCB    \n"     // save current SP
        "ld.w   r4, (r3)    \n"
        "st.w   sp, (r4)    \n"

        "jbsr   CKTimer1Handler \n"
        "jbsr   xPortSysTickHandler \n"

        "ldw    r1, (sp,4)  \n"             // Get the PSR for the task
        "mtcr   r1, EPSR    \n"
        "ldw    r1, (sp,0)  \n"             // Get the PC for the task
        "mtcr   r1, EPC \n"
        "addi   sp, 8   \n"                 // Increment SP past the PC and PSR

        "ldm    r1-r15, (sp)    \n"         // Load all registers from the stack
        "addi   sp, 32  \n"                 // Increment SP past the registers
        "addi   sp, 28  \n"                 // Increment SP past the registers
        "rte    \n"
        );
}

__attribute__((naked)) void CKUart1Isr(void)
{
    __asm__ __volatile__(
        "subi   sp, 32  \n"     // Decrement SP to save registers
        "subi   sp, 28  \n"             // Decrement SP to save registers
        "stm    r1-r15, (sp)    \n"         // Save all registers to the stack

        "subi   sp, 8   \n"                 // Decrement SP to save PC and PSR
        "mfcr   r1, EPC \n"                 // Save the PC for the current task
        "st.w   r1, (sp,0)  \n"
        "mfcr   r1, EPSR    \n"             // Save the PSR for the current task
        "st.w   r1, (sp,4)  \n"

        "lrw    r3, pxCurrentTCB    \n" // save current SP
        "ld.w   r4, (r3)    \n"
        "st.w   sp, (r4)    \n"

        "jbsr   CKUart1Handler  \n"

        "ldw    r1, (sp,4)  \n"                 // Get the PSR for the task
        "mtcr   r1, EPSR    \n"

        "ldw    r1, (sp,0)  \n"                 // Get the PC for the task
        "mtcr   r1, EPC \n"

        "addi   sp, 8   \n"                     // Increment SP past the PC and PSR

        "ldm    r1-r15, (sp)    \n"                 // Load all registers from the stack
        "addi   sp, 32  \n"                 // Increment SP past the registers
        "addi   sp, 28  \n"                 // Increment SP past the registers
        "rte    \n"
        );
}

__attribute__((naked)) void CKPendSVIsr(void)
{
    __asm__ __volatile__ (
    "subi   sp, 32  \n"                 // Decrement SP to save registers
    "subi   sp, 28  \n"                 // Decrement SP to save registers
    "stm    r1-r15, (sp)    \n"             // Save all registers to the stack

    "subi   sp, 8   \n"                     // Decrement SP to save PC and PSR

    "mfcr   r1, EPC \n"                 // Save the PC for the current task
    "addi   r1, 2   \n"                     // Add 2 to PC to get past TRAP
                                    // instruction when returning
    "st.w   r1, (sp,0)  \n"

    "mfcr   r1, EPSR    \n"                 // Save the PSR for the current task
    "st.w   r1, (sp,4)  \n"

    "lrw    r2, pxCurrentTCB    \n"         // Save the current task SP in the TCB
    "ld.w   r3, (r2)    \n"
    "st.w   sp, (r3)    \n"

    "jbsr   vTaskSwitchContext  \n"

    "lrw    r4, pxCurrentTCB    \n"
    "ld.w   r4, (r4)    \n"                 // the current task stack pointer is the first member
    "ld.w   sp, (r4)    \n"

    "ld.w   r1, (sp,0)  \n"                 // Get the PC for the task
    "mtcr   r1, EPC \n"

    "ld.w   r1, (sp,4)  \n"                 // Get the PSR for the task
    "mtcr   r1, EPSR    \n"

    "addi   sp, 8   \n"                 // Increment SP past the PC and PSR

    "ldm    r1-r15,(sp) \n"                 // Load R0-R13 from the stack
    "addi   sp, 32  \n"                 // Increment SP past the registers
    "addi   sp, 28  \n"                 // Increment SP past the registers
    "rte    \n"
    );

}


