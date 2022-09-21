/** 
 * @file 	clock.c
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 05:11:58 pm
 * -----
 * Last Modified: 21 09 2022, 08:56:03 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Clock driver for mOS
 * 
 */
#include <stdint.h>
#include <stddef.h>
#include "clock.h"
#include "clockconfig.h"

static const size_t ACLK_FREQ = MOS_GET(ACLK_FREQ);
static const size_t MCLK_FREQ = MOS_GET(MCLK_FREQ);

void clock_init()
{
    DCOCTL = 0;
    /* Main clock */
    BCSCTL1 = CLK_BC1_REG;
    DCOCTL = CLK_DCO_REG;
    /* Aux clock */
    BCSCTL3 |= CLK_BC3_REG;
}

size_t getMCLK()
{
    return MCLK_FREQ;
}

size_t getACLK()
{
    return ACLK_FREQ;
}