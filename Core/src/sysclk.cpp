/*
 * sysclk.cpp
 *
 *  Created on: 15 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */


#include "main.h"

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void){

	/*
	 *  Configure the main internal regulator output voltage
	  */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);				// Power interface clock enable
	while(!READ_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN));
	MODIFY_REG(PWR->CR1, PWR_CR1_VOS, PWR_CR1_VOS);			// Configure the main internal regulator output voltage: Scale 1

	CLEAR_BIT(RCC->CR, RCC_CR_PLLON); 						// PLL Off
	while(READ_BIT(RCC->CR, RCC_CR_PLLRDY));				// Wait till PLL is ready

	/*
	 * HSE Configuration
	  */
	SET_BIT(RCC->CR, RCC_CR_HSEBYP);						// HSE oscillator bypassed with an external clock
	SET_BIT(RCC->CR, RCC_CR_HSEON);							// HSE oscillator ON
	while(!READ_BIT(RCC->CR,RCC_CR_HSERDY)); 				// Wait till HSE is ready

	/*
	 * PLL Configuration
	 */
	RCC->PLLCFGR =
			(RCC_PLLCFGR_PLLSRC_HSE 				 |		// HSE oscillator clock selected as PLL
			4          								 |		// PLLM = 4
	        (216 << RCC_PLLCFGR_PLLN_Pos)            |		// PLLN = 216
	        (((2 >> 1) -1) << RCC_PLLCFGR_PLLP_Pos)  |		// PLLP = 2
	        (2 << RCC_PLLCFGR_PLLQ_Pos));					// PLLQ = 2
	        //(2 << RCC_PLLCFGR_PLLR_Pos));					// PLLR = 2

	SET_BIT(RCC->CR, RCC_CR_PLLON); 						// PLL On
	while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY));				// Wait till PLL is ready

	/*
	 * Activate the Over-Drive mode
	 */
	SET_BIT(PWR->CR1, PWR_CR1_ODEN);						// Over-drive enabled
	while(!READ_BIT(PWR->CSR1, PWR_CSR1_ODRDY));			// Wait till Over-drive mode is ready

	SET_BIT(PWR->CR1, PWR_CR1_ODSWEN);						// Over-drive switching enabled
	while(!READ_BIT(PWR->CSR1, PWR_CSR1_ODSWRDY));			// Wait till Over-drive mode switching is ready

	/*
	 * Flash latency configuration
	 */
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_7WS);// Program the new LATENCY  in the FLASH_ACR register
	while(READ_BIT((FLASH->ACR), FLASH_ACR_LATENCY) != FLASH_ACR_LATENCY_7WS);// Wait till Flash is ready

	/*
	 * HCLK configuration
	 */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV16);	// Set the highest APBx dividers
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, (RCC_CFGR_PPRE1_DIV16 << 3));

	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);		// AHB prescaler = 1
	while(!READ_BIT(RCC->CR, RCC_CR_PLLRDY));						// Check if PLL is ready

	/*
	 * SYSCLK Configuration
	 */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);			// PLL selected as system clock
	while(READ_BIT(RCC->CFGR, RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);	// Wait till clock is ready

	/*
	 * PCLK1 Configuration
	 */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV4);		// APB1 prescaler = 4

	/*
	 * PCLK2 Configuration
	 */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV2);		// APB2 prescaler = 2

	SystemCoreClockUpdate();										// Update SystemCoreClock variable according to Clock Register Values

}
