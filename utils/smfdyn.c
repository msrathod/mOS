/** 
 * @file 	smfdyn.c
 * @author 	Mohit Rathod
 * Created: 28 09 2022, 02:53:01 pm
 * -----
 * Last Modified: 06 10 2022, 10:45:24 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   State Machine Framework(SMF) API
 *          SMF is an application agnostic framework that provides
 *          an easy way to integrate state-machine into an mOS app.
 */
#include "smfdyn.h"
#include <utils/queue.h>

typedef struct
{
    uint8_t stateMax;
    uint8_t eventMax;
    pAction_t action[];
} stateMachine_t;

#define EVENT_QUEUE_LEN         (8)

static qid_t event_qID;
static uint8_t event_qMEM[EVENT_QUEUE_LEN];
static state_t _smState;
static stateMachine_t *pSM;


static int SMF_getEvent(qid_t qID, event_t *pEvent);
static state_t SMF_transition(state_t uState, event_t uEvent);

int SMF_init(state_t iState, uint8_t stateNUM, uint8_t evnetNUM)
{
    q_attr_t attr = {
        .elen = (event_qMEM[0]),
        .qlen = ARRAY_SIZE(event_qMEM),
        .buffer = event_qMEM
    };
    /* set the intitial state of the state machine */
    _smState = iState;
    pSM = malloc(sizeof(stateMachine_t) + stateNUM * evnetNUM * sizeof(pAction_t));
    pSM->stateMax = stateNUM;
    pSM->eventMax = evnetNUM;
    return q_init(&event_qID, &attr);
}

int SMF_addState(state_t uState, stateTransition_t *ptr)
{
    int ret = -1;
    if ((ptr != NULL) && (uState >= STATE_0) && (uState < pSM->stateMax)) {
        int len = ptr->len;
        event_t uEvent;
        while (len--) {
            uEvent = (ptr->pAction+len)->event;
            if ((uEvent >= EVENT_0) && (uEvent < pSM->eventMax)) {
                pSM->action[(uState - STATE_0) * pSM->eventMax +
                            (uEvent - EVENT_0)] = (ptr->pAction+len)->action;
               }
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
    // @todo add checks for vaild state and event variables.
    if (pSM->action[(uState - STATE_0) * pSM->eventMax + (uEvent - EVENT_0)] != NULL) {
        uState = pSM->action[(uState - STATE_0) * pSM->eventMax + (uEvent - EVENT_0)]();
    }
    return uState;
}
