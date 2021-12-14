/*
 * insulation_measurement.cpp
 *
 *  Created on: 10 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 *
 */


#include "main.h"
#include "systick.h"


/**
 * @brief Measure vpos or vneg
 *
 * @return uint32_t
 */
uint32_t measure_voltage(ADC_TypeDef * adc_base)
{
	uint32_t result = 0;

	SET_BIT(adc_base->CR2, ADC_CR2_JSWSTART);
	while(!READ_BIT(adc_base->SR, ADC_SR_JEOC));

	result += READ_REG(adc_base->JDR1);
	result += READ_REG(adc_base->JDR2);
	result += READ_REG(adc_base->JDR3);
	result += READ_REG(adc_base->JDR4);

	result = result >> 2;

	return result;
}

/**
 * @brief Isolation measurement
 *
 */
bool insulation_measurement(void)
{
	uint32_t iso_pos = 0;
	uint32_t iso_neg = 0;

	delay_ms(50);

	/* Check initial values */
	iso_pos = measure_voltage(VPOS);
	iso_neg = measure_voltage(VNEG);

	delay_ms(50);

	/* Turn on Pos Relay */
	RELAY_CLOSE(RELAY_POS);

	delay_ms(50);

	iso_pos = measure_voltage(VPOS);
	iso_neg = measure_voltage(VNEG);

	delay_ms(50);

	/* Turn on Neg Relay (Both Relays closed) */
	RELAY_CLOSE(RELAY_NEG);

	delay_ms(50);

	iso_pos = measure_voltage(VPOS);
	iso_neg = measure_voltage(VNEG);

	delay_ms(50);

	/* Turn off Pos Relay (Only Neg Relay Closed) */
	RELAY_OPEN(RELAY_POS);

	delay_ms(50);

	iso_pos = measure_voltage(VPOS);
	iso_neg = measure_voltage(VNEG);

	delay_ms(50);

	/* Turn off Neg Relay */
	RELAY_OPEN(RELAY_NEG);
	
	return true;
}



