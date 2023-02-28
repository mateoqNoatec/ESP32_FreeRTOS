#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>

/*
    (Counting semaphore[3])    --> Task1 - Task 1 takes a semaphore after 1 second             --- (queue) ---|    
                               --> Task2 - Task 2 takes a semaphore after 3 seconds            --- (queue) ---|--> Task_control
                               --> Task3 - Task 3 takes a semaphore after 5 seconds            --- (queue) ---|
                               --> Task4 - Task4 tries to take a semaphore after 7 seconds     --- X ---

                                                                                                                       When a task gets a semaphore, sends its number ID to Task control
                                                                                                                       Task control suspends the corresponding task
*/

TaskHandle_t Task1_handle=NULL;
TaskHandle_t Task2_handle=NULL;
TaskHandle_t Task3_handle=NULL;
TaskHandle_t Task4_handle=NULL;

/* Queue that connets tasks with task control  */
QueueHandle_t queue = NULL;

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
TaskData Task2_data = {'2', 3000};
TaskData Task3_data = {'3', 5000};
TaskData Task4_data = {'4', 7000}; 

/* Pointers to the argument structures */
TaskData *pTask1_Data = &Task1_data;
TaskData *pTask2_Data = &Task2_data;
TaskData *pTask3_Data = &Task2_data;
TaskData *pTask4_Data = &Task2_data;

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
    //TaskHandle_t handleTask = data->handleTask;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(time)); // Enter in bocked state for a few seconds
        if(xSemaphoreTake(countingSemaphore, pdMS_TO_TICKS(100))) // Take a semaphpre
        {
            printf("\n\tTask%c executing...\n", ID);
            int nID=(int)ID;
            if(xQueueSend(queue,&ID, pdMS_TO_TICKS(1000)))
            {
                printf("\n\tID succesfully sent from Task%c\n", ID);
            }
        }
    
        else
        {
            printf("\n\tTask%c unable to execute, out of semaphores!\n", ID);
        }
    }
}

/* Control task creation - this task receives the ID of the tasks once they take the semaphore
   When received an ID, this task suspends the corresonding task */
void control_task(void *pvArgs)
{   
    printf("\n\tInside control task\n");
    int taskID;
    //char *ptaskID = &taskID;
    BaseType_t receiveState = xQueueReceive(queue, &taskID,pdMS_TO_TICKS(10));
    printf("\n\tData received in control task\n");
    printf("\n\tThe data received in control task is: %d\n",(int)taskID);
    while(1)
    {
        if(receiveState == pdTRUE)
        {

            if(taskID == 1)
            {
                vTaskSuspend(Task1_handle);
            }
            if(taskID == 2)
            {
                vTaskSuspend(Task2_handle);
            }
            if(taskID == 3)
            {
                vTaskSuspend(Task3_handle);
            }
            if(taskID == 4)
            {
                vTaskSuspend(Task4_handle);
            }
        }
        else{
            //printf("\n\tUnable to receive data from queue\n");
        }
    vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main(void)
{
    /* Counting semaphore creation */
    countingSemaphore = xSemaphoreCreateCounting(3, 3);

    /* Queue creation */
    queue = xQueueCreate(5,sizeof(int));

    xTaskCreate(Task, "Task1", 4096,(void *) pTask1_Data, 5, NULL);

    xTaskCreate(control_task, "control_task", 4096, NULL, 7, NULL);
}
