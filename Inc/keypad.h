/*
 * keypad.h
 *
 *  Created on: Oct 10, 2025
 *      Author: sean1
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

int val_macro(int row, int col);
int row_ret();
void keypad_output(int i);
int check_press(int current_column);

#endif /* INC_KEYPAD_H_ */
