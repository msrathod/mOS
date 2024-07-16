/** 
 * @file 	smf.c
 * @author 	Mohit Rathod
 * Created: 26 09 2022, 01:49:19 pm
 * -----
 * Last Modified: 26 09 2022, 04:01:45 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   State Machine Framework API source
 * 
 */
#include "smf.h"
#include <utils/queue.h>

typedef struct
{
    pAction_t action[STATESMAX][EVENTSMAX];
} stateMachine_t;


#define EVENT_QUEUE_LEN         (8)

static qid_t event_qID;
static uint8_t event_qMEM[EVENT_QUEUE_LEN];
static state_t _smState;
static stateMachine_t SM;


static int SMF_getEvent(qid_t qID, event_t *pEvent);
static state_t SMF_transition(state_t uState, event_t uEvent);

int SMF_init(state_t iState)
{
    q_attr_t attr = {
        .elen = (event_qMEM[0]),
        .qlen = ARRAY_SIZE(event_qMEM),
        .buffer = event_qMEM
    };
    /* set the intitial state of the state machine */
    _smState = iState;
    return q_init(&event_qID, &attr);
}

int SMF_addState(state_t uState, evAction_t *ptr, size_t len)
{
    int ret = -1;
    if ((ptr != NULL) && (uState >= STATE_0) && (uState < MAX_States)) {
        while (len--) {
            SM.action[uState - STATE_0][((ptr+len)->event) - EVENT_0] 
                                        = (ptr+len)->action;
        }
        ret = 0;
    }
    return ret;
}

state_t SMF_getState()
{
    return _smState;
}

int SMF_putEvent(const uint8_t *pEvent)
{
    return qEnqueue(event_qID, pEvent);
}

void SMF_Run()
{
    event_t uEvent;
    if (SMF_getEvent(event_qID, &uEvent) == 0) {
        _smState = SMF_transition( _smState, uEvent);
    }
}

static int SMF_getEvent(qid_t qID, event_t *pEvent)
{
    uint8_t tmp;
    int ret = qDequeue(qID, &tmp);
    *pEvent = (event_t)tmp;
    return ret;
}

static state_t SMF_transition(state_t uState, event_t uEvent)
{
    if (SM.action[uState - STATE_0][uEvent - EVENT_0] != NULL) {
        uState = SM.action[uState - STATE_0][uEvent - EVENT_0]();
    }
    return uState;
}
