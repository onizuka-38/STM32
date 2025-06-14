/*
 * File: LCD16x2_cfg.c
 * Driver Name: [[ LCD16x2 Display (GPIO 4-Bit Mode) ]]
 * SW Layer:   BMEAL
 * Created on: Sep 22, 2023
 * Author:     JeongWhan Lee
 *
 */

#include "LCD16x2.h"

const LCD16x2_CfgType LCD16x2_CfgParam =
{
	GPIOC,			// LCD_GPIO
	GPIO_PIN_5,		// D4_PIN
	GPIO_PIN_6,		// D5_PIN
	GPIO_PIN_7,		// D6_PIN
	GPIO_PIN_8,		// D7_PIN
	GPIO_PIN_9,		// RS_PIN;

	GPIOA,			// LCD_GPIO_CTRL
	GPIO_PIN_12, 	// EN_PIN;


	20
};
