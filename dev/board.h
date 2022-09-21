/** 
 * @file 	board.h
 * @author 	Mohit Rathod
 * Created: 21 09 2022, 10:40:27 pm
 * -----
 * Last Modified: 21 09 2022, 10:44:44 pm
 * Modified By  : Mohit Rathod
 * -----
 * MIT License
 * https://opensource.org/licenses/MIT
 * 
 * @brief   Board Initialization API
 *          Hardware configuration and intialization for all 
 *          the peripherals setup in mosconfig.
 */
#ifndef dev_board_h
#define dev_board_h

/**
 * @fn      int board_init(void);
 * @brief   Initialize all board dependent functionality.
 * @return  0 on success, -1 otherwise 
 */
int board_init(void);

#endif /* dev_board_h */

