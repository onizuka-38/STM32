/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Physical AI navigation demo (sensor fusion + adaptive policy)
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
  NAV_DRIVE = 0,
  NAV_BRAKE,
  NAV_REVERSE,
  NAV_TURN,
  NAV_EDGE_RECOVER
} NavState;

typedef struct
{
  int8_t left;
  int8_t right;
} MotorCommand;

typedef struct
{
  uint32_t raw_cm;
  uint32_t filtered_cm;
  uint8_t ir_left;
  uint8_t ir_right;
  uint8_t obstacle_score;
} WorldModel;

typedef struct
{
  int8_t turn_bias;
  int8_t last_turn_dir;
  uint32_t avoid_count;
  uint32_t success_count;
} AgentMemory;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SENSOR_PERIOD_MS 80U
#define PWM_PERIOD_MS 20U
#define PWM_MAX_SPEED 100U

#define STOP_DISTANCE_CM 22U
#define SLOW_DISTANCE_CM 40U

#define BRAKE_MS 110U
#define REVERSE_MS 280U
#define TURN_MS 340U
#define EDGE_RECOVER_MS 460U
#define EDGE_REVERSE_MS 180U

#define SPD_FAST 72
#define SPD_SLOW 50
#define SPD_REVERSE 58
#define SPD_TURN_OUT 74
#define SPD_TURN_IN 28

#define IR_ACTIVE_LEVEL GPIO_PIN_RESET
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static volatile uint8_t g_manual_stop = 0U;
static NavState g_state = NAV_DRIVE;
static uint32_t g_state_enter_tick = 0U;
static WorldModel g_world = {0};
static AgentMemory g_memory = {0};
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
static int32_t clamp_i32(int32_t value, int32_t lo, int32_t hi);
static uint8_t abs_i8(int8_t value);
static void motor_apply_software_pwm(int8_t left_speed, int8_t right_speed);
static uint32_t ultrasonic_read_cm(void);
static void world_update(WorldModel *world);
static void nav_set_state(NavState next);
static int8_t planner_choose_turn_dir(const WorldModel *world, const AgentMemory *memory);
static void planner_step(WorldModel *world, AgentMemory *memory, MotorCommand *cmd);
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

static int32_t clamp_i32(int32_t value, int32_t lo, int32_t hi)
{
  if (value < lo)
  {
    return lo;
  }
  if (value > hi)
  {
    return hi;
  }
  return value;
}

static uint8_t abs_i8(int8_t value)
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
  uint8_t left_mag = abs_i8(left_speed);
  uint8_t right_mag = abs_i8(right_speed);
  uint32_t left_on_ms;
  uint32_t right_on_ms;

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

static void world_update(WorldModel *world)
{
  uint32_t cm = ultrasonic_read_cm();
  uint8_t ir_left = (HAL_GPIO_ReadPin(IR_LEFT_GPIO_Port, IR_LEFT_Pin) == IR_ACTIVE_LEVEL) ? 1U : 0U;
  uint8_t ir_right = (HAL_GPIO_ReadPin(IR_RIGHT_GPIO_Port, IR_RIGHT_Pin) == IR_ACTIVE_LEVEL) ? 1U : 0U;
  int32_t score = world->obstacle_score;

  world->raw_cm = cm;
  world->ir_left = ir_left;
  world->ir_right = ir_right;

  if (cm > 0U)
  {
    if (world->filtered_cm == 0U)
    {
      world->filtered_cm = cm;
    }
    else
    {
      world->filtered_cm = (world->filtered_cm * 7U + cm * 3U) / 10U;
    }
  }

  if ((world->filtered_cm > 0U) && (world->filtered_cm <= STOP_DISTANCE_CM))
  {
    score += 14;
  }
  else if ((world->filtered_cm > 0U) && (world->filtered_cm <= SLOW_DISTANCE_CM))
  {
    score += 6;
  }
  else
  {
    score -= 4;
  }

  if (ir_left || ir_right)
  {
    score += 25;
  }

  world->obstacle_score = (uint8_t)clamp_i32(score, 0, 100);
}

static void nav_set_state(NavState next)
{
  g_state = next;
  g_state_enter_tick = HAL_GetTick();
}

static int8_t planner_choose_turn_dir(const WorldModel *world, const AgentMemory *memory)
{
  if (world->ir_left && !world->ir_right)
  {
    return +1;
  }
  if (world->ir_right && !world->ir_left)
  {
    return -1;
  }
  if (memory->turn_bias >= 0)
  {
    return +1;
  }
  return -1;
}

static void planner_step(WorldModel *world, AgentMemory *memory, MotorCommand *cmd)
{
  uint32_t elapsed = HAL_GetTick() - g_state_enter_tick;
  int32_t base_speed = SPD_FAST;

  cmd->left = 0;
  cmd->right = 0;

  switch (g_state)
  {
  case NAV_DRIVE:
    if ((world->filtered_cm > 0U) && (world->filtered_cm <= SLOW_DISTANCE_CM))
    {
      base_speed = SPD_SLOW;
    }

    cmd->left = (int8_t)base_speed;
    cmd->right = (int8_t)base_speed;

    if (world->obstacle_score >= 60U)
    {
      nav_set_state(NAV_BRAKE);
    }
    else if (world->ir_left || world->ir_right)
    {
      nav_set_state(NAV_EDGE_RECOVER);
    }
    break;

  case NAV_BRAKE:
    cmd->left = 0;
    cmd->right = 0;
    if (elapsed >= BRAKE_MS)
    {
      nav_set_state(NAV_REVERSE);
    }
    break;

  case NAV_REVERSE:
    cmd->left = -SPD_REVERSE;
    cmd->right = -SPD_REVERSE;
    if (elapsed >= REVERSE_MS)
    {
      nav_set_state(NAV_TURN);
      memory->avoid_count++;
    }
    break;

  case NAV_TURN:
  {
    int8_t dir = planner_choose_turn_dir(world, memory);

    if (dir > 0)
    {
      cmd->left = SPD_TURN_OUT;
      cmd->right = -SPD_TURN_IN;
    }
    else
    {
      cmd->left = -SPD_TURN_IN;
      cmd->right = SPD_TURN_OUT;
    }

    memory->last_turn_dir = dir;

    if (elapsed >= TURN_MS)
    {
      if (world->obstacle_score < 40U)
      {
        memory->success_count++;
        memory->turn_bias = (int8_t)clamp_i32(memory->turn_bias + dir, -20, 20);
      }
      else
      {
        memory->turn_bias = (int8_t)clamp_i32(memory->turn_bias - (2 * dir), -20, 20);
      }
      nav_set_state(NAV_DRIVE);
    }
    break;
  }

  case NAV_EDGE_RECOVER:
  {
    int8_t dir = planner_choose_turn_dir(world, memory);

    if (elapsed < EDGE_REVERSE_MS)
    {
      cmd->left = -SPD_REVERSE;
      cmd->right = -SPD_REVERSE;
    }
    else
    {
      if (dir > 0)
      {
        cmd->left = SPD_TURN_OUT;
        cmd->right = -SPD_TURN_IN;
      }
      else
      {
        cmd->left = -SPD_TURN_IN;
        cmd->right = SPD_TURN_OUT;
      }
    }

    if (elapsed >= EDGE_RECOVER_MS)
    {
      nav_set_state(NAV_DRIVE);
    }
    break;
  }

  default:
    nav_set_state(NAV_DRIVE);
    break;
  }
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
      nav_set_state(NAV_DRIVE);
    }
  }
}
/* USER CODE END 0 */

int main(void)
{
  uint32_t sensor_tick = 0U;
  uint32_t stop_led_tick = 0U;
  uint32_t log_tick = 0U;
  char log_buf[96];
  MotorCommand cmd = {0, 0};

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  DWT_Init();
  uart_log("PHYSICAL_AI_NAV_01 start\r\n");

  nav_set_state(NAV_DRIVE);

  while (1)
  {
    if ((HAL_GetTick() - sensor_tick) >= SENSOR_PERIOD_MS)
    {
      sensor_tick = HAL_GetTick();
      world_update(&g_world);
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

    planner_step(&g_world, &g_memory, &cmd);
    motor_apply_software_pwm(cmd.left, cmd.right);

    if ((HAL_GetTick() - log_tick) >= 300U)
    {
      log_tick = HAL_GetTick();
      snprintf(log_buf,
               sizeof(log_buf),
               "cm=%lu f=%lu irL=%u irR=%u risk=%u bias=%d succ=%lu/%lu\r\n",
               g_world.raw_cm,
               g_world.filtered_cm,
               g_world.ir_left,
               g_world.ir_right,
               g_world.obstacle_score,
               g_memory.turn_bias,
               g_memory.success_count,
               g_memory.avoid_count);
      uart_log(log_buf);
    }

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

  GPIO_InitStruct.Pin = US_ECHO_Pin | IR_LEFT_Pin | IR_RIGHT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

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
