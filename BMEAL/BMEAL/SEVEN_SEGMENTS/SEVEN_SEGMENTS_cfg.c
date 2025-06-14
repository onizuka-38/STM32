/*
 * File: SEVEN_SEGMENTS_cfg.c
 * Driver Name: [[ 7-Segments Display ]]
 * SW Layer:   BMEAL
 * Created on: Sep 22, 2023
 * Author:     JeongWhan Lee
 *
 */

#include "SEVEN_SEGMENTS.h"

const SEVEN_SEG_CfgType SEVEN_SEG_CfgParam[SEVEN_SEG_UNITS] =
{
	// 7-Segments Display Unit 1 Configurations
    {
    	/* Seven Segments Pins Info */
	    {GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB, GPIOB},
		{GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6},
		/* Enable Control Signal's Pins */
		{GPIOA, GPIOA, GPIOC, GPIOC},
		{GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_4, GPIO_PIN_3}
	}
};
