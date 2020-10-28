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

	/*
	 * User LEDs
	 *
	 * PB0 	= LD1 green
	 * PB7 	= LD2 blue
	 * PB14 = LD3 red
	 */
	GPIO_OutputPinInit(LD1_PIN, LD_PORT, PushPull, High, NoPull);
	GPIO_OutputPinInit(LD2_PIN, LD_PORT, PushPull, High, NoPull);
	GPIO_OutputPinInit(LD3_PIN, LD_PORT, PushPull, High, NoPull);

	/*
	 * User Button = PC13
	 */
	GPIO_InputPinInit(BUTTON_PIN, BUTTON_PORT, NoPull);

}
