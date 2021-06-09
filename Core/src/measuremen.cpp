/*
 * dma.cpp
 *
 *  Created on: 1 апр. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#include "measurement.h"
#include "stm32f767xx.h"

void Measurement::DMA_Init(){
	/*
	 * DMA2 Clock Enable
	 */
	SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN);

	/*
	 * DMA2 Stream 4 Channel 0
	 * ADC1 -> Current B
	 */
	CLEAR_BIT(DMA2_Stream4->CR, DMA_SxCR_CHSEL);



	WRITE_REG(DMA2_Stream4->PAR,  (uint32_t)&ADC1->DR);
	WRITE_REG(DMA2_Stream4->M0AR, (uint32_t)&PhaseCurrentB[0]);

	/*
	 * DMA2 Stream 2 Channel 1
	 * ADC2 -> VBus
	 */

	/*
	 * DMA2 Stream 0 Channel 2
	 * ADC3 -> BEMF
	 */
}



