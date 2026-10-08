/*
 * dac.c
 *
 *  Created on: Oct 27, 2024
 *      Author: ksadh, Elizabeth Acevedo
 */
#include "main.h"
#include "dac.h"

void dac_init(void) {
	  //Start clock for GPIOA and SPI1
	  RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
	  RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	  //Set up GPIOA 4, 5 and 7 for NSS, SCK and PICO in SPI AF
	  GPIOA->MODER &= ~(GPIO_MODER_MODE4 |
			  	  	  	GPIO_MODER_MODE5 |
			  	  	  	GPIO_MODER_MODE7);
	  GPIOA->MODER |= (GPIO_MODER_MODE4_1 |
			  	  	   GPIO_MODER_MODE5_1 |
			  	  	   GPIO_MODER_MODE7_1);
	  GPIOA->OTYPER &= ~(GPIO_OTYPER_OT4 |
			  	  	  	 GPIO_OTYPER_OT5 |
			  	  	  	 GPIO_OTYPER_OT7);
	  GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD4 |
			  	  	  	GPIO_PUPDR_PUPD5 |
						GPIO_PUPDR_PUPD7);
	  GPIOA->OSPEEDR |= (GPIO_OSPEEDR_OSPEED4 |
			  	  	  	 GPIO_OSPEEDR_OSPEED5 |
			   	   	   	 GPIO_OSPEEDR_OSPEED7);
	  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL4 |
			  	  	  	 GPIO_AFRL_AFSEL5 |
						 GPIO_AFRL_AFSEL7);
	  //AF 0x5 << pos
	  GPIOA->AFR[0] |= (GPIO_AFRL_AFSEL4_0 | GPIO_AFRL_AFSEL4_2 |
			  	  	  	GPIO_AFRL_AFSEL5_0 | GPIO_AFRL_AFSEL5_2 |
			  	  	  	GPIO_AFRL_AFSEL7_0 | GPIO_AFRL_AFSEL7_2);

	  SPI1->CR1 &= ~(SPI_CR1_RXONLY); //RX and TX
	  SPI1->CR1 &= ~SPI_CR1_CPHA; // 0,0
	  SPI1->CR1 &= ~SPI_CR1_CPOL;
	  SPI1->CR1 |= SPI_CR1_BIDIOE;
	  SPI1->CR1 |= SPI_CR1_MSTR; //Board as Controller
	  SPI1->CR1 &= ~(SPI_CR1_BR); //1/2 sck
	  SPI1->CR1 &= ~(SPI_CR1_LSBFIRST); //MSB first
	  SPI1->CR1 &= ~(SPI_CR1_SSM); //Using Hardware NSS, low when enable high

	  SPI1->CR2 &= ~(SPI_CR2_NSSP | SPI_CR2_FRXTH);
	  SPI1->CR2 |= (SPI_CR2_SSOE | SPI_CR2_DS |
			  	  	SPI_CR2_NSSP);

	  //Enable SPI after all other config
	  SPI1->CR1 |= SPI_CR1_SPE;
	  SPI1->DR = 0x3FFF;
}

void dac_write(uint16_t data) {
	while(!(SPI1->SR & SPI_SR_TXE)){}
	data |= DAC_CTRL; //Add req'd ctrl bits to val
	SPI1->DR = data;
	while((SPI1->SR & SPI_SR_BSY)){};
}

uint16_t dac_volt_conv(uint16_t value) {
	if(value > V_REF) {
		value = V_REF;
	}
	return (value/V_REF)*STEPS;
}
