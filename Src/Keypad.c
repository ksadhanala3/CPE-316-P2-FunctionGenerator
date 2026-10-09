/*
 * Keypad.c
 *
 *  Created on: Oct 10, 2025
 *      Author: sean1
 */
#include "main.h"
#include <stdio.h>
#include <keypad.h>



	const int pad_vals[4][3] = { {1, 2, 3},
  							{4, 5, 6},
  							{7, 8, 9},
  							{10,0,11} };

  int val_macro(int row, int col) {
    	return pad_vals[row][col];
    }

  int row_ret(void) {
   if (GPIOA->IDR & GPIO_IDR_ID8) { //PA8
  	return 0;
  }
   if (GPIOC->IDR & GPIO_IDR_ID0) { //PC0
  	return 1;
  }
   if (GPIOC->IDR & GPIO_IDR_ID1) { //PC1
  	return 2;
  }
   if (GPIOB->IDR & GPIO_IDR_ID0) { //PB0
  	return 3;
  }
   return -1;
  }

  void keypad_output(int i){
	  GPIOC->BRR = GPIO_PIN_9;
	  GPIOC->BRR = GPIO_PIN_10;
	  GPIOC->BRR = GPIO_PIN_11;
	  GPIOC->BRR = GPIO_PIN_12;
	  GPIOC->ODR |= (i << 9);
  }


  int check_press(int current_column){
  	  int row = 0;
  	  GPIOA->ODR &= ~((GPIO_ODR_OD4) | GPIO_ODR_OD1 | GPIO_ODR_OD0);
  	  HAL_Delay(10);
  	  if (current_column == 0){
  		  GPIOA->ODR |= (0x1 << 4);
  		  row = row_ret();
  		  if (row >= 0){
  			  return (val_macro(row, current_column));
  		  } else {
  			  current_column++;
  			  return check_press(current_column);
  		  }
  	  }
  		if (current_column == 1){
  		  		  GPIOA->ODR |= (0x1 << 1);
  		  		  row = row_ret();
  		  		  if (row >= 0){
  		  			  return (val_macro(row, current_column));
  		  		  } else {
  		  			  current_column++;
  		  			  return check_press(current_column);
  		  		  }
  		 }
  		if (current_column == 2){
  		  		  GPIOA->ODR |= (0x1);
  		  		  row = row_ret();
  		  		  if (row >= 0){
  		  			  return (val_macro(row, current_column));
  		  		  } else {
  		  			  current_column++;
  		  			  return check_press(current_column);
  		  		  }
  				}
  	  return -1;
  	  }
