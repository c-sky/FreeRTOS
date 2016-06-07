#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "lib_print.h"
#include "bsp_uart.h"
#include "string.h"

#define HELLO_INFO()		printk("\r\n\r\n");\
						printk("*******************************************\r\n");\
						printk("Welcome to C-sky CK610 Shell under FreeRTOS\r\n");\
						printk("*******************************************\r\n\r\n")
						
#define BYE_INFO()		printk("\r\nBye Bye .. \r\n")
#define CSKY_H()			printk("Csky $ ")

extern xSemaphoreHandle s_xsem_serial;
extern char shellcmd[SHELLNAME_LEN];
extern uint8_t shellidx;

enum SHELLSTATUS {SOPEN, SRUN, SCLOSE};	
enum SHELLSTATUS ssts = SOPEN;


struct cmdreg
{
	char name[16];
	void (*cmdhandler)(char *buf);
};

static void dump_os_status(char *buf)
{
	sprintf(buf + strlen(buf), "\r\n%s", "Name		  State  Priority  Stack   PID" );
	sprintf(buf + strlen(buf), "\r\n%s", "---------------------------------------------"); 
	sprintf(buf + strlen(buf), "\r\n");
	vTaskList((char *)(buf + strlen(buf)));
	sprintf(buf + strlen(buf), "%s", "---------------------------------------------"); 
	sprintf(buf + strlen(buf), "\r\n%s","B : Blocked, R : Ready, D : Deleted, S : Suspended");
	sprintf(buf + strlen(buf), "\r\n");
}

static void dbg_handler(char *buf)
{
	sprintf(buf, "\r\n%s\r\n", "debug handler");
}

struct cmdreg shellcmdreg[] = 
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
		.cmdhandler = NULL
	}
};

static char *shellcmdlaunch (char *cmd)
{
	char *shellrsp = cmd;
	struct cmdreg *shellcmdptr = NULL;

	for (shellcmdptr = shellcmdreg; shellcmdptr->cmdhandler != NULL; shellcmdptr ++)
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

	s_xsem_serial = xSemaphoreCreateBinary();
	xSemaphoreTake(s_xsem_serial, 1);
	
	HELLO_INFO();
	CSKY_H();
	ssts = SRUN;
	memset (shellcmd, 0, SHELLNAME_LEN);
	
	while(1)
	{
		xsem_take = xSemaphoreTake(s_xsem_serial, 1000 * 60 * 5 / portTICK_RATE_MS);
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
					if (!strcmp("quit", shellcmd) || !strcmp("exit", shellcmd))
					{
						ssts = SOPEN;
						BYE_INFO();
						memset (shellcmd, 0, SHELLNAME_LEN);
						shellidx = 0;
					}
					else
					{
						shellcmdlaunch(shellcmd);
						CSKY_H();
						memset (shellcmd, 0, SHELLNAME_LEN);
						shellidx = 0;
					}
					break;
				default:
					break;
			}
		}
		
	}
	
}



