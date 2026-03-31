/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Ultrasonic-based autonomous car demo with software PWM
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
  CAR_FORWARD = 0,
  CAR_AVOID_BRAKE,
  CAR_AVOID_REVERSE,
  CAR_AVOID_TURN
} CarState;

typedef struct
{
  int8_t left;
  int8_t right;
} MotorCommand;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define STOP_DISTANCE_CM 25U
#define REVERSE_DISTANCE_CM 15U

#define BRAKE_MS 120U
#define REVERSE_MS 320U
#define TURN_MS 360U
#define SENSOR_PERIOD_MS 80U

#define PWM_PERIOD_MS 20U
#define PWM_MAX_SPEED 100U

#define SPD_FORWARD 68
#define SPD_REVERSE 58
#define SPD_TURN_OUT 72
#define SPD_TURN_IN 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static volatile uint8_t g_manual_stop = 0U;
static uint8_t g_turn_right_next = 1U;
static CarState g_state = CAR_FORWARD;
static uint32_t g_state_enter_tick = 0U;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
static void DWT_Init(void);
static uint32_t micros(void);
static void delay_us(uint32_t us);
static void uart_log(const char *msg);
static void car_set_state(CarState next);
static uint8_t speed_abs_i8(int8_t value);
static void motor_apply_software_pwm(int8_t left_speed, int8_t right_speed);
static uint32_t ultrasonic_read_cm(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void DWT_Init(void)
{
  CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
  DWT->CYCCNT = 0;
  DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t micros(void)
{
  return DWT->CYCCNT / (SystemCoreClock / 1000000U);
}

static void delay_us(uint32_t us)
{
  uint32_t start = micros();
  while ((micros() - start) < us)
  {
  }
}

static void uart_log(const char *msg)
{
  HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg), 100);
}

static void car_set_state(CarState next)
{
  g_state = next;
  g_state_enter_tick = HAL_GetTick();

  switch (next)
  {
  case CAR_FORWARD:
    uart_log("STATE: FORWARD\r\n");
    break;
  case CAR_AVOID_BRAKE:
    uart_log("STATE: BRAKE\r\n");
    break;
  case CAR_AVOID_REVERSE:
    uart_log("STATE: REVERSE\r\n");
    break;
  case CAR_AVOID_TURN:
    uart_log("STATE: TURN\r\n");
    break;
  default:
    break;
  }
}

static uint8_t speed_abs_i8(int8_t value)
{
  if (value < 0)
  {
    return (uint8_t)(-value);
  }
  return (uint8_t)value;
}

static void motor_apply_software_pwm(int8_t left_speed, int8_t right_speed)
{
  uint32_t phase = HAL_GetTick() % PWM_PERIOD_MS;
  uint32_t left_on_ms;
  uint32_t right_on_ms;

  uint8_t left_mag = speed_abs_i8(left_speed);
  uint8_t right_mag = speed_abs_i8(right_speed);

  if (left_mag > PWM_MAX_SPEED)
  {
    left_mag = PWM_MAX_SPEED;
  }
  if (right_mag > PWM_MAX_SPEED)
  {
    right_mag = PWM_MAX_SPEED;
  }

  left_on_ms = ((uint32_t)left_mag * PWM_PERIOD_MS) / PWM_MAX_SPEED;
  right_on_ms = ((uint32_t)right_mag * PWM_PERIOD_MS) / PWM_MAX_SPEED;

  if ((left_speed > 0) && (phase < left_on_ms))
  {
    HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_RESET);
  }
  else if ((left_speed < 0) && (phase < left_on_ms))
  {
    HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(MOTOR_L_IN1_GPIO_Port, MOTOR_L_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_L_IN2_GPIO_Port, MOTOR_L_IN2_Pin, GPIO_PIN_RESET);
  }

  if ((right_speed > 0) && (phase < right_on_ms))
  {
    HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_RESET);
  }
  else if ((right_speed < 0) && (phase < right_on_ms))
  {
    HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_SET);
  }
  else
  {
    HAL_GPIO_WritePin(MOTOR_R_IN1_GPIO_Port, MOTOR_R_IN1_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MOTOR_R_IN2_GPIO_Port, MOTOR_R_IN2_Pin, GPIO_PIN_RESET);
  }
}

static uint32_t ultrasonic_read_cm(void)
{
  const uint32_t timeout_us = 30000U;
  uint32_t start_us;
  uint32_t pulse_start;
  uint32_t pulse_end;

  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);
  delay_us(2);
  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_SET);
  delay_us(10);
  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);

  start_us = micros();
  while (HAL_GPIO_ReadPin(US_ECHO_GPIO_Port, US_ECHO_Pin) == GPIO_PIN_RESET)
  {
    if ((micros() - start_us) > timeout_us)
    {
      return 0U;
    }
  }

  pulse_start = micros();
  while (HAL_GPIO_ReadPin(US_ECHO_GPIO_Port, US_ECHO_Pin) == GPIO_PIN_SET)
  {
    if ((micros() - pulse_start) > timeout_us)
    {
      return 0U;
    }
  }
  pulse_end = micros();

  return (pulse_end - pulse_start) / 58U;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  static uint32_t last_tick = 0U;
  uint32_t now = HAL_GetTick();

  if ((GPIO_Pin == SAFETY_BTN_Pin) && ((now - last_tick) > 200U))
  {
    last_tick = now;
    g_manual_stop ^= 1U;

    if (g_manual_stop)
    {
      uart_log("SAFETY STOP ON\r\n");
    }
    else
    {
      uart_log("SAFETY STOP OFF\r\n");
      car_set_state(CAR_FORWARD);
    }
  }
}
/* USER CODE END 0 */

int main(void)
{
  uint32_t distance_cm = 0U;
  uint32_t sensor_tick = 0U;
  uint32_t stop_led_tick = 0U;
  char log_buf[64];
  MotorCommand cmd = {0, 0};

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  DWT_Init();
  uart_log("AUTO_CAR_US_01 start\r\n");
  uart_log("Software PWM enabled\r\n");
  car_set_state(CAR_FORWARD);

  while (1)
  {
    if ((HAL_GetTick() - sensor_tick) >= SENSOR_PERIOD_MS)
    {
      sensor_tick = HAL_GetTick();
      distance_cm = ultrasonic_read_cm();

      if (distance_cm > 0U)
      {
        snprintf(log_buf, sizeof(log_buf), "dist=%lucm\r\n", distance_cm);
        uart_log(log_buf);
      }
    }

    if (g_manual_stop)
    {
      cmd.left = 0;
      cmd.right = 0;
      motor_apply_software_pwm(cmd.left, cmd.right);

      if ((HAL_GetTick() - stop_led_tick) >= 150U)
      {
        stop_led_tick = HAL_GetTick();
        HAL_GPIO_TogglePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin);
      }

      HAL_Delay(2);
      continue;
    }

    HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_SET);

    switch (g_state)
    {
    case CAR_FORWARD:
      cmd.left = SPD_FORWARD;
      cmd.right = SPD_FORWARD;
      if ((distance_cm > 0U) && (distance_cm <= STOP_DISTANCE_CM))
      {
        car_set_state(CAR_AVOID_BRAKE);
      }
      break;

    case CAR_AVOID_BRAKE:
      cmd.left = 0;
      cmd.right = 0;
      if ((HAL_GetTick() - g_state_enter_tick) >= BRAKE_MS)
      {
        car_set_state(CAR_AVOID_REVERSE);
      }
      break;

    case CAR_AVOID_REVERSE:
      cmd.left = -SPD_REVERSE;
      cmd.right = -SPD_REVERSE;
      if ((HAL_GetTick() - g_state_enter_tick) >= REVERSE_MS)
      {
        car_set_state(CAR_AVOID_TURN);
      }
      break;

    case CAR_AVOID_TURN:
      if ((distance_cm > 0U) && (distance_cm < REVERSE_DISTANCE_CM))
      {
        cmd.left = -SPD_REVERSE;
        cmd.right = -SPD_REVERSE;
      }
      else if (g_turn_right_next)
      {
        cmd.left = SPD_TURN_OUT;
        cmd.right = -SPD_TURN_IN;
      }
      else
      {
        cmd.left = -SPD_TURN_IN;
        cmd.right = SPD_TURN_OUT;
      }

      if ((HAL_GetTick() - g_state_enter_tick) >= TURN_MS)
      {
        g_turn_right_next ^= 1U;
        car_set_state(CAR_FORWARD);
      }
      break;

    default:
      car_set_state(CAR_FORWARD);
      cmd.left = 0;
      cmd.right = 0;
      break;
    }

    motor_apply_software_pwm(cmd.left, cmd.right);
    HAL_Delay(2);
  }
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

  HAL_GPIO_WritePin(GPIOB, MOTOR_L_IN1_Pin | MOTOR_L_IN2_Pin | MOTOR_R_IN1_Pin | MOTOR_R_IN2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(US_TRIG_GPIO_Port, US_TRIG_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(STATUS_LED_GPIO_Port, STATUS_LED_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = SAFETY_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SAFETY_BTN_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = MOTOR_L_IN1_Pin | MOTOR_L_IN2_Pin | MOTOR_R_IN1_Pin | MOTOR_R_IN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = US_TRIG_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(US_TRIG_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = US_ECHO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(US_ECHO_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = STATUS_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STATUS_LED_GPIO_Port, &GPIO_InitStruct);

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
