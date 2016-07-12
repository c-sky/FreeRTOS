#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lib_print.h"
#include "bsp_uart.h"
#include "string.h"

#define HELLO_INFO()    printk("\r\n\r\n");\
                        printk("*******************************************\r\n");\
                        printk("Welcome to C-sky CK610 Shell under FreeRTOS\r\n");\
                        printk("*******************************************\r\n\r\n")

#define BYE_INFO()      printk("\r\nBye Bye .. \r\n")
#define CSKY_H()        printk("C-sky $ ")

extern xSemaphoreHandle g_xsem_serial;
extern char g_shellcmd[SHELLNAME_LEN];
extern uint8_t g_shellidx;

enum SHELLSTATUS {SOPEN, SRUN, SCLOSE};
static enum SHELLSTATUS ssts = SOPEN;


struct cmdreg
{
    char name[16];
    void (*cmdhandler)(char *buf);
};

static void dump_os_status(char *buf)
{
    sprintf(buf + strlen(buf), "\r\n%s", "Name        State  Priority  Stack   PID" );
    sprintf(buf + strlen(buf), "\r\n%s", "---------------------------------------------");
    sprintf(buf + strlen(buf), "\r\n");
    vTaskList((char *)(buf + strlen(buf)));
    sprintf(buf + strlen(buf), "\r\n");
    sprintf(buf + strlen(buf), "%s", "---------------------------------------------");
    sprintf(buf + strlen(buf), "\r\n");
    sprintf(buf + strlen(buf), "%s","B : Blocked, R : Ready, D : Deleted, S : Suspended");
    sprintf(buf + strlen(buf), "\r\n");
}
extern size_t xPortGetFreeHeapSize( void );
static void mem_handler(char *buf)
{
    sprintf(buf, "\r\nfreeheapsize: %d\r\n", xPortGetFreeHeapSize());
}

static uint8_t flag = 0;
static void dbg_handler(char *buf)
{
    flag = 1;
    sprintf(buf, "\r\n%s\r\n", "debug handler");
}

static const struct cmdreg s_shellcmdreg[] =
{
    {
        .name = "os",
        .cmdhandler = dump_os_status
    },
    {
        .name = "dbg",
        .cmdhandler = dbg_handler
    },
    {
        .name = "mem",
        .cmdhandler = mem_handler
    },
    {
        .cmdhandler = NULL
    }
};

static char *shellcmdlaunch (char *cmd)
{
    char *shellrsp = cmd;
    const struct cmdreg *shellcmdptr = NULL;

    for (shellcmdptr = s_shellcmdreg; shellcmdptr->cmdhandler != NULL; shellcmdptr ++)
    {
        if (!strcmp(shellcmdptr->name, shellrsp))
        {
            memset(shellrsp, 0, SHELLNAME_LEN);
            shellcmdptr->cmdhandler(shellrsp);
            break;
        }
    }

    if (shellcmdptr->cmdhandler == NULL)
    {
        memset(shellrsp, 0, SHELLNAME_LEN);
        sprintf(shellrsp, "\r\n%s\r\n", "Unknown Shell Command");
    }

    printk("%s", shellrsp);

    return shellrsp;
}

void ShellTask(void *pvParameters)
{
    portBASE_TYPE xsem_take;

    g_xsem_serial = xSemaphoreCreateBinary();
    xSemaphoreTake(g_xsem_serial, 1);

    HELLO_INFO();
    CSKY_H();
    ssts = SRUN;
    memset(g_shellcmd, 0, SHELLNAME_LEN);

    while(1)
    {
        xsem_take = xSemaphoreTake(g_xsem_serial, 1000 * 60 * 5 / portTICK_RATE_MS);
        if (xsem_take == pdTRUE)
        {
            switch (ssts)
            {
                case SOPEN:
                    HELLO_INFO();
                    CSKY_H();
                    ssts = SRUN;
                    break;
                case SRUN:
                    if (!strcmp("quit", g_shellcmd) || !strcmp("exit", g_shellcmd))
                    {
                        ssts = SOPEN;
                        BYE_INFO();
                        memset (g_shellcmd, 0, SHELLNAME_LEN);
                        g_shellidx = 0;
                    }
                    else
                    {
                        shellcmdlaunch(g_shellcmd);
                        CSKY_H();
                        memset (g_shellcmd, 0, SHELLNAME_LEN);
                        g_shellidx = 0;
                    }
                    break;
                default:
                    break;
            }
        }

    }
}

void dumpTask(void *pvParameters)
{
    while(1)
    {
        if (flag)
        {
            flag = 0;
            vTaskDelete(NULL);
        }
        printk("dump task \r\n");
    }
}

void goTask(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(100);
        printk("go task \r\n");
    }
}

void backTask(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(50/portTICK_PERIOD_MS);
        printk("back task \r\n");
    }
}



