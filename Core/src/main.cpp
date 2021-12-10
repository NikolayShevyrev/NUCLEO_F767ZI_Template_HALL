/*
 * main.cpp
 *
 *  Created on: Oct 6, 2020
 *      Author: Nikolay Shevyrev
 */

#include "main.h"
#include "delay.h"
#include "gpio.h"
#include "adc.h"
#include "dfilter.h"
#include "systick.h"


/* Function prototypes -----------------------------------------------*/
void CheckButton(void);


bool meas_on_flag = false;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(){

	SystemClock_Config();


	GPIO_Init();

	ADC_Init();

	ADC1On();
	ADC2On();

	systick_config();

	/* Infinite loop */
	while(1)
	{
		CheckButton();

		if(meas_on_flag == true)
		{
			insulation_measurement();
		}
	}

}


/**
  * @brief  Does stuff if user button was pressed
  * @retval None
  */
void CheckButton(void){
	if(GPIO_ReadPin(BUTTON_PIN, BUTTON_PORT)){
		while(GPIO_ReadPin(BUTTON_PIN, BUTTON_PORT)){
			DelayUS(100);
		}
		/* Do somesthing */
		GPIO_TogglePin(LD3_PIN, LD_PORT);

		if(meas_on_flag == false)
		{
			meas_on_flag = true;
		}
		else
		{
			meas_on_flag = false;
		}
	}
}
