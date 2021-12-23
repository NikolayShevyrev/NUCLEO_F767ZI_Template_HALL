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
#include "insulation_measurement.h"



/* Function prototypes -----------------------------------------------*/
void CheckButton(void);

insulation_measurement im(5100, 1205880, 300, 500);

dFilter<float, 8> rp_filter;
dFilter<float, 8> rn_filter;

float rp = 0;
float rn = 0;

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

	im.calibrate(10);

	/* Infinite loop */
	while(1)
	{
		CheckButton();

		/*if(meas_on_flag == true)
		{
			im.run();
			meas_on_flag = false;
			GPIO_ResetPin(LD3_PIN, LD_PORT);
		}
		*/
		im.run();
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
		//GPIO_TogglePin(LD3_PIN, LD_PORT);

		if(meas_on_flag == false)
		{
			meas_on_flag = true;
			GPIO_SetPin(LD3_PIN, LD_PORT);
		}
		else
		{
			meas_on_flag = false;
			GPIO_ResetPin(LD3_PIN, LD_PORT);
		}
	}
}
