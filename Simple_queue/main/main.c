#include <stdio.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

QueueHandle_t queueHandle;

void task1 (void *pvArgs)
{
    int data = 1;
    while(1)
    {
        xQueueSend(queueHandle, &data, pdMS_TO_TICKS(1000));
        vTaskDelay(pdMS_TO_TICKS(500));
        if(data==100)
        {
            data=0;
        }
        data++;
        
    }
}

void task2(void *pvArgs)
{
    int data=0;
    while(1)
    {
        if(xQueueReceive(queueHandle, &data, pdMS_TO_TICKS(1000)))
        {
            printf("\n\tData received: %d\n", data);
        }
    }
}

void app_main(void)
{
    queueHandle = xQueueCreate(5,sizeof(int));
    xTaskCreate(task1, "task1", 2048, NULL,1,NULL);
    xTaskCreate(task2, "task2", 2048, NULL,1,NULL);
}
