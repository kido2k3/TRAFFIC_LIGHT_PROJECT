/*
 * scheduler.c
 *
 *  Created on: Nov 18, 2023
 *      Author: HP
 */

#include "my_scheduler.h"

/*
@brief: pTask - pointer pointing to the function that will run
		counter - the amount of remaining time or the time after the previous task
		period - the amount of cycle time to run task
		next_task - pointer pointing to the next task
*/
struct task
{
    void (*pTask)();
    uint16_t counter;
    uint16_t period;
    struct task *next_task;
};
/*
@brief: top - the top of task stack
*/
struct
{
    struct task *top;
    // struct task *bottom;
    // uint32_t time_length;
} stack_task;
// struct task *dispatcher;
/*
 * @brief:	add a new task to the task stack
 * @para:	pTask - pointer pointing to the function that will run
			delay - the amount of time after which the task will run
			period - the amount of cycle time to run task
 * @retval:	1 - add successfully
 * 			0 - add badly
 * */
bool sch_add_task(void (*pTask)(), uint16_t delay, uint16_t period)
{
    struct task *my_task = (struct task *)malloc(sizeof(struct task));
    my_task->pTask = pTask;
    my_task->counter = delay *FREQUENCY_OF_TIM/1000;
    my_task->period = period;
    my_task->next_task = 0;
    if (stack_task.top == 0)
    {
        stack_task.top = my_task;
        // stack_task.bottom = stack_task.top;
        // stack_task.time_length = stack_task.top->counter;
        return 1;
    }
    // if (delay >= stack_task.time_length)
    // {
    //     my_task->counter = delay - stack_task.time_length;
    //     stack_task.bottom->next_task = my_task;
    //     stack_task.bottom = stack_task.bottom->next_task;
    //     stack_task.time_length += my_task->counter;
    //     return 1;
    // }
    struct task *pre = stack_task.top;
    struct task *cur = stack_task.top;
    while (cur && my_task->counter >= cur->counter)
    {
        my_task->counter = my_task->counter - cur->counter;
        pre = cur;
        cur = cur->next_task;
    }
    if (pre != cur)
    {
        pre->next_task = my_task;
        my_task->next_task = cur;
    }
    else
    {
        my_task->next_task = cur;
        stack_task.top = my_task;
    }
    if (cur)
        cur->counter -= my_task->counter;
    return 1;
}
/*
 * @brief:	decrease the remaining time of the top task
 * @para:	none
 * @retval:	none
 * */
void sch_update(void)
{
    if (stack_task.top == 0)
        return;
    if (stack_task.top->counter > 0)
    {
        stack_task.top->counter--;
    }
}
/*
 * @brief:	delete a task in the stack and release the memory
 * @para:	del_task - the task need to be deleted
 * @retval:	none
 * */
void sch_delete_task(struct task *del_task)
{
    if (del_task == 0)
    {
        return;
    }
    free(del_task);
}
/*
 * @brief:	run the top task and update if over remainning time
 * @para:	none
 * @retval:	1 - run succesfully
 *			0 - run badly
 * */
bool sch_dispatch(void)
{
    if (stack_task.top == 0)
        return 0;
    if (stack_task.top->counter == 0)
    {
        (*stack_task.top->pTask)();
        struct task *temp = stack_task.top;
        stack_task.top = stack_task.top->next_task;
        temp->next_task = 0;
        if (temp->period != 0)
        {
            sch_add_task(temp->pTask, temp->period, temp->period);
        }
        sch_delete_task(temp);
        return 1;
    }
    return 0;
}
