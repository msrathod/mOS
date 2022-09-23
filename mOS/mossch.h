/** 
 * @file 	mossch.h
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 10:32:25 am
 * -----
 * Last Modified: 23 09 2022, 06:16:24 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   MOSS - A co-operative scheduler for mOS.
 * @note    Based on the works of Michael J. Pont in "Patterns for Time
 *          Triggered Embeded Systems".
 * @details The co-operative scheduler implemented here provides a
 *          single-tasking system architecture and its operates as
 *          follows:
 *          -> Tasks are scheduled to run at specific times
 *              (either on a periodic or one-shot basis)
 *          -> When a task is scheduled to run it is added 
 *              to the waiting list.
 *          -> When the CPU is free, the next waiting task
 *              (if any) is executed.
 *          -> The task runs to completion, then returns
 *              control to the scheduler.
 */
#ifndef mos_scheduler_h
#define mos_scheduler_h
#include <mosconfig.h>
#include <stdint.h>

/**
 * @typedef typedef void (*task_t)(void);
 * @brief   typedef for scheduler tasks.
 */
typedef void (*task_t)(void);

/**
 * @fn      int moss_init(void);
 * @brief   mOS-Scheduler[mOSS] initialization.
 * @param   void
 * @return  0 on success, -1 otherwise
 */
int moss_init(void);

/**
 * @fn      int mossAddTask(task_t, const uint16_t, const uint16_t);
 * @brief   Add task to mOS-Scheduler[mOSS].
 * @param   pFunc  task to add to the scheduler.
 * @param   delay  interval(TICKS) before the task is first executed.
 * @param   period period(TICKS) between successive task execution.
 * @return  taskID on success, -1 otherwise
 * @note    All pFunc should be of type @ref task_t. If 'period' is set
 *          to 0, the task is executed only once at a time determined by
 *          the 'delay' parameter. If 'period' is non-zero, then the task
 *          is executed periodically with a periodic interval of 'period'
 * 
 * @note:   The return value 'taskID' may be required, if a task is to be
 *          subsequently deleted - @see mossDelTask(int taskID)
 *
 * @example:
 * Task_ID = mossAddTask(Do_X,1000,0);
 * Causes the function Do_X() to be executed once after 1000 sch ticks.
 *
 * Task_ID = mossAdd_Task(Do_X,0,1000);
 * Causes the function Do_X() to be executed regularly, every 1000 sch ticks.
 *
 * Task_ID = mossAdd_Task(Do_X,300,1000);
 * Causes the function Do_X() to be executed regularly, every 1000 ticks.
 * Task will be first executed at T = 300 ticks, then 1300, 2300, etc.

 */
int mossAddTask(task_t pFunc, const uint16_t delay, const uint16_t period);

/**
 * @fn      int mossDelTask(int taskID);
 * @brief   Remove task from mOS-Scheduler[mOSS].
 * @param   taskID task identifier of the task to remove. 
 * @return  0 on success, -1 otherwise.
 */
int mossDelTask(int taskID);

/**
 * @fn      void mossUpdate(void);
 * @brief   Scheduler ISR.
 *          It is called at a rate determined by the timer setting
 *          used in the 'moss_init' function.
 * @param   void
 * @return  void
 */
void mossUpdate(void);

/**
 * @fn      void mossRun(void);
 * @brief   Task dispatcher for mOS-Scheduler[mOSS].
 * @param   void
 * @return  void
 */
void mossRun(void);

#endif /* mos_scheduler_h */