/*
 * interrupts.cpp
 *
 *  Created on: 26 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include <sixstepcomm.h>
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
	extern SixStepCommutation Motor;
	extern States currentState;

	timer1.ClearUIF();

	if(currentState == StartingState){
		Motor.Start();
	}

	nbDelay.Tick();

}

void TIM6_DAC_IRQHandler(void) {
	extern Timer6 timer6;

	timer6.ClearUIF();
}

/*void TIM2_IRQHandler(void) {

	extern Timer2 timer2;
	extern uint32_t CCR_value;
	extern dFilter<uint32_t, 16> rpmFilter;

	CCR_value = timer2.GetCCR1();
	timer2.ClearSR();

	CCR_value = rpmFilter.Calc(CCR_value);

	GPIO_TogglePin(LD3_PIN, LD_PORT);

}*/


#ifdef __cplusplus
}
#endif /* __cplusplus */

