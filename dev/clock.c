/** 
 * @file 	clock.c
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 05:11:58 pm
 * -----
 * Last Modified: 21 09 2022, 08:44:02 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Clock driver for mOS
 * 
 */
#include <stdint.h>
#include "clock.h"
#include "clockconfig.h"

static const uint16_t ACLK_FREQ = MOS_GET(MCLK_FREQ);
static const uint16_t MCLK_FREQ = MOS_GET(MCLK_FREQ);

void clock_init()
{
    DCOCTL = 0;
    /* Main clock */
    BCSCTL1 = CLK_BC1_REG;
    DCOCTL = CLK_DCO_REG;
    /* Aux clock */
    BCSCTL3 |= CLK_BC3_REG;
}

int getMCLK()
{
    return MOS_GET(MCLK_FREQ);
}

int getACLK()
{
    return MOS_GET(ACLK_FREQ);
}