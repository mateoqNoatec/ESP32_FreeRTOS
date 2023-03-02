#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>

SemaphoreHandle_t Mutex = NULL; // Handle for mutex

TaskHandle_t HTask1;
TaskHandle_t HTask2; 

void Task1(void *pvAgrs)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if(xSemaphoreTake(Mutex, pdMS_TO_TICKS(100)))
        {
            printf("\n\tMutex taken by Task1\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            xSemaphoreGive(Mutex);
        }
        else
        {
            printf("\n\tUnable to take Mutex, it has been taken by Task2\n");
        }
    }
}

void Task2(void *pvAgrs)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(7000));
        if(xSemaphoreTake(Mutex, pdMS_TO_TICKS(10)))
        {
            printf("\n\tMutex taken by Task2\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            xSemaphoreGive(Mutex);
        }
        else
        {
            printf("\n\tUnable to take Mutex, it has been taken by Task3\n");
        }
    }
}


void app_main(void)
{
    Mutex = xSemaphoreCreateMutex();
    xTaskCreate(Task1, "Task1", 4096, NULL, 3, &HTask1);
    xTaskCreate(Task2, "Task2", 4096, NULL, 3, &HTask2);
    

}
