/*
 * timers.cpp
 *
 *  Created on: 23 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "timers.h"

void Timer1::Init(uint16_t pwmFrequnce){

	pwmFrequnce_ = pwmFrequnce;
	pwm100usFactor = pwmFrequnce / 10000;
	period_ = ((SystemCoreClock/pwmFrequnce_)/2)-1;
	/*
	 * Timer 1 Clock Enable
	 * f = 216 MHz
	 */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);

	/*
	 * Timer 1 CR1 Configuration
	 *
	 * Auto-reload preload enable
	 * Center-aligned mode 1 is enabled
	 */
	SET_BIT(TIM1->CR1, TIM_CR1_ARPE | TIM_CR1_CMS_0);

	/*
	 * Timer 1 CR2 Configuration
	 *
	 * Master mode selection: OC1REF signal is used as trigger output
	 * Capture/compare preloaded control: CCxE, CCxNE and OCxM bits are preloaded
	*/
	SET_BIT(TIM1->CR2, TIM_CR2_MMS_2 | TIM_CR2_CCPC);

	/*
	 * Set ARR, PSC and RCR values
	 */
	WRITE_REG(TIM1->ARR, period_);
	WRITE_REG(TIM1->PSC, 0);
	WRITE_REG(TIM1->RCR, 1);

	/* Generate update event */
	SET_BIT(TIM1->EGR, TIM_EGR_UG);

	/*
	 * Capture/Compare Mode Configuration
	 *
	 * Output Compare preload enabled
	 * Output Compare mode: PWM mode 2
	 */
	SET_BIT(TIM1->CCMR1, TIM_CCMR1_OC1PE | TIM_CCMR1_OC1M |
						 TIM_CCMR1_OC2PE | TIM_CCMR1_OC2M);
	SET_BIT(TIM1->CCMR2, TIM_CCMR2_OC3PE | TIM_CCMR2_OC3M);

	/*
	 * Enabling Capture/Compare channels
	 */
	SET_BIT(TIM1->CCER, TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E |
						TIM_CCER_CC1NE | TIM_CCER_CC2NE | TIM_CCER_CC3NE);

	/*
	 * Set defaulte Capture/Compare values: 50% duty sycle
	 */
	WRITE_REG(TIM1->CCR1, period_/2);
	WRITE_REG(TIM1->CCR2, period_/2);
	WRITE_REG(TIM1->CCR3, period_/2);

	/*
	 * Break and dead-time register Configuration
	 * Off-state selection for Run mode and Idle mode
	 * Dead time = 2.5 us
	 */
	SET_BIT(TIM1->BDTR, TIM_BDTR_OSSI | TIM_BDTR_OSSR);
	SET_BIT(TIM1->BDTR, 0x00D5);

	/* Generate Capture/Compare control update event */
	SET_BIT(TIM1->EGR, TIM_EGR_COMG);

	/*
	 * Enable Timer 1 Update Interrupt
	 */
	SET_BIT(TIM1->DIER, TIM_DIER_UIE);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void Timer6::Init(){
	/*
	 * Timer 6 Clock Enable
	 * f = 108 MHz
	 */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM6EN);

	WRITE_REG(TIM6->CNT, 0);		// Resetting timer
	WRITE_REG(TIM6->PSC, 107);		// T = 1 us
	WRITE_REG(TIM6->ARR, 0);

	/*
	 * Enable Timer 6 Update Interrupt
	 */
	SET_BIT(TIM6->DIER, TIM_DIER_UIE);
	NVIC_SetPriority(TIM6_DAC_IRQn, 0);
	NVIC_EnableIRQ(TIM6_DAC_IRQn);
}

void Timer7::Init(){
	/*
	 * Timer 7 Clock Enable
	 * f = 108 MHz
	 */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM7EN);

	WRITE_REG(TIM7->CNT, 0);		// Resetting timer
	WRITE_REG(TIM6->PSC, 107);		// T = 1 us
	WRITE_REG(TIM6->ARR, 0xFFFF);

}
