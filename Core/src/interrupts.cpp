/*
 * interrupts.cpp
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "timers.h"
#include "main.h"
#include "delay.h"
#include "gpio.h"
#include "dfilter.h"

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

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



void TIM2_IRQHandler(void) {

	extern Timer2 timer2;
	extern uint32_t CCR_value;
	extern dFilter<uint32_t, 16> rpmFilter;

	CCR_value = timer2.GetCCR1();
	timer2.ClearSR();

	CCR_value = rpmFilter.Calc(CCR_value);

	GPIO_TogglePin(LD1_PIN, LD_PORT);

}


#ifdef __cplusplus
}
#endif /* __cplusplus */

