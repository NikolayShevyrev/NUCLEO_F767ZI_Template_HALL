/*
 * systick.cpp
 *
 *  Created on: 8 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 */


#include "systick.h"

volatile static uint32_t delay;

/*!
    \brief      configure systick
    \param[in]  none
    \param[out] none
    \retval     none
*/
void systick_config()
{
	if(SysTick_Config(SystemCoreClock/1000U))
	{
		while(1)
		{
			// Catch error
		}
	}

	NVIC_EnableIRQ(SysTick_IRQn);
	NVIC_SetPriority (SysTick_IRQn, 0);
}

/*!
    \brief      delay a time in milliseconds
    \param[in]  count: count in milliseconds
    \param[out] none
    \retval     none
*/
void delay_ms(uint32_t count)
{
	delay = count;

	while(delay != 0U)
	{

	}
}


/*!
    \brief      delay decrement
    \param[in]  none
    \param[out] none
    \retval     none
*/
void delay_decrement(void)
{
    if (delay != 0U){
        delay--;
    }
}
