#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "bsp.h"

extern void ShellTask(void *pvParameters);
extern void dumpTask(void *pvParameters);
extern void goTask(void *pvParameters);
extern void backTask(void *pvParameters);
extern void vStartBlockingQueueTasks( UBaseType_t uxPriority );
extern BaseType_t xAreBlockingQueuesStillRunning( void );

static void preSetupHardware(void)
{
    BSP_Init();
}

int main (void)
{
    preSetupHardware();

    xTaskCreate( ShellTask, ( portCHAR * ) "Shell", configMINIMAL_STACK_SIZE*2, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( dumpTask,  ( portCHAR * ) "Dump",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( goTask,    ( portCHAR * ) "Go",    configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( backTask,  ( portCHAR * ) "back",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

    vTaskStartScheduler();

    return 0;

}

