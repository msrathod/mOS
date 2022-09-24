/** 
 * @file 	mos.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 05:06:20 pm
 * -----
 * Last Modified: 24 09 2022, 08:00:55 am
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   mOS - The mini Operating System includes.
 *          Provides all the modules configured in the 
 *          mosconfig file. 
 */
#ifndef mos_include_h
#define mos_include_h
#include <mosconfig.h>
#include <mosprint.h>
#include <mossch.h>

void setup(void);
void loop(void);

#endif /* mos_include_h */