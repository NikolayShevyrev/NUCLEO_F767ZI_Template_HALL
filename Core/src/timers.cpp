/*
 * timers.cpp
 *
 *  Created on: 23 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "timers.h"
#include "gpio.h"

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
	CLEAR_BIT(TIM1->SR, TIM_SR_UIF);

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
	CLEAR_BIT(TIM1->SR, TIM_SR_COMIF);

	/*
	 * Enable Timer 1 Update Interrupt
	 */
	SET_BIT(TIM1->DIER, TIM_DIER_UIE);
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 0);
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

void Timer2::Init(){
	/*
	 * Timer 2 Clock Enable
	 * f = 108 MHz
	 */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);

	/*
	 * Timer 2 Pins configuration
	 */
	PinsInit();

	/*
	 * Setting CNT, ARR and PSC values
	 */
	WRITE_REG(TIM2->CNT, 0x00000000);		// Resetting timer
	WRITE_REG(TIM2->PSC, 107);		// T = 1 us
	WRITE_REG(TIM2->ARR, 0xFFFFFFFF);

	/* Generate update event */
	SET_BIT(TIM2->EGR, TIM_EGR_UG);
	CLEAR_BIT(TIM2->SR, TIM_SR_UIF);

	/* Timer input XOR function enabled */
	SET_BIT(TIM2->CR2, TIM_CR2_TI1S);

	/*
	 * Slave Mode Configuration
	 *
	 * Reset Mode
	 * Trigger selection: Filtered Timer Input 1 (TI1FP1)
	 */
	SET_BIT(TIM2->SMCR, TIM_SMCR_SMS_2 |
						TIM_SMCR_TS_0 | TIM_SMCR_TS_2);

	/*
	 * Input capture mode configuration
	 *
	 * CC1 channel is configured as input, IC1 is mapped on TI1
	 * Input capture 1 filter: fSAMPLING=fDTS/32, N=8 -> 1us
	 * Polarity: noninverted/both edges
	 */
	SET_BIT(TIM2->CCMR1, TIM_CCMR1_CC1S_0 |
						 TIM_CCMR1_IC1F);
	SET_BIT(TIM2->CCER, TIM_CCER_CC1E |
						TIM_CCER_CC1P | TIM_CCER_CC1NP);

	/*
	 * Enable Timer 2 Capture/Compare 1 interrupt
	 */
	SET_BIT(TIM2->DIER, TIM_DIER_CC1IE);
	NVIC_SetPriority(TIM2_IRQn, 0);
	NVIC_EnableIRQ(TIM2_IRQn);

	SET_BIT(DBGMCU->APB1FZ, DBGMCU_APB1_FZ_DBG_TIM2_STOP);

}

void Timer2::PinsInit(){
	/*
	 * Ports Clock Enable
	 */
	GPIO_PortClockInit(HALL1_IN_PORT);
	GPIO_PortClockInit(HALL23_IN_PORT);

	/*
	 * Timer 2 Pins
	 * PA15 -> HALL1_IN
	 * PB3  -> HALL2_IN
	 * PB10 -> HALL3_IN
	 */
	GPIO_AFPinInit(HALL1_IN_PIN, HALL1_IN_PORT, PushPull, VeryHigh, NoPull, AF1);
	GPIO_AFPinInit(HALL2_IN_PIN, HALL23_IN_PORT, PushPull, VeryHigh, NoPull, AF1);
	GPIO_AFPinInit(HALL3_IN_PIN, HALL23_IN_PORT, PushPull, VeryHigh, NoPull, AF1);
}

