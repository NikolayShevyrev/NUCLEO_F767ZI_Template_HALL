/*
 * main.h
 *
 *  Created on: Oct 6, 2020
 *      Author: Nikolay Shevyrev
 */

#ifndef CORE_INC_MAIN_H_
#define CORE_INC_MAIN_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f767xx.h"
#include "stm32f7xx.h"

/* Function prototypes -------------------------------------------------------*/
void SystemClock_Config(void);

/* Defines -------------------------------------------------------------------*/
#define LD1_PIN 0
#define LD2_PIN 7
#define LD3_PIN 14
#define LD_PORT GPIOB

#define BUTTON_PIN 13
#define BUTTON_PORT GPIOC

#define DAC1_PIN 4
#define DAC2_PIN 5
#define DAC_PORT GPIOA

#define HALL1_OUT_PIN 4
#define HALL2_OUT_PIN 5
#define HALL3_OUT_PIN 6
#define HALL_OUT_PORT GPIOE

#define HALL1_IN_PIN 15
#define HALL2_IN_PIN 3
#define HALL3_IN_PIN 10
#define HALL1_IN_PORT GPIOA
#define HALL23_IN_PORT GPIOB

#define UART4_RX_PIN 0
#define UART4_TX_PIN 1
#define UART4_PORT GPIOD

#define FPWM (uint16_t)20000


#endif /* CORE_INC_MAIN_H_ */
