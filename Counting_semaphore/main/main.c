#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/*
    (Counting semaphore[3])    --> Task1 - Task 1 takes a semaphore after 1 second and then gets suspended                 
                               --> Task2 - Task 2 takes a semaphore after 3 seconds and then gets suspended           
                               --> Task3 - Task 3 takes a semaphore after 5 seconds and then gets suspended           
                               --> Task4 - Task4 tries to take a semaphore after 7 seconds and then gets suspended    

                                                                                                                      
*/

/* Struct created to send multiple  */
typedef struct 
{
    char taskID;
    uint16_t time;
} TaskData;

/* Creation of the argument structures fot the tasks 
   The tasks receive 2 arguments, for that reason a struct must be created
   this struct contains the 2 arguments as members
   A pointer to the corresponding struct must be passed as argument o the xTaskCreate function*/
TaskData Task1_data = {'1', 1000};
TaskData Task2_data = {'2', 5000};
TaskData Task3_data = {'3', 9000};
TaskData Task4_data = {'4', 13000}; 

/* Pointers to the argument structures */
TaskData *pTask1_Data = &Task1_data;
TaskData *pTask2_Data = &Task2_data;
TaskData *pTask3_Data = &Task3_data;
TaskData *pTask4_Data = &Task4_data;

/* handle of the counting semaphore */
SemaphoreHandle_t countingSemaphore = NULL;

/* Task creation
   The four tasks perform the same operation 
   for that reason, a single task definition will be used as template for the 4 tasks*/
void Task(void *pvArgs)
{
    TaskData * data = (TaskData *) pvArgs; // Convert the argument of the task to the same type of the argument struct and assigns it to a pointer of the same type
    char ID = data->taskID; // Assings the ID member to the ID variable
    
    uint16_t time = data->time; // Assings the time member to the time variable
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(time)); // Enter in bocked state for a few seconds
        if(xSemaphoreTake(countingSemaphore, pdMS_TO_TICKS(100))) // Take a semaphpre
        {
            printf("\n\tTask%c has a semaphore...\n", ID);
        }
        else
        {
            printf("\n\tTask%c unable to execute, out of semaphores!\n", ID);
        }
        printf("\tSuspending Task%c\n", ID);
        vTaskSuspend(NULL);
    }
}



void app_main(void)
{
    /* Counting semaphore creation */
    countingSemaphore = xSemaphoreCreateCounting(3, 3);

    xTaskCreate(Task, "Task1", 4096,(void *) pTask1_Data, 3, NULL);
    xTaskCreate(Task, "Task2", 4096,(void *) pTask2_Data, 3, NULL);
    xTaskCreate(Task, "Task3", 4096,(void *) pTask3_Data, 2, NULL);
    xTaskCreate(Task, "Task4", 4096,(void *) pTask4_Data, 3, NULL);

    
}
