#include <stdio.h>
#include <stdlib.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>

#define EVENTBit_TASK1 1<<1
#define EVENTBit_TASK2 1<<2
#define EVENTBit_TASK3 1<<4

/*
            Task1           Task2           Task3
              |               |               | 
              |               |               |
              |               |               |
               \              |              /
                \             |             /
                 \            |            /
                  \           |           /
                   \          v          /
                     -->[Event Group]<--
                              |
                              |
                              v
                            Task 4

configUSE_16_BIT_TICKS=0 --> this means that the event group has 24 bits(flags)

                
                                               <<< Event Group Configuration >>>
         _____________________________________________________________________________________________________________________________                                            
bit --> |  24  |  23  |  22  |  21  |  20  |   ... | 13  |  12 |  11 |  10 |  9 |  8  |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  | --> 22
task--> |_____ |_____ |_____ |_____ |_____ |_______|_____|_____|_____|_____|____|____ |_____|____ |____ |Task3|____ |Task2|Task1|____ |
    
*/

EventGroupHandle_t EventGroupH; // Handle for an event group

void eventGroupCreation(); // Function prototype to create the event group
void tasksCreation();      // Function prototype to create all the tasks

/* Struct created to pass multiple arguments to a task */
typedef struct 
{
    int bit;
    int time; // Time in ms
}Task_Args_t;

Task_Args_t task1_conf = {EVENTBit_TASK1,1000}; // Arguments for task 1
Task_Args_t task2_conf = {EVENTBit_TASK2,3000}; // Arguments for task 2
Task_Args_t task3_conf = {EVENTBit_TASK3,7000}; // Arguments for task 3

void Task(void *pvArgs)
{
    Task_Args_t *data = (Task_Args_t *)pvArgs;
    int bit = data->bit;
    int time = data->time;

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(time));
        EventBits_t groupState = xEventGroupSetBits(EventGroupH, bit);
        printf("\n\tEvent group state: %d\n", (int)groupState);
        vTaskSuspend(NULL);

    }
}

void mainTask(void *pvArgs)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(10)); // delay that avoids mains task starving IDLE task
        if(xEventGroupGetBits(EventGroupH)==0x16)
        {
            printf("\n\tMain task executed!\n");
            vTaskSuspend(NULL);
        }
    }
}



void app_main(void)
{
    eventGroupCreation();
    tasksCreation();
}








void eventGroupCreation()
{
    EventGroupH = xEventGroupCreate();
    if(EventGroupH==NULL)
    {
        printf("\n\tInsufficient heap memory to create Event Group!\n");
    }
    else
    {
        printf("\t\nEvent group succesfully created!\n");
    }
}

void tasksCreation()
{
    xTaskCreate(Task, "Task1", 4096, &task1_conf, 3, NULL);
    xTaskCreate(Task, "Task2", 4096, &task2_conf, 3, NULL);
    xTaskCreate(Task, "Task3", 4096, &task3_conf, 3, NULL);
    xTaskCreatePinnedToCore(mainTask, "mainTask", 4096, NULL, 5, NULL, 1);
}