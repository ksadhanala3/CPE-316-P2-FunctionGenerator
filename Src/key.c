/*
 * key.c
 *
 *  Created on: Oct 25, 2024
 *      Author: ksadh, Guru Prasad
 */

#include "main.h"
#include "key.h"

const  uint8_t  keypadVals[ 4 ][ 3 ] = {
		{  1 ,  2 ,  3  },
		{  4 ,  5 ,  6  },
		{  7 ,  8 ,  9  },
		{  STAR ,  0 ,  HASH  }
};

void key_init(void) {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOCEN |
					RCC_AHB2ENR_GPIOBEN);
	GPIOC->MODER &= ~(GPIO_MODER_MODE0 |
					GPIO_MODER_MODE1 |
					GPIO_MODER_MODE2 |
					GPIO_MODER_MODE3);
	// Configuration of PD resistors for rows
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD0 |
					GPIO_PUPDR_PUPD1 |
					GPIO_PUPDR_PUPD2 |
					GPIO_PUPDR_PUPD3);
	GPIOC->PUPDR |= (GPIO_PUPDR_PUPD0_1 |  // row 1 -- PC0
					GPIO_PUPDR_PUPD1_1 |  // row 2 -- PC1
					GPIO_PUPDR_PUPD2_1 |  // row 3 -- PC2
					GPIO_PUPDR_PUPD3_1);  // row 4 -- PC3
	// Configuration of pins B5, B6, and B4 for columns
	GPIOB->MODER &= ~(GPIO_MODER_MODE5 |
					GPIO_MODER_MODE6 |
					GPIO_MODER_MODE4);
	GPIOB->MODER |= (GPIO_MODER_MODE5_0 |  // col 3 -- PB6
					GPIO_MODER_MODE6_0 |  // col 2 -- PB5
					GPIO_MODER_MODE4_0);  // col 1 -- PB4
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT5 |
					 GPIO_OTYPER_OT6 |
					 GPIO_OTYPER_OT4);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5 |
					  GPIO_OSPEEDR_OSPEED6 |
					  GPIO_OSPEEDR_OSPEED4);
}

int32_t  positionToValue(  uint8_t  col,  uint8_t  row)  {
	return  keypadVals[row][col];
}

int32_t  pollValue(  void  ){
	for  (  int32_t  currentColumn =  0 ; currentColumn + GPIO_ODR_OD4_Pos <=  GPIO_ODR_OD6_Pos ;  currentColumn++){
			// All columns first go LOW
			GPIOB->ODR &= ~((GPIO_ODR_OD4) | (GPIO_ODR_OD5) | (GPIO_ODR_OD6));
			// One column is set to high, iterating consistently
			GPIOB->ODR |= ( 1  << (GPIO_ODR_OD4_Pos + currentColumn));
			if  ((GPIOC->IDR & (GPIO_IDR_ID0))){
				return  positionToValue(currentColumn,  0 );
			}
			if  ((GPIOC->IDR & (GPIO_IDR_ID1))){
				return  positionToValue(currentColumn,  1 );
			}
			if  ((GPIOC->IDR & (GPIO_IDR_ID2))){
				return  positionToValue(currentColumn,  2 );
			}
			if  ((GPIOC->IDR & (GPIO_IDR_ID3))){
				return  positionToValue(currentColumn,  3 );
			}
	}
	return  NO_PRESS ;
}
