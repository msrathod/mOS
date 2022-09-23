/** 
 * @file 	mossch.c
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 04:31:05 pm
 * -----
 * Last Modified: 23 09 2022, 06:27:40 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   MOSS - A co-operative scheduler for mOS
 * 
 */

#include <mcu.h>
#include <stddef.h>
#include <dev/clock.h>
#include "mossch.h"

/* If invalid number of application tasks configured revert to default value. */
#if (MOS_GET(APP_TASKS) != 0) && \
    (MOS_GET(APP_TASKS) != 1) && \
    (MOS_GET(APP_TASKS) != 2) && \
    (MOS_GET(APP_TASKS) != 3) && \
    (MOS_GET(APP_TASKS) != 4)
#define MOS_APP_TASKS           (2)
#warning "Invalid APP tasks NUM setting, reverting to default taskNUM(2)."
#endif

#define SYS_CLK_FREQ            MOS_GET(MCLK_FREQ)*1000000
#define TICK_RESOLUTION_MS      10


static const uint16_t MaxTASK = 1 + MOS_GET(APP_TASKS);
static const uint16_t SCH_PERIOD = ((((SYS_CLK_FREQ / 4) / 1000)
                                        * TICK_RESOLUTION_MS) - 1);

typedef struct mossch
{
    task_t   pTask;
    uint16_t delay;
    uint16_t period;
    uint16_t run;
}sTask_t;

static sTask_t _tasks[MaxTASK];

int moss_init()
{
    int taskID;
    /* clear the scheduler task structure array. */
    for ( taskID = 0; taskID < MaxTASK; taskID++ ) {
        mossDelTask(taskID);
    }
    /* TIMER_A0 setup */
    /* Halt timer */
    TA0CTL = TACLR + MC_0;
    /* set timer to use SMCLK, clock divider 4, continuous-upmode */
    TA0CTL = TASSEL_2 + ID_2 + MC_2;
    /* TA0CCR0 set to the desired resolution (10 ms) based on 16MHz SMCLK */
    TA0CCR0 = SCH_PERIOD;
    /* Enable CCIE interrupt */
    TA0CCTL0 = CCIE;
    return 0;    
}

int mossAddTask(task_t pFunc, const uint16_t delay, const uint16_t period)
{
    int taskID = 0;
    /* search for an empty slot in the scheduler array */
    while ((_tasks[taskID].pTask != NULL) && (taskID < MaxTASK)) {
        taskID++;
    }
    /* taskID is valid? */
    if (taskID < MaxTASK) {
        _tasks[taskID].pTask = pFunc;
        _tasks[taskID].delay = delay;
        _tasks[taskID].period = period;
        _tasks[taskID].run = 0;
    } else {
        taskID = -1;
    }
    return taskID;
}

int mossDelTask(int taskID)
{
    int ret = -1;
    if (taskID < MaxTASK) {
        _tasks[taskID].pTask = NULL;
        _tasks[taskID].delay = 0;
        _tasks[taskID].period = 0;
        _tasks[taskID].run = 0;
        ret = 0;
    }
    return ret;
}

void mossUpdate()
{
    int taskID;

    for (taskID = 0; taskID < MaxTASK; taskID++) {
        /* Is there a valid task? */
        if (_tasks[taskID].pTask != NULL) {
            if (_tasks[taskID].delay == 0) {
                /* Is task periodic? */
                if (_tasks[taskID].period > 0) {
                    /* Schedule it to run again. */
                    _tasks[taskID].delay = _tasks[taskID].period - 1;
                }
                /* task is due to run now. */
                _tasks[taskID].run++;
            }
            else {
                _tasks[taskID].delay--;
            }
        }
    }
}

void mossRun()
{
    int taskID;
    for (taskID = 0; taskID < MaxTASK; taskID++) {
        if ((_tasks[taskID].pTask != NULL) && (_tasks[taskID].run > 0)) {
            _tasks[taskID].run--;
            _tasks[taskID].pTask();
            /* Is it a one shot task */
            if (_tasks[taskID].period == 0) {
                _tasks[taskID].pTask = NULL;
            }
        }   
    }
}

// Timer A0 interrupt service routine
__attribute__ ((interrupt(TIMER0_A0_VECTOR))) void TimerA0_ISR(void)
{
  /* Clear the interrupt flag */
  TA0CCTL0 &= ~CCIFG;
  mossUpdate();
  TA0CCR0 += SCH_PERIOD;
}