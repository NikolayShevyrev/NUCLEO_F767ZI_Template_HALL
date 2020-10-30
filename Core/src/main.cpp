/*
 * main.cpp
 *
 *  Created on: Oct 6, 2020
 *      Author: Nikolay Shevyrev
 */

#include "main.h"
#include "delay.h"
#include "gpio.h"
#include "uart.h"
#include "timers.h"
#include "dfilter.h"

/* Function prototypes -----------------------------------------------*/
void CheckButton(void);

/* Variables ---------------------------------------------------------*/
Timer1 timer1;
Timer2 timer2;
NonBlockingDelay nbDelay;
dFilter<uint32_t, 16> rpmFilter;

const uint16_t HALL_STATES_CLKW[6] = {0x0001,0x0005,0x0004,0x0006,0x0002,0x0003};

uint32_t CCR_value, dvalue = 1000;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(){

	SystemClock_Config();

	GPIO_Init();


	timer1.Init(FPWM);

	timer2.Init();

	uint8_t i = 0;

	timer2.Start();

	/* Infinite loop */
	while(1){
		WRITE_REG(HALL_OUT_PORT->ODR, (HALL_STATES_CLKW[i] << 4));
		if((++i) > 5) { i = 0; }
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
