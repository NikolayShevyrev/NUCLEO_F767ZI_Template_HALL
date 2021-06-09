/*
 * adc.cpp
 *
 *  Created on: 31 мар. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#include "adc.h"

void ADC_Init(){
	/*
	 * ADC123 Clock Enable
	 */
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC1EN);
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC2EN);
	SET_BIT(RCC->APB2ENR, RCC_APB2ENR_ADC3EN);

	ADCPins_Init();

	/*
	 * ADC1
	 * Channel 11
	 * Current B
	 */
	SET_BIT(ADC1->CR2, ADC_CR2_CONT);
	//SET_BIT(ADC1->CR2, ADC_CR2_DMA);
	//SET_BIT(ADC1->CR2, ADC_CR2_DDS);
	SET_BIT(ADC1->SQR3, ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_1 | ADC_SQR3_SQ1_3);

	/*
	 * ADC2
	 * Channel 1
	 * VBUS
	 */
	SET_BIT(ADC2->CR2, ADC_CR2_CONT);
	//SET_BIT(ADC2->CR2, ADC_CR2_DMA);
	//SET_BIT(ADC2->CR2, ADC_CR2_DDS);
	SET_BIT(ADC2->SQR3, ADC_SQR3_SQ1_0);

	/*
	 * ADC3
	 * Channel 13/14/15
	 * BEMF 1/2/3
	 */
	SET_BIT(ADC3->CR2, ADC_CR2_CONT);
	//SET_BIT(ADC3->CR2, ADC_CR2_DMA);
	//SET_BIT(ADC3->CR2, ADC_CR2_DDS);
	SET_BIT(ADC3->SQR3, ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2 | ADC_SQR3_SQ1_3);

}

void ADCPins_Init(){
	/*
	 * Ports Clock Enable
	 */
	GPIO_PortClockInit(BEMF1_PORT);
	GPIO_PortClockInit(VBUS_PORT);
	GPIO_PortClockInit(CURR_B_PORT);

	/*
	 * PA0 -> Current A
	 * PC1 -> Current B
	 * PC0 -> Current C
	 * PA1 -> VBUS
	 * PC2 -> Temperature
	 */
	GPIO_AnalogPinInit(CURR_A_PIN, CURR_A_PORT);
	GPIO_AnalogPinInit(CURR_B_PIN, CURR_B_PORT);
	GPIO_AnalogPinInit(CURR_C_PIN, CURR_C_PORT);
	GPIO_AnalogPinInit(VBUS_PIN, VBUS_PORT);
	GPIO_AnalogPinInit(TEMP_PIN, TEMP_PORT);

	/*
	 * PC3 -> BEMF1
	 * PC4 -> BEMF2
	 * PC5 -> BEMF3
	 */
	GPIO_AnalogPinInit(BEMF1_PIN, BEMF1_PORT);
	GPIO_AnalogPinInit(BEMF2_PIN, BEMF2_PORT);
	GPIO_AnalogPinInit(BEMF3_PIN, BEMF3_PORT);

	/*
	 * PC9 -> GPIO_BEMF
	 */
	GPIO_OutputPinInit(GPIO_BEMF_PIN, GPIO_BEMF_PORT, PushPull, High, NoPull);
}
