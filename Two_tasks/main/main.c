#include <stdio.h>
#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void task1(void *pvArgs)
{
    while(1)
    {
        printf("\n\tTask 1 running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void task2(void *pvArgs)
{
    while(1)
    {
        printf("\n\tTask 2 running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreate(task1,"task1",2048,NULL,1,NULL);
    xTaskCreate(task2,"task2",2048,NULL,1,NULL);
}
