

#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_


#define TASK_PERIOD_MS(t_ms)  (t_ms)

typedef void (* pTask)(void);


void Scheduler_Init(void);	/*任务调度初始化*/
uint16_t Scheduler_FindTask(const pTask task); /*查找指定任务*/
uint16_t Scheduler_AddTask(const pTask task, const uint16_t period); /*添加一个任务*/
void Scheduler_DeleteTask(const pTask task); /*删除指定的任务*/
void Scheduler_DispatchTasks(void); /*任务调度处理*/


#endif      /* __SCHEDULER_H_ */



