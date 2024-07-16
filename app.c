/** 
 * @file 	app.c
 * @author 	Mohit Rathod
 * Created: 24 09 2022, 05:52:24 pm
 * -----
 * Last Modified: 15 07 2024, 04:16:36 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   User Application
 * 
 */
#include <mcu.h>
#include <mos.h>
#include <utils/errmos.h>
#include <utils/icsserver.h>


// Service functions
uint8_t srvc_port0(void *pargs);
uint8_t srvc_port1(void *pargs);
uint8_t srvc_port2(void *pargs);

uint8_t var_p1[1];
uint8_t var_p2[1];

//sunroof state machine setup
#include <utils/smf.h>
#include <dev/md13s.h>

/* Desired debouncing time in ms */
#define DEBOUNCE_TIME_MS    10
/* Debounce period register value to be fed to TACCRx */
const uint16_t debounce_period = ((((MOS_GET(MCLK_FREQ) * 1000)/4)*DEBOUNCE_TIME_MS) - 1);
/* Map sunroof events to StateMachine */
#define EV_OPEN       EVENT_0
#define EV_CLOSE      EVENT_1
#define EV_STOP       EVENT_2
#define EV_FSM_RST    EVENT_3
#define EV_LIMIT      EVENT_4

/* Map sunroof states to StateMachine */
#define ST_OPEN       STATE_0
#define ST_OPENING    STATE_1
#define ST_CLOSE      STATE_2
#define ST_CLOSING    STATE_3
#define ST_STOPPED    STATE_4

stateTransition_t Transition;

state_t openEvent_Handler(void)
{
  md13s_run(MD_CW);
  MPRINT("\nOpen Event Handler.");
  return ST_OPENING;
}

state_t closeEvent_Handler(void)
{
  md13s_run(MD_CCW);
  MPRINT("\nClose Event Handler.");
  return ST_CLOSING;
}

state_t stopEvent_Handler(void)
{
  md13s_stop();
  MPRINT("\nStop Event Handler.");
  return ST_STOPPED;
}

state_t limit_openEvent_Handler(void)
{
  md13s_stop();
  MPRINT("\nLimit(o)Event Handler.");
  return ST_OPEN;
}

state_t limit_closeEvent_Handler(void)
{
  md13s_stop();
  MPRINT("\nLimit(c)Event Handler.");
  return ST_CLOSE;
}

state_t resetEvent_Handler(void)
{
  MPRINT("\nReset Event Handler.");
  return ST_CLOSE;
}

void setup()
{
    errmos = ICSserver_init();
    EPRINT("\nI2C server initialization");
    errmos = ICS_addService(srvc_port0, 0, NULL, PORT_0);
    EPRINT("\nAdding service to port 0 of ICS server.");
    errmos = ICS_addService(srvc_port1, 1, var_p1, PORT_1);
    EPRINT("\nAdding service to port 1 of ICS server.");
    errmos = ICS_addService(srvc_port2, 1, var_p2, PORT_2);
    EPRINT("\nAdding service to port 2 of ICS server.");

        /* event action pair for ST_OPEN case */
    static evAction_t eAST_Open[] = {
                                  {EV_CLOSE, closeEvent_Handler},
                                  {EV_STOP, stopEvent_Handler}
                                };

    /* event action pair for ST_CLOSE case */
    static evAction_t eAST_Close[] =  {
                                    {EV_OPEN, openEvent_Handler},
                                    {EV_STOP, stopEvent_Handler}
                                  };
    
    /* event action pair for ST_OPENING case */
    static evAction_t eAST_Opening[] ={
                                    {EV_LIMIT, limit_openEvent_Handler},
                                    {EV_CLOSE, closeEvent_Handler},
                                    {EV_STOP, stopEvent_Handler}
                                  };
    
    /* event action pair for ST_CLOSING case */
    static evAction_t eAST_Closing[] ={
                                    {EV_LIMIT, limit_closeEvent_Handler},
                                    {EV_OPEN, openEvent_Handler},
                                    {EV_STOP, stopEvent_Handler}
                                  };
    
    /* event action pair for ST_STOPPED case */
    static evAction_t eAST_Stopped[] ={ {EV_FSM_RST, resetEvent_Handler} };

    errmos = SMF_init(ST_CLOSE);
    EPRINT("\nState Machine Initialization");

    errmos = SMF_addState(ST_CLOSE, eAST_Close, ARRAY_SIZE(eAST_Close));
    EPRINT("\nAdding ST_CLOSE to SMF");
    
    errmos = SMF_addState(ST_OPEN, eAST_Open, ARRAY_SIZE(eAST_Open));
    EPRINT("\nAdding ST_OPEN to SMF");
    
    errmos = SMF_addState(ST_OPENING, eAST_Opening, ARRAY_SIZE(eAST_Opening));
    EPRINT("\nAdding ST_OPENING to SMF");

    errmos = SMF_addState(ST_CLOSING, eAST_Closing, ARRAY_SIZE(eAST_Closing));
    EPRINT("\nAdding ST_CLOSING to SMF");
    
    errmos = SMF_addState(ST_STOPPED, eAST_Stopped, ARRAY_SIZE(eAST_Stopped));
    EPRINT("\nAdding ST_STOPPED to SMF");
    
    mossAddTask(SMF_Run, 20, 50);
}

void loop()
{
    ICS_run();
}

uint8_t srvc_port0(void *pargs)
{
    (void) pargs;
    return SMF_getState();
}

uint8_t srvc_port1(void *pargs)
{
    uint8_t *args = (uint8_t*)pargs;
    return (uint8_t)SMF_putEvent(args);
}

uint8_t srvc_port2(void *pargs)
{
    uint8_t *args = (uint8_t*)pargs;
    md13s_setDuty(*args);
    return 0;
}

/**
 * @brief Interrupt servie routine for sensors(buttons)
 * 
 * @return * __attribute__() void Port1_ISR(void) 
 */
__attribute__((interrupt(PORT1_VECTOR))) void Port1_ISR(void)
{
    /* Button ISR */
    if(P1IFG & BIT3)
    {
        P1IE &= ~BIT3;
        P1IFG &= ~(BIT3);
        TA0CCR1 = TA0R + debounce_period;
        TA0CCTL1 = CCIE;
    }
    /* Limit ISR for State Machine */
    if(P1IFG & BIT4)
    {
        P1IE &= ~BIT4;
        P1IFG &= ~(BIT4);
        TA0CCR2 = TA0R + debounce_period;
        TA0CCTL2 = CCIE;
    }
}

/**
 * @brief   Interrupt service routine for debouncing mechanism.
 * 
 * @return  __attribute__() void Timer0A1_ISR(void) 
 */
__attribute__ ((interrupt(TIMER0_A1_VECTOR))) void Timer0A1_ISR(void)
{
    uint8_t evnt;
    switch(TA0IV)
    {
        case TA0IV_TACCR1:
            TA0CCTL1 &= ~CCIFG;
            TA0CCTL1 &= ~CCIE;
            if((P1IN & BIT3)==0)
            {
              /* Push a reset event on the SM queue */
                evnt = EV_FSM_RST;
                SMF_putEvent(&evnt);
            }
            P1IFG &= ~(BIT3);
            P1IE |= BIT3;
        break;

        case TA0IV_TACCR2:
            TA0CCTL2 &= ~CCIFG;
            TA0CCTL2 &= ~CCIE;
            if((P1IN & BIT4)==0)
            {
                /* Push a limits event on the SM queue */
                evnt = EV_LIMIT;
                SMF_putEvent(&evnt);
            }
            P1IFG &= ~(BIT4);
            P1IE |= BIT4;
        break;

        case TA0IV_TAIFG:

        break;
    }
}