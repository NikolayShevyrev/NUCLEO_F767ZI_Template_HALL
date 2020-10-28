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

/* Function prototypes -----------------------------------------------*/
void CheckButton(void);

/* Variables ---------------------------------------------------------*/
static Timer1 timer1;
static Timer7 timer7;
static NonBlockingDelay nbDelay;
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(){

	SystemClock_Config();

	GPIO_Init();

	timer1.Init(FPWM);

	timer7.Init();

	/* Infinite loop */
	while(1){
		 CheckButton();
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
