#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp.h"

extern void ShellTask(void *pvParameters);

int main (void)
{
	BSP_Init();
	
	xTaskCreate( ShellTask, ( portCHAR * ) "Shell", configMINIMAL_STACK_SIZE * 2, NULL, tskIDLE_PRIORITY+2, NULL );
	
	vTaskStartScheduler();
	
	return 0;

}

