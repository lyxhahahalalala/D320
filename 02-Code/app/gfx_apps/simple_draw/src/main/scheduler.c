#include "r_typedefs.h"
#include "string.h"
#include "scheduler.h"
#include "ptmr.h"

typedef struct
{
    pTask Task;      /* Pointer to the task function */
	
    uint32_t Period; /* Interval (ticks) between subsequent runs */
	
    uint32_t Timer;  /* Timer (ticks) */
	
} TASK_TYPE;

#define MAX_TASKS  (100u)
static struct
{
    uint16_t  TasksCount;
    TASK_TYPE Tasks[MAX_TASKS];
} s_task;

/** 任务调度初始化 **/
void Scheduler_Init(void)
{
    s_task.TasksCount = 0;
    memset(&s_task.Tasks, 0, sizeof(s_task.Tasks));
	
}

/** 查找指定任务 **/
uint16_t Scheduler_FindTask(const pTask task)
{
    uint16_t index;
    uint16_t ret = 0xff;
    /* First find a gap in the array (if there is one) */
    for(index = 0u; index < s_task.TasksCount; index++)
    {
        /* if find that task, return the index */
        if(s_task.Tasks[index].Task == task)
        {
            ret = index;
            break;
        }
    }
    return ret;
}

/** 添加一个任务 **/
uint16_t Scheduler_AddTask(const pTask task, const uint16_t period)
{
    uint16_t index, top;

    do
    {
        /* if already added, return success */
        index = Scheduler_FindTask(task);
        if(index != 0xFFu)  /*如果该任务存在*/
        {
            index = 0x00u;  /*已经成功添加 任务*/
            break;
        }

        /* if no space, return failed */
        if(s_task.TasksCount == MAX_TASKS)
        {
            index = 0xFFu; /*添加任务失败*/
            break;
        }
        /* add a new task at top */
        top = s_task.TasksCount++;
        s_task.Tasks[top].Task = task;
        s_task.Tasks[top].Period = period;//1MS
		ptmr_Start(&s_task.Tasks[top].Timer);
        /* indicate add success */
        index = 0x00u;
    }
    while(0);
    return index;
}

/** 删除指定的任务 **/
void Scheduler_DeleteTask(const pTask task)
{
    uint16_t index, top;

    do
    {
        /* if not added, do nothing */
        index = Scheduler_FindTask(task);
        if(index == 0xFFu)
        {
            break; /*任务不存在*/
        }
        /* replace top task to this task */
        top = --s_task.TasksCount;
        if(index != top)
        {
            s_task.Tasks[index].Task = s_task.Tasks[top].Task;
            s_task.Tasks[index].Timer = s_task.Tasks[top].Timer;
            s_task.Tasks[index].Period = s_task.Tasks[top].Period;
        }
		
    }
    while(0);
}

/** 任务调度处理 **/
void Scheduler_DispatchTasks(void)
{
    uint16_t index=0;

    /* Dispatches (runs) the next task (if one is ready) */
    for (index = 0; index < s_task.TasksCount; index++)
    {
        if(!ptmr_IsStart(&s_task.Tasks[index].Timer))
		{
			ptmr_Start(&s_task.Tasks[index].Timer);
		}
		else if(ptmr_Get(&s_task.Tasks[index].Timer) >= s_task.Tasks[index].Period)
        {
            ptmr_Start(&s_task.Tasks[index].Timer);
            s_task.Tasks[index].Task();                  /* Run the task  */
			
        }
    }
}



