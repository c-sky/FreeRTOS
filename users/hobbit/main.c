#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


extern void goTask(void *pvParameters);
extern void backTask(void *pvParameters);
extern void SystemInit(void);
/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Function
 *          void CKEnableInterrupt(uint32_t irq)
 * Description
 *          Enable interrupt
 * Parameter
 *          irq             irq number
 * Return
 *          None
 ****************************************************************************/

static void preSetupHardware(void)
{
    SystemInit();
}

/****************************************************************************
 * void main (void)
 ****************************************************************************/
int main(void)
{
    preSetupHardware();

    xTaskCreate( goTask,    ( portCHAR * ) "Go",    configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
    xTaskCreate( backTask,  ( portCHAR * ) "back",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );

    vTaskStartScheduler();

    return 0;

}

