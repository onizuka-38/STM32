/*
 * File: DWT_Delay.h
 * Driver Name: [[ DWT Delay ]]
 * SW Layer:   BMEAL
 * Created on: Sep 22, 2023
 * Author:     JeongWhan Lee
 *
 */

#ifndef DWT_DELAY_H_
#define DWT_DELAY_H_

#include "main.h"
//#include "stm32f4xx_hal.h"

uint32_t DWT_Delay_Init(void);


// This Function Provides Delay In Microseconds Using DWT
__STATIC_INLINE void DWT_Delay_us(volatile uint32_t au32_microseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000000);
  au32_microseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_microseconds-au32_ticks);
}

// This Function Provides Delay In Milliseconds Using DWT
__STATIC_INLINE void DWT_Delay_ms(volatile uint32_t au32_milliseconds)
{
  uint32_t au32_initial_ticks = DWT->CYCCNT;
  uint32_t au32_ticks = (HAL_RCC_GetHCLKFreq() / 1000);
  au32_milliseconds *= au32_ticks;
  while ((DWT->CYCCNT - au32_initial_ticks) < au32_milliseconds);
}

#endif /* DWT_DELAY_H_ */
