/** 
 * @file 	mosdefs.h
 * @author 	Mohit Rathod
 * Created: 16 09 2022, 11:02:01 am
 * -----
 * Last Modified: 16 09 2022, 08:37:49 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Defines and macros for mOS 
 *          mini operating system for msp430g2xxx 
 */
#ifndef mos_defs_h
#define mos_defs_h

/* Macros to check #define setting name in header or src files */
#define MOS_USES(x)             MOS_CONFIG_ ## x
#define MOS_GET(y)              MOS_ ## y

/* Macro to get the size of an array */
#define ARRAY_SIZE(array)       (sizeof(array)/sizeof(array[0]))

/* Macro to ignore unused parameters */
#define IGNORE(p)               ((void) p)

/* Macro to assert an expression at compile time. */
#define static_assert           _Static_assert

/* Macro to swap to numbers */
#define SWAP(a, b)              do { a ^= b; b ^= a; a ^= b; } while (0)

#endif /* mos_defs_h */