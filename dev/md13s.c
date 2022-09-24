/** 
 * @file 	md13s.c
 * @author 	Mohit Rathod
 * Created: 23 09 2022, 09:21:29 pm
 * -----
 * Last Modified: 24 09 2022, 05:55:34 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Driver for MD13s motor controller
 * 
 */
#include <mcu.h>
#include <mosdefs.h>
#include <dev/clock.h>
#include <dev/boardconfig.h>
#include "md13s.h"

/* If not configured or invalid freq configured, revert to default freq */
#if !MOS_GET(MD13S_FREQ)
static const uint16_t PWM_freq = 2000;      /* default */
#warning "MD13S Frequency not set, reverting to default freq(2 KHz)."
#elif MOS_GET(MD13S_FREQ) < 250             
static const uint16_t PWM_freq = 250;       /* floor */
#warning "MD13S Frequency too low, reverting to lowest freq(250 Hz)."
#elif MOS_GET(MD13S_FREQ) > 20000
static const uint16_t PWM_freq = 20000;     /* ceil */
#warning "MD13S Frequency too high, reverting to highest freq(20 KHz)."
static_assert(MOS_GET(MCLK_FREQ) != 1, "Error: f_pwm > 10KHz requires a clock freq > 1 MHz. Please configure clock to a higher freq or lower the pwm freq below 10 KHz.");
#else
static const uint16_t PWM_freq = MOS_GET(MD13S_FREQ);
#if MOS_GET(MD13S_FREQ) > 10000
    static_assert(MOS_GET(MCLK_FREQ) != 1, "Error: f_pwm > 10KHz requires a clock freq > 1 MHz. Please configure clock to a higher freq or lower the pwm freq below 10 KHz.");
#endif
#endif


/* the timer count to generate the desired frequency */
static const uint16_t PWM_count = MOS_GET(MCLK_FREQ) * 1000000 / PWM_freq;
static const uint16_t DUTY_factor = PWM_count / 100;
/* in % */
static const uint16_t DEFAULT_Duty = 75;
static const uint16_t MAX_Duty = 90;
static const uint16_t MIN_Duty = 10;

#if defined(MD13S_DIR)
#define DIR_PIN             MD13S_DIR
#elif MOS_USES(MD13S) == 1
#error "MD13S Pins not assigned. Driver requires MD13S_DIR and MD13S_PWM pin be assigned to appropriate GPIO pins."
#else
#define DIR_PIN             BIT2
#endif


int md13s_init()
{
    TA1CTL = TACLR + MC_0;                      // Halt timer and clear TAR
    TA1CCR0 = PWM_count - 1;                    // PWM Period @ SMCLK
    TA1CCR1 = DUTY_factor*DEFAULT_Duty - 1;     // CCR1 PWM duty cycle
    TA1CCTL1 &= ~(OUT);                         // set OUT to 0
    TA1CCTL1 = OUTMOD_0;                        // Outputs the OUT bit value
    TA1CTL = TASSEL_2 + ID_0 + MC_1;            // SMCLK, up mode
    return 0;
}

void md13s_run(mCmd_t cmd)
{
    switch (cmd)
    {
        case MD_CW:
            P2OUT &= ~(DIR_PIN);
            TA1CCTL1 = OUTMOD_7;            // CCR1 reset/set
            break;

        case MD_CCW:
            P2OUT |= DIR_PIN;
            TA1CCTL1 = OUTMOD_7;            // CCR1 reset/set
            break;

        case MD_STOP:
            TA1CCTL1 &= ~(OUT);
            TA1CCTL1 = OUTMOD_0;
            break;
        case MD_IDLE:
        case MD_ERR:
        break;
    }
}

void md13s_stop(void)
{
    TA1CCTL1 &= ~(OUT);
    TA1CCTL1 = OUTMOD_0;
}

void md13s_setDuty(uint8_t pwm_duty)
{
    uint16_t duty = DEFAULT_Duty;
    if ((pwm_duty > MIN_Duty) && (pwm_duty < MAX_Duty)) {
        duty = pwm_duty;
    }
    TA1CCR1 = (DUTY_factor * duty) - 1;
}

size_t getMD13sFreq()
{
    return PWM_freq;
}
