/*
 * interrupts.cpp
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "timers.h"
#include "main.h"
#include "delay.h"

/**
  * @brief  Timer1 PWM Interrupt Service Routine
  * @retval None
  */
void TIM1_UP_TIM10_IRQHandler(void) {

	extern Timer1 timer1;
	extern NonBlockingDelay nbDelay;

	timer1.ClearUIF();

	nbDelay.Tick();

}

