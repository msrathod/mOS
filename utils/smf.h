/** 
 * @file 	smf.h
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 10:49:14 pm
 * -----
 * Last Modified: 26 09 2022, 04:01:35 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   State Machine Framework(SMF) API
 *          SMF is an application agnostic framework that provides
 *          an easy way to integrate state-machine into an mOS app.
 */
#ifndef utils_state_machine_framework_h
#define utils_state_machine_framework_h
#include <stdint.h>
#include <stddef.h>
#include <mosconfig.h>

#if MOS_GET(SMF_MAX_EVENTS)
#define EVENTSMAX               MOS_GET(SMF_MAX_EVENTS)
#else
#define EVENTSMAX               (5)
#endif
#if MOS_GET(SMF_MAX_STATES)
#define STATESMAX               MOS_GET(SMF_MAX_STATES)
#else
#define STATESMAX               (5)
#endif

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
    MAX_Events = EVENT_0 + EVENTSMAX
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
    MAX_States = STATE_0 + STATESMAX
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
} evAction_t;

/**
 * @fn      int SMF_init(state_t iState);
 * @brief   Initialize the State Machine Framework, also sets up the initial
 *          state of the generic state machine.
 * @param   iState  initial state of the state machine to be implemented.
 * @return  0 on success, -1 otherwise
 */
int SMF_init(state_t iState);

/**
 * @fn      int SMF_addState(state_t uState, stateTransition_t *ptr);
 * @brief   Add a state and its transition rules to the SM Framework
 * @param   uState  the state to add.
 * @param   ptr     pointer to state transition table of the above state. 
 * @return  0 on success, -1 otherwise
 */
int SMF_addState(state_t uState, evAction_t *ptr, size_t len);

/**
 * @fn      state_t SMF_getState(void);
 * @brief   Fetches the current state of the State Machine implemented
 *          via the SM Framework.
 * @return  state 
 */
state_t SMF_getState(void);

/**
 * @fn      int SMF_putEvent(const uint8_t *pEvent);
 * @brief   Add an event on the SM Framework event queue.
 * @param   pEvent Event to add
 * @return  0 on success, -1 otherwise
 */
int SMF_putEvent(const uint8_t *pEvent);

/**
 * @fn      void SMF_Run(void);
 * @brief   SM Manager for the SM Framework. This must be
 *          called upon periodically. It performs the transition
 *          of the State Machine based on events from the event
 *          queue.
 */
void SMF_Run(void);
#endif /* utils_state_machine_framework_h */