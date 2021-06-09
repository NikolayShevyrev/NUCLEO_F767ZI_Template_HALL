/*
 * main.cpp
 *
 *  Created on: Oct 6, 2020
 *      Author: Nikolay Shevyrev
 */

#include <sixstepcomm.h>
#include "main.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "timers.h"
#include "adc.h"
#include "dfilter.h"
#include "settings.h"


/* Function prototypes -----------------------------------------------*/
void CheckButton(void);

/* Variables ---------------------------------------------------------*/
SixStepCommutation Motor;

Timer1 timer1;
Timer7 timer7;
Timer6 timer6;

NonBlockingDelay nbDelay;

dFilter<uint16_t, 4> rpmFilter;

SixStepCommSettings settings;

States currentState = StoppedState;
bool fStartUp = false;

const int n = 10;
std::array<int, n> g;

const uint16_t HALL_STATES_CLKW[6] = {0x0001,0x0005,0x0004,0x0006,0x0002,0x0003};

uint32_t CCR_value, dvalue = 10000;


/**
  * @brief  The application entry point.
  * @retval int
  */
int main(){

	SystemClock_Config();

	GPIO_Init();

	ADC_Init();

	timer1.Init(FPWM);

	timer7.Init();

	// Fill Motor Settings
	FillSixStepCommSettings(settings);

	//Init motor
	Motor.Init(settings, timer7.GetPSC(), FPWM, timer1.GetPWMPeriod());
	Motor.SetDiraction(Clockwise);

	BEMFPullHigh();
	AllADCsOn();
	AllADCsStartConv();

	timer1.Start();

	timer1.PWMOutputsOn();

	//timer2.Init();

	//uint8_t i = 0;

	//timer2.Start();

	/* Infinite loop */
	while(1){
		//WRITE_REG(HALL_OUT_PORT->ODR, (HALL_STATES_CLKW[i] << 4));
		//if((++i) > 5) { i = 0; }
		GPIO_TogglePin(LD3_PIN, LD_PORT);
		GPIO_TogglePin(LEDX_PIN, LEDX_PORT);

		DelayUS(dvalue);
	}

}


/**
  * @brief  Does stuff if user button was pressed
  * @retval None
  */
void CheckButton(void){
	if(GPIO_ReadPin(BUTTON_PIN, BUTTON_PORT)){
		while(GPIO_ReadPin(BUTTON_PIN, BUTTON_PORT)){
			DelayUS(10);
		}
		/* Do somesthing */
		GPIO_TogglePin(LD1_PIN, LD_PORT);
	}
}
