/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_0
#define LED4_GPIO_Port GPIOC
#define LED5_Pin GPIO_PIN_1
#define LED5_GPIO_Port GPIOC
#define LED6_Pin GPIO_PIN_2
#define LED6_GPIO_Port GPIOC
#define FND_SEL3_Pin GPIO_PIN_3
#define FND_SEL3_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define FND_SEL2_Pin GPIO_PIN_4
#define FND_SEL2_GPIO_Port GPIOC
#define FND0_Pin GPIO_PIN_0
#define FND0_GPIO_Port GPIOB
#define FND1_Pin GPIO_PIN_1
#define FND1_GPIO_Port GPIOB
#define FND2_Pin GPIO_PIN_2
#define FND2_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_12
#define LED0_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_13
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_14
#define LED2_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOB
#define FND_SEL0_Pin GPIO_PIN_10
#define FND_SEL0_GPIO_Port GPIOA
#define FND_SEL1_Pin GPIO_PIN_11
#define FND_SEL1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SW1_Pin GPIO_PIN_2
#define SW1_GPIO_Port GPIOD
#define FND3_Pin GPIO_PIN_3
#define FND3_GPIO_Port GPIOB
#define FND4_Pin GPIO_PIN_4
#define FND4_GPIO_Port GPIOB
#define FND5_Pin GPIO_PIN_5
#define FND5_GPIO_Port GPIOB
#define FND6_Pin GPIO_PIN_6
#define FND6_GPIO_Port GPIOB
#define FND7_Pin GPIO_PIN_7
#define FND7_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
