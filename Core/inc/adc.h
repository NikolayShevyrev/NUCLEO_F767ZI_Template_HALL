/*
 * adc.h
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include "main.h"
#include "gpio.h"

void ADC_Init();
void ADCPins_Init();


static inline void ADCSwitchBEMFChannel(uint16_t sector){

}

static inline void ADC1On(){
	SET_BIT(ADC1->CR2, ADC_CR2_ADON);
}

static inline void ADC2On(){
	SET_BIT(ADC2->CR2, ADC_CR2_ADON);
}

static inline void ADC3On(){
	SET_BIT(ADC3->CR2, ADC_CR2_ADON);
}

static inline void AllADCsOn(){
	ADC1On();
	ADC2On();
	ADC3On();
}

static inline void ADC1StartConv(){
	SET_BIT(ADC1->CR2, ADC_CR2_SWSTART);
}

static inline void ADC2StartConv(){
	SET_BIT(ADC2->CR2, ADC_CR2_SWSTART);
}

static inline void ADC1StartInjConv(){
	SET_BIT(ADC1->CR2, ADC_CR2_JSWSTART);
}

static inline void ADC2StartInjConv(){
	SET_BIT(ADC2->CR2, ADC_CR2_JSWSTART);
}

static inline void ADC1Wait(){
	while(!READ_BIT(ADC1->SR, ADC_SR_JEOC));
}

static inline void ADC2Wait(){
	while(!READ_BIT(ADC2->SR, ADC_SR_JEOC));
}

static inline void ADC3StartConv(){
	SET_BIT(ADC3->CR2, ADC_CR2_SWSTART);
}

static inline void AllADCsStartConv(){
	ADC1StartConv();
	ADC2StartConv();
	ADC3StartConv();
}


#endif /* INC_ADC_H_ */
