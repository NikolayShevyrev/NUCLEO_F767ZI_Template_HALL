/*
 * stm32f7xx_it.h
 *
 *  Created on: 16 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_STM32F7XX_IT_H_
#define INC_STM32F7XX_IT_H_


/* Functions prototypes ---------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


#endif /* INC_STM32F7XX_IT_H_ */
