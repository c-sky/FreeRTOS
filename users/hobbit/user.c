#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"

void goTask(void *pvParameters)
{
    uint32_t cnt = 0;

    while(1)
    {
        printf("go task %d\r\n", cnt++);
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}

void backTask(void *pvParameters)
{
    uint32_t cnt = 0;

    while(1)
    {
        printf("back task %d\r\n", cnt++);
        vTaskDelay(50/portTICK_PERIOD_MS);
    }
}



