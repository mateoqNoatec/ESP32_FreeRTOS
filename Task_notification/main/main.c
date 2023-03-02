#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


/*

        [Task1]          ---Task Notification--->          [Task2]
    (after 3 secs)
*/ 

TaskHandle_t Htask1 = NULL;
TaskHandle_t Htask2 = NULL;

void task1(void *pvArgs)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        xTaskNotifyGive(Htask2);
    }
}

void task2(void *pvArgs)
{
    while(1)
    {
        int notifications = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        printf("\n\tTask2 has received a notification from Task1.\n");
    }
}


void app_main(void)
{
    xTaskCreate(task1, "Task1", 4096, NULL, 2, &Htask1); // IMPORTANT, the handler mst be passed to the task creation function with an &
    xTaskCreate(task2, "Task2", 4096, NULL, 2, &Htask2);
}
