/** 
 * @file 	smf.h
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 10:49:14 pm
 * -----
 * Last Modified: 24 09 2022, 11:21:04 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   State Machine Framework(SMF) is an application agnostic
 *          framework that provides an easy way to integrate state-
 *          machine into an mOS application.
 */
#ifndef utils_state_machine_framework_h
#define utils_state_machine_framework_h
#include <stdint.h>

static const uint16_t MaxEvents = 5;
static const uint16_t MaxStates = 5;

/**
 * @brief   Event data type
 * @note    Add enumeration members when num of events
 *          is greater than 8.
 */
typedef enum EventEnum
{
    EVENT_0 = 0x10,
    EVENT_1,
    EVENT_2,
    EVENT_3,
    EVENT_4,
    EVENT_5,
    EVENT_6,
    EVENT_7,
    MAX_Events = EVENT_0 + MaxEvents
} event_t;

/**
 * @brief   State data type
 * @note    Add enumeration members when num of states
 *          is greater than 8.
 */ 
typedef enum StateEnum
{
    STATE_0 = 0x40,
    STATE_1,
    STATE_2,
    STATE_3,
    STATE_4,
    STATE_5,
    STATE_6,
    STATE_7,
    MAX_States = STATE_0 + MaxStates
} state_t;

/**
 * @brief   EventHandler (Action) function prototype
 * 
 */
typedef state_t (*pAction_t)(void);

/**
 * @brief   Event-Action data structure, lists event and its corresponding
 *          action for a given state.
 */
typedef struct 
{
    event_t event;       /* event */
    pAction_t action;    /* eventHandler for the above event */
} eventAction_t;

/**
 * @brief   State Transition data structure, lists all possible event-action
 *          pair for a given state.
 */
typedef struct
{
    eventAction_t *pAction;     /* Pointer to action table for a given state */
    int len;                    /* length of eventAction list for a state */
} stateTransition_t;

/**
 * @brief   State Machine Data structure
 */
typedef struct state_machine
{
    stateTransition_t state[MaxStates];
} stateMachine_t;


int SMF_create( state_t iState, stateMachine_t *pstateMachine );
void SMF_Run(void);
state_t SMF_getState(void);
int SMF_putEvent(const uint8_t *pEvent);
#endif /* utils_state_machine_framework_h */