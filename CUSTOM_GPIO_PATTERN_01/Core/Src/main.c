/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  PATTERN_SHIFT = 0,
  PATTERN_BOUNCE,
  PATTERN_ALTERNATE,
  PATTERN_MAX
} LedPatternMode;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED_BIT_0 0x01U
#define LED_BIT_1 0x02U
#define LED_BIT_2 0x04U
#define LED_BIT_3 0x08U
#define LED_BIT_4 0x10U
#define LED_BIT_5 0x20U
#define LED_BIT_6 0x40U
#define LED_BIT_7 0x80U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static volatile LedPatternMode g_mode = PATTERN_SHIFT;
static volatile uint32_t g_last_button_tick = 0U;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static void LED_Write8(uint8_t value);
static void App_RunPattern(LedPatternMode mode);
static void UART_Print(const char *msg);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LED_Write8(uint8_t value)
{
  HAL_GPIO_WritePin(GPIOB, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, LED0_Pin, (value & LED_BIT_0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LED1_Pin, (value & LED_BIT_1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LED2_Pin, (value & LED_BIT_2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LED3_Pin, (value & LED_BIT_3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED4_Pin, (value & LED_BIT_4) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED5_Pin, (value & LED_BIT_5) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED6_Pin, (value & LED_BIT_6) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOC, LED7_Pin, (value & LED_BIT_7) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void UART_Print(const char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg), 100);
}

static void App_RunPattern(LedPatternMode mode)
{
  static uint8_t shift = LED_BIT_0;
  static uint8_t bounce = LED_BIT_0;
  static int8_t bounce_dir = 1;
  static uint8_t alt = 0x55U;

  switch (mode)
  {
  case PATTERN_SHIFT:
    LED_Write8(shift);
    shift = (shift == LED_BIT_7) ? LED_BIT_0 : (uint8_t)(shift << 1);
    break;

  case PATTERN_BOUNCE:
    LED_Write8(bounce);
    if (bounce == LED_BIT_7)
    {
      bounce_dir = -1;
    }
    else if (bounce == LED_BIT_0)
    {
      bounce_dir = 1;
    }
    bounce = (bounce_dir > 0) ? (uint8_t)(bounce << 1) : (uint8_t)(bounce >> 1);
    break;

  case PATTERN_ALTERNATE:
    LED_Write8(alt);
    alt ^= 0xFFU;
    break;

  default:
    LED_Write8(0x00U);
    break;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == B1_Pin)
  {
    uint32_t now = HAL_GetTick();

    if ((now - g_last_button_tick) >= 150U)
    {
      g_last_button_tick = now;
      g_mode = (LedPatternMode)((g_mode + 1U) % PATTERN_MAX);

      switch (g_mode)
      {
      case PATTERN_SHIFT:
        UART_Print("Mode: SHIFT\r\n");
        break;
      case PATTERN_BOUNCE:
        UART_Print("Mode: BOUNCE\r\n");
        break;
      case PATTERN_ALTERNATE:
        UART_Print("Mode: ALTERNATE\r\n");
        break;
      default:
        break;
      }
    }
  }
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

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */
  UART_Print("CUSTOM_GPIO_PATTERN_01 start\r\n");
  UART_Print("Press USER button to change mode\r\n");
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE BEGIN WHILE */
    App_RunPattern(g_mode);
    HAL_Delay(150);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

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

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_USART2_UART_Init(void)
{
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
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED0_Pin | LED1_Pin | LED2_Pin | LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  (void)file;
  (void)line;
}
#endif
