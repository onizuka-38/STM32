/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t data[10] = {
    0xFC, // 0
    0x60, // 1
    0xDA, // 2
    0xF2, // 3
    0x66, // 4
    0xB6, // 5
    0xBE, // 6
    0xE4, // 7
    0xFE, // 8
    0xF6  // 9
};

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void Seg_Number( uint16_t val, uint8_t dp){



	uint16_t _bits;



	_bits = FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin|FND4_Pin|FND5_Pin;  // "0"

	// Clear All segment port bits...

	HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin

				  |FND4_Pin|FND5_Pin|FND6_Pin|FND7_Pin, GPIO_PIN_RESET);




	switch(val%10){

		case 1:

			_bits = FND1_Pin|FND2_Pin;

			break;

		case 2:

			_bits = FND0_Pin|FND1_Pin|FND3_Pin|FND4_Pin|FND6_Pin;

			break;

		case 3:

			_bits = FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin|FND6_Pin;

			break;

		case 4:

			_bits = FND1_Pin|FND2_Pin|FND5_Pin|FND6_Pin;

			break;

		case 5:

			_bits = FND0_Pin|FND2_Pin|FND3_Pin|FND5_Pin|FND6_Pin;

			break;

		case 6:

			_bits = FND2_Pin|FND3_Pin|FND4_Pin|FND5_Pin|FND6_Pin;

			break;

		case 7:

			_bits = FND0_Pin|FND1_Pin|FND2_Pin;

			break;

		case 8:

			_bits = FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin|FND4_Pin|FND5_Pin|FND6_Pin;

			break;

		case 9:

			_bits = FND0_Pin|FND1_Pin|FND2_Pin|FND5_Pin|FND6_Pin;

			break;

	}

	if(dp)

		_bits |= FND7_Pin;

	HAL_GPIO_WritePin(GPIOB, _bits, GPIO_PIN_SET);





}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  int _digit = 0;
  int data[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int _digits[4] = {0};

  while (1)
  {


	  //	  Seg_Reset();

	  //	  HAL_GPIO_WritePin(GPIOA, FND_SEL0_Pin, GPIO_PIN_SET);

	  //	  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin|FND4_Pin|FND5_Pin,GPIO_PIN_SET);  // 0





	  	  HAL_GPIO_WritePin(GPIOA, FND_SEL0_Pin, GPIO_PIN_SET);

	  //	  Seg_Number(8);

	  	  int _digit;



	  	  if( _digit == 0)

	  		  Seg_Number(data[0],1);

	  	  else

	  		  Seg_Number(data[0],0);

	  	  HAL_Delay(500);

	  	  HAL_GPIO_WritePin(GPIOA, FND_SEL0_Pin, GPIO_PIN_RESET);



	  //	  Seg_Reset();

	  	  HAL_GPIO_WritePin(GPIOA, FND_SEL1_Pin, GPIO_PIN_SET);

	  //	  HAL_GPIO_WritePin(GPIOB, FND1_Pin|FND2_Pin,GPIO_PIN_SET);  // 1

	  //	  Seg_Number(9);

	  	  if( _digit == 1)

	  		  Seg_Number(data[1],1);

	  	  else

	  	  	  Seg_Number(data[1],0);

	  	  HAL_Delay(500);

	  	  HAL_GPIO_WritePin(GPIOA, FND_SEL1_Pin, GPIO_PIN_RESET);



	  //	  Seg_Reset();

	  	  HAL_GPIO_WritePin(GPIOC, FND_SEL2_Pin, GPIO_PIN_SET);

	  //	  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND3_Pin|FND4_Pin|FND6_Pin,GPIO_PIN_SET);  // 2

	  //	  Seg_Number(0);

	  	  if( _digit == 2)

	  		  Seg_Number(data[2],1);

	  	  else

	  	  	  Seg_Number(data[2],0);

	  	  HAL_Delay(500);

	  	  HAL_GPIO_WritePin(GPIOC, FND_SEL2_Pin, GPIO_PIN_RESET);



	  //	  Seg_Reset();

	  	  HAL_GPIO_WritePin(GPIOC, FND_SEL3_Pin, GPIO_PIN_SET);

	  //	  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin|FND6_Pin,GPIO_PIN_SET);  // 3

	  //	  Seg_Number(7);

	  	  if( _digit == 3)

	  		  Seg_Number(data[3],1);

	  	  else

	  	  	  Seg_Number(data[3],0);

	    	  HAL_Delay(500);

	    	  HAL_GPIO_WritePin(GPIOC, FND_SEL3_Pin, GPIO_PIN_RESET);

	    	  _digit++;
	    	          if (_digit >= 4) {
	    	              _digit = 0;
	    	          }



	  //  	  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin

	  //  			  |FND4_Pin|FND5_Pin|FND6_Pin|FND7_Pin, GPIO_PIN_RESET);



	  //  	  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|FND3_Pin

	  //  			  |FND4_Pin|FND5_Pin|FND6_Pin|FND7_Pin, GPIO_PIN_RESET);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED4_Pin|LED5_Pin|LED6_Pin|FND_SEL3_Pin
                          |FND_SEL2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|FND_SEL0_Pin|FND_SEL1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, FND0_Pin|FND1_Pin|FND2_Pin|LED0_Pin
                          |LED1_Pin|LED2_Pin|LED3_Pin|FND3_Pin
                          |FND4_Pin|FND5_Pin|FND6_Pin|FND7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SW1_GPIO_Port, SW1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED4_Pin LED5_Pin LED6_Pin FND_SEL3_Pin
                           FND_SEL2_Pin */
  GPIO_InitStruct.Pin = LED4_Pin|LED5_Pin|LED6_Pin|FND_SEL3_Pin
                          |FND_SEL2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin FND_SEL0_Pin FND_SEL1_Pin */
  GPIO_InitStruct.Pin = LD2_Pin|FND_SEL0_Pin|FND_SEL1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : FND0_Pin FND1_Pin FND2_Pin LED0_Pin
                           LED1_Pin LED2_Pin LED3_Pin FND3_Pin
                           FND4_Pin FND5_Pin FND6_Pin FND7_Pin */
  GPIO_InitStruct.Pin = FND0_Pin|FND1_Pin|FND2_Pin|LED0_Pin
                          |LED1_Pin|LED2_Pin|LED3_Pin|FND3_Pin
                          |FND4_Pin|FND5_Pin|FND6_Pin|FND7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SW1_Pin */
  GPIO_InitStruct.Pin = SW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
