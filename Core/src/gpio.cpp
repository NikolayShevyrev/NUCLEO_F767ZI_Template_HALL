/*
 * gpio.cpp
 *
 *  Created on: 16 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */


#include "main.h"
#include "gpio.h"

/**
  * @brief  GPIO ports initialization
  * @retval None
  */
void GPIO_Init(void){

	/*
	 * GPIOB and GPOIC Ports Clock Enable
	 */
	GPIO_PortClockInit(GPIOB);
	GPIO_PortClockInit(GPIOC);
	GPIO_PortClockInit(HALL_OUT_PORT);

	/*
	 * User LEDs
	 *
	 * PB0 	= LD1 green
	 * PB7 	= LD2 blue
	 * PB14 = LD3 red
	 */
	//GPIO_OutputPinInit(LD1_PIN, LD_PORT, PushPull, High, NoPull);
	//GPIO_OutputPinInit(LD2_PIN, LD_PORT, PushPull, High, NoPull);
	//GPIO_OutputPinInit(LD3_PIN, LD_PORT, PushPull, High, NoPull);
	/*
	 * PB2 = LEDX Red
	 */
	GPIO_OutputPinInit(LEDX_PIN, LEDX_PORT, PushPull, High, NoPull);

	/*
	 * User Button = PC13
	 */
	GPIO_InputPinInit(BUTTON_PIN, BUTTON_PORT, NoPull);

	/*
	 * Hall Output
	 */
	GPIO_OutputPinInit(HALL1_OUT_PIN, HALL_OUT_PORT, PushPull, High, NoPull);
	GPIO_OutputPinInit(HALL2_OUT_PIN, HALL_OUT_PORT, PushPull, High, NoPull);
	GPIO_OutputPinInit(HALL3_OUT_PIN, HALL_OUT_PORT, PushPull, High, NoPull);
}
