/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dac.h"
#include "key.h"
#include <math.h>
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void tim2_init(void);
void wave_data(void);

int32_t status = SQR;
uint16_t sin_data[STEPS];
uint16_t tri_data[STEPS];
uint16_t saw_data[STEPS];
uint16_t index = 0;
uint8_t duty = 50;
uint16_t freq = 100;
uint32_t ccr = STEPS;

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  wave_data();
  key_init();
  dac_init();
  tim2_init();
  __enable_irq();

  //Alter calculation values and data values used
  while (1)
  {
	  int32_t press = pollValue();
	  switch(press)
	  {
	  case 1:
		  freq = 1;
		  break;
	  case 2:
		  freq = 2;
		  break;
	  case 3:
		  freq = 3;
		  break;
	  case 4:
		  freq = 4;
		  break;
	  case 5:
		  freq = 5;
		  break;
	  case SINE:
		  status = SINE;
		  break;
	  case TRI:
		  status = TRI;
		  break;
	  case SAW:
		  status = SAW;
		  break;
	  case SQR:
		  status = SQR;
		  break;
	  case 0:
		  duty = 50;
		  break;
	  case STAR:
		  if(duty > 10) {
			  duty -= 10;
		  } break;
	  case HASH:
		  if(duty < 90) {
			  duty += 10;
		  } break;
	  }
  }
}

//Handle constantly writing out values
void TIM2_IRQHandler(void) {
	TIM2->SR &= ~(TIM_SR_UIF | TIM_SR_CC1IF);
	//SPI1->DR = 0x3FFF;

	switch(status)
	{
	case SINE:
		dac_write(dac_volt_conv(sin_data[index]));
		index = (index+freq)%STEPS;
		break;
	case TRI:
		dac_write(dac_volt_conv(tri_data[index]));
		index = (index+freq)%STEPS;
		break;
	case SAW:
		dac_write(dac_volt_conv(saw_data[index]));
		index = (index+freq)%STEPS;
		break;
	case SQR:
		if(index<((duty*STEPS)/100)) {
			dac_write(dac_volt_conv(V_MAX));
		}
		else {
			dac_write(dac_volt_conv(0));
		}
		index += freq;

		if(index > STEPS) {
			index = 0;
		}
		break;
	}
	TIM2->CCR1 += STEPS;
}

//Initialize Timer 2
void tim2_init(void){
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	TIM2->CR1 |= TIM_CR1_ARPE;
	TIM2->CCR1 = STEPS;
	TIM2->DIER |= TIM_DIER_UIE;
	TIM2->DIER |= TIM_DIER_CC1IE;
	TIM2->ARR = 0xFFFFFFFF;
	NVIC->ISER[0] = (1 << (TIM2_IRQn & 0x1F));
	TIM2->CR1 |= TIM_CR1_CEN;
}

//Fill wave data array
void wave_data(void){
	for(int i = 0; i < STEPS; i++) {
		sin_data[i] = (uint16_t)((V_MAX/2)*sin(2*M_PI*i/STEPS)+(V_MAX/2));
	}
	for(int i = 0; i < STEPS/2; i++) {
		tri_data[i] = (uint16_t)(V_MAX/(STEPS/2)*i);
	}
	for(int i = STEPS/2; i < STEPS; i++) {
		tri_data[i] = (uint16_t)(V_MAX - (V_MAX/(STEPS/2))*(i-(STEPS/2)));
	}
	for(int i = 0; i < STEPS; i++) {
		saw_data[i] = (uint16_t)(V_MAX*i/STEPS);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
