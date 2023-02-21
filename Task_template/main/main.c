#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

int task1ID=1;
int task2ID=2;

void Task(void *pvArgs)
{
    int counter = (int)pvArgs;
    while(1)
    {
        printf("\n\tTask %d running...\n", counter);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    xTaskCreate(Task,"task1",20487,task1ID, 1, NULL);
    xTaskCreate(Task,"task2",20487,task2ID, 1, NULL);
}
