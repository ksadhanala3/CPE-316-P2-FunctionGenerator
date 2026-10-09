#include "DAC.h"
#include "main.h"

	void DAC_init(void) {
		//PA4 - NSS/CS, PA5 - SCK, PA7 - COPI
			RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN);
			GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL4 | GPIO_AFRL_AFSEL5 |
					GPIO_AFRL_AFSEL7);
			GPIOA->AFR[0] |= ((5<<GPIO_AFRL_AFSEL4_Pos) |
					(5<<GPIO_AFRL_AFSEL4_Pos) |
					(5<<GPIO_AFRL_AFSEL5_Pos) |
					(5<<GPIO_AFRL_AFSEL7_Pos));
			GPIOA->MODER &= ~(GPIO_MODER_MODE4 | GPIO_MODER_MODE5 |
					GPIO_MODER_MODE7);
			GPIOA->MODER |= (GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1 |
					GPIO_MODER_MODE7_1);
			//SPI Configure
			RCC->APB2ENR |= (RCC_APB2ENR_SPI1EN);
			SPI1->CR1 = (SPI_CR1_MSTR);
			SPI1->CR2 = (SPI_CR2_SSOE | SPI_CR2_NSSP |
					(0xF << SPI_CR2_DS_Pos));
			SPI1->CR1 |= (SPI_CR1_SPE);
	}
	void DAC_write(uint16_t to_write) {
		while(!(SPI1->SR & SPI_SR_TXE));
		to_write &= ~(0xF << 12);
		to_write |= (0x3 << 12);
		SPI1->DR = to_write;
	//	while(~(SPI1->SR) & SPI_SR_TXE);
	//	while(SPI1->SR & SPI_SR_BSY);
	}
	uint16_t DAC_volt_conv(uint16_t mv) {
		//4096 possible values, each step = Vmax / 4096, Vmax is 3.3V for us
		if(mv > 3300) {
			return 3300*4096/3300;
		}
		return mv*4096/3300;
	}



