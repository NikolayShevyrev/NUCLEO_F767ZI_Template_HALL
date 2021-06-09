/*
 * dma.h
 *
 *  Created on: 2 апр. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_DMA_H_
#define INC_DMA_H_

#include "main.h"

enum PriorityLevels{
	Low 	 = 0b00,
	Medium	 = 0b01,
	High 	 = 0b10,
	VeryHigh = 0b11
};

enum Size{
	Byte 	 = 0b00,
	HalfWord = 0b01,
	Word 	 = 0b10
};

enum DataDiractoin{
	PeripheralToMemory	= 0b00,
	MemoryToPeripheral	= 0b01,
	MemoryToMemory		= 0b10
};

class DMA {
private:
	DMA_Stream_TypeDef * base;
public:
	DMA(DMA_Stream_TypeDef * dma_base) : base(dma_base){
	}

	void ChannelSelect(uint16_t channel){
		if(!READ_BIT(base->CR, DMA_SxCR_EN) && (channel < 16)){
			SET_BIT(base->CR, channel << DMA_SxCR_CHSEL_Pos);
		}
	}

	void PriorityLevel(PriorityLevels level){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, level << DMA_SxCR_PL_Pos);
		}
	}

	void MemoryDataSize(Size size){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, size << DMA_SxCR_MSIZE_Pos);
		}
	}

	void PeripheralDataSize(Size size){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, size << DMA_SxCR_PSIZE_Pos);
		}
	}

	void MemoryIncrementMode(){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, DMA_SxCR_MINC);
		}
	}

	void PeripheralIncrementMode(){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, DMA_SxCR_PINC);
		}
	}

	void CircularMode(){
		SET_BIT(base->CR, DMA_SxCR_CIRC);
	}

	void DataTransferDiraction(DataDiraction dir){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			SET_BIT(base->CR, dir << DMA_SxCR_DIR);
		}
	}

	void TransferCompleteInterruptEnable(){
		SET_BIT(base->CR, DMA_SxCR_TCIE);
	}

	void StreamEnable(){
		SET_BIT(base->CR, DMA_SxCR_EN);
	}

	void NumberOfData(uint16_t number){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			WRITE_REG(base->NDTR, number);
		}
	}

	void PeripheralAddress(uint32_t address){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			WRITE_REG(base->PAR, address);
		}
	}

	void Memory0Address(uint32_t address){
		if(!READ_BIT(base->CR, DMA_SxCR_EN)){
			WRITE_REG(base->M0AR, address);
		}
	}

	void SetPriorityIRQ(uint32_t priority){
		switch(base){
			case DMA2_Stream0:
				NVIC_SetPriority(DMA2_Stream0_IRQn, priority);
				break;
			case DMA2_Stream1:
				NVIC_SetPriority(DMA2_Stream1_IRQn, priority);
				break;
			case DMA2_Stream2:
				NVIC_SetPriority(DMA2_Stream2_IRQn, priority);
				break;
			case DMA2_Stream3:
				NVIC_SetPriority(DMA2_Stream3_IRQn, priority);
				break;
			case DMA2_Stream4:
				NVIC_SetPriority(DMA2_Stream4_IRQn, priority);
				break;
			case DMA2_Stream5:
				NVIC_SetPriority(DMA2_Stream5_IRQn, priority);
				break;
			case DMA2_Stream6:
				NVIC_SetPriority(DMA2_Stream6_IRQn, priority);
				break;
			case DMA2_Stream7:
				NVIC_SetPriority(DMA2_Stream7_IRQn, priority);
				break;
		}
	}

	void EnableIRQ(){
		switch(base){
			case DMA2_Stream0:
				NVIC_EnableIRQ(DMA2_Stream0_IRQn);
				break;
			case DMA2_Stream1:
				NVIC_EnableIRQ(DMA2_Stream1_IRQn);
				break;
			case DMA2_Stream2:
				NVIC_EnableIRQ(DMA2_Stream2_IRQn);
				break;
			case DMA2_Stream3:
				NVIC_EnableIRQ(DMA2_Stream3_IRQn);
				break;
			case DMA2_Stream4:
				NVIC_EnableIRQ(DMA2_Stream4_IRQn);
				break;
			case DMA2_Stream5:
				NVIC_EnableIRQ(DMA2_Stream5_IRQn);
				break;
			case DMA2_Stream6:
				NVIC_EnableIRQ(DMA2_Stream6_IRQn);
				break;
			case DMA2_Stream7:
				NVIC_EnableIRQ(DMA2_Stream7_IRQn);
				break;
		}
	}

	void DisableIRQ(){
			switch(base){
				case DMA2_Stream0:
					NVIC_DisableIRQ(DMA2_Stream0_IRQn);
					break;
				case DMA2_Stream1:
					NVIC_DisableIRQ(DMA2_Stream1_IRQn);
					break;
				case DMA2_Stream2:
					NVIC_DisableIRQ(DMA2_Stream2_IRQn);
					break;
				case DMA2_Stream3:
					NVIC_DisableIRQ(DMA2_Stream3_IRQn);
					break;
				case DMA2_Stream4:
					NVIC_DisableIRQ(DMA2_Stream4_IRQn);
					break;
				case DMA2_Stream5:
					NVIC_DisableIRQ(DMA2_Stream5_IRQn);
					break;
				case DMA2_Stream6:
					NVIC_DisableIRQ(DMA2_Stream6_IRQn);
					break;
				case DMA2_Stream7:
					NVIC_DisableIRQ(DMA2_Stream7_IRQn);
					break;
			}
		}


};


#endif /* INC_DMA_H_ */
