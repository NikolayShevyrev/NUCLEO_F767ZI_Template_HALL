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
bool insulation_measurement(void);
uint32_t measure_voltage(ADC_TypeDef * adc_base);


/* Defines -------------------------------------------------------------------*/
#define FPWM (uint16_t)20000
#define CONVERSIONS_COUNT 6

#define CURRENT_CONV_COEF 	(float)(0.001F / (float)CONVERSIONS_COUNT)
#define VOLTAGE_CONV_COEF 	(float)(0.01F / (float)CONVERSIONS_COUNT)
#define BEMF_CONV_COEF 		(float)(0.01F / (float)CONVERSIONS_COUNT)

// Leds
#define LD1_PIN 0
#define LD2_PIN 7
#define LD3_PIN 14
#define LD_PORT GPIOB

/**
 *  Relays
 */

#define RELAY_1_OUT_PIN 4
#define RELAY_2_OUT_PIN 5
#define RELAY_OUT_PORT  GPIOE

#define RELAY_POS 4
#define RELAY_NEG 5

#define RELAY_CLOSE(pin) SET_BIT(GPIOE->BSRR, (GPIO_BSRR_BS0  << pin));
#define RELAY_OPEN(pin) SET_BIT(GPIOE->BSRR, (GPIO_BSRR_BS0  << (pin+16)));

#define LEDX_PIN	2
#define LEDX_PORT	GPIOB

// Button
#define BUTTON_PIN 13
#define BUTTON_PORT GPIOC


// BEMF
#define VPOS_PIN	0
#define VNEG_PIN	1
#define V_MEAS_PORT	GPIOC

#define VPOS ADC1
#define VNEG ADC2

#define VREF 2048


// DAC Outputs
#define DAC1_PIN 4
#define DAC2_PIN 5
#define DAC_PORT GPIOA





#endif /* CORE_INC_MAIN_H_ */
