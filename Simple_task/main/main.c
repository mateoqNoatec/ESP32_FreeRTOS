#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void task(void *pvargs)
{
    while(1)
    {
        printf("\n\tTasj running...\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}



void app_main(void)
{
    xTaskCreate(task,"task",2048, NULL,10,NULL);

}
