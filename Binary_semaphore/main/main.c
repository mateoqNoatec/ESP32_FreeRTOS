#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/gpio.h>

/*¨
            Task1 --> (Binary Semaphore) --> Task2
              |                                |
              |                                |
              v                                v
        blocked until                      Blocked until
       3 seconds passed                 Task1 gives semaphore
       Then gives the semaphore         Then unblocks and 
                                         executes itself
*/

SemaphoreHandle_t binarySemaphoreHandle = NULL;

void task1(void *pvArgs)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(3000));
        xSemaphoreGive(binarySemaphoreHandle);
    }
}

void task2(void *pvArgs)
{
    while(1)
    {
        if(xSemaphoreTake(binarySemaphoreHandle, pdMS_TO_TICKS(100)))
        {
            printf("\n\tTask 2 obtained he semaphore!\n");
        }
    }
}

void app_main(void)
{
    /* Creating binary semaphore */
    binarySemaphoreHandle = xSemaphoreCreateBinary();

    /* Task creations */
    xTaskCreate(task1,"task1",4096,NULL,1,NULL);
    xTaskCreate(task2,"task2",4096,NULL,1,NULL);
}