/*
 * timers.h
 *
 *  Created on: 23 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include "main.h"

class Timer {
private:
	TIM_TypeDef * timer_;
public:
	Timer(TIM_TypeDef * timer) :
		  timer_(timer){
	}

	virtual void Init(){

	}

	void Start() const { SET_BIT(timer_->CR1, TIM_CR1_CEN); }

	void Stop() const { CLEAR_BIT(timer_->CR1, TIM_CR1_CEN); }

	void ResetCNT() const { WRITE_REG(timer_->CNT, 0); }

	void SetCNT(uint16_t cnt) const { WRITE_REG(timer_->CNT, cnt); }

	uint16_t GetCNT() const { return READ_REG(timer_->CNT); }

	void SetARR(uint16_t arr) const { WRITE_REG(timer_->ARR, arr); }

	uint16_t GetARR() const { return READ_REG(timer_->ARR); }

	void SetPSC(uint16_t psc) const { WRITE_REG(timer_->PSC, psc); }

	uint16_t GetPSC() const { return READ_REG(timer_->PSC); }

	void ClearUIF()  const  { CLEAR_BIT(timer_->SR, TIM_SR_UIF); }

};

class Timer1 : public Timer {
private:
	uint16_t period_;
	uint16_t pwmFrequnce_;
	uint16_t pwm100usFactor;
	/*
	 * Capture/Compare output enable depending on the commutation sector
	 * 0x1 -> OCx is output, OCxN is not active
	 * 0x4 -> OCx is not active, OCxN is output
	 * 0x5 -> OCx is output, OCxN is output
	 */
	const uint16_t PWM_CCER_STATES_CLKW[6] = {0x0145,0x0415,0x0451,0x0154,0x0514,0x0541};
	uint16_t PWM_CCER_STATES[6];

	/*
	 * Capture/Compare output mode depending on the commutation sector
	 * 0x78 = PWM mode 2
	 * 0x58 = Force active level - OC1REF is forced high
	 * 0x48 = Force inactive level - OC1REF is forced low
	 */
	const uint16_t PWM_CCMR1_STATES_CLKW[6] = {0x5878,0x4878,0x7848,0x7858,0x4858,0x5848};
	const uint16_t PWM_CCMR2_STATES_CLKW[6] = {0x0048,0x0058,0x0058,0x0048,0x0078,0x0078};
	uint16_t PWM_CCMR1_STATES[6];
	uint16_t PWM_CCMR2_STATES[6];

public:
	Timer1() : Timer(TIM1) {
	}

	virtual void Init(uint16_t pwmFrequnce);

	void PWMOutputsOn(){
		SET_BIT(TIM1->BDTR, TIM_BDTR_MOE);
	}

	void PWMOutputsOff(){
		CLEAR_BIT(TIM1->BDTR, TIM_BDTR_MOE);
	}

	uint16_t Getpwm100usFactor(){
		return pwm100usFactor;
	}

	void SetDuty(uint16_t duty){
		if(duty < period_){
			duty = period_ - duty;
		} else {
			duty = 0;
		}
		WRITE_REG(TIM1->CCR1, duty);
		WRITE_REG(TIM1->CCR2, duty);
		WRITE_REG(TIM1->CCR3, duty);
	}

	void SetDiraction(bool dir){
		if(dir) {
			for(int i; i < 6; i++){
				PWM_CCER_STATES[i] = PWM_CCER_STATES_CLKW[i];
				PWM_CCMR1_STATES[i] = PWM_CCMR1_STATES_CLKW[i];
				PWM_CCMR2_STATES[i]	= PWM_CCMR2_STATES_CLKW[i];
			}
		} else {
			for(int i; i < 6; i++){
				PWM_CCER_STATES[i] = PWM_CCER_STATES_CLKW[5-i];
				PWM_CCMR1_STATES[i] = PWM_CCMR1_STATES_CLKW[5-i];
				PWM_CCMR2_STATES[i]	= PWM_CCMR2_STATES_CLKW[5-i];
			}
		}
	}

	void SwitchCommSector(uint16_t sector){
		TIM1->CCER 	= PWM_CCER_STATES[sector];
		TIM1->CCMR1 = PWM_CCMR1_STATES[sector];
		TIM1->CCMR2 = PWM_CCMR2_STATES[sector];

		/* Generate Capture/Compare control update event */
		TIM1->EGR |= TIM_EGR_COMG;
	}

};

class Timer6 : public Timer {
public:
	Timer6() : Timer(TIM6) {
	}

	virtual void Init();
};

class Timer7 : public Timer {
public:
	uint16_t value;
	uint16_t average;
public:
	Timer7() : Timer(TIM7) {
		}

	virtual void Init();
};

/* Function prototypes -----------------------------------------------*/
/*void Timer1_Init(void);
void Timer6_Init(void);
void Timer7_Init(void);

static inline void TimerStart(TIM_TypeDef * timer){
	SET_BIT(timer->CR1, TIM_CR1_CEN);
}

static inline void TimerStop(TIM_TypeDef * timer){
	CLEAR_BIT(timer->CR1, TIM_CR1_CEN);
}

static inline void TimerResetCNT(TIM_TypeDef * timer){
	WRITE_REG(timer->CNT, 0);
}

static inline void TimerSetCNT(TIM_TypeDef * timer, uint16_t cnt){
	WRITE_REG(timer->CNT, cnt);
}

static inline uint16_t TimerGetCNT(TIM_TypeDef * timer){
	return READ_REG(timer->CNT);
}

static inline void Timer1SetDuty(uint16_t duty){
	extern uint16_t ARR_value;
	if(duty < ARR_value){
		duty = ARR_value - duty;
	} else {
		duty = 0;
	}
	WRITE_REG(TIM1->CCR1, duty);
	WRITE_REG(TIM1->CCR2, duty);
	WRITE_REG(TIM1->CCR3, duty);
}*/

#endif /* INC_TIMERS_H_ */
