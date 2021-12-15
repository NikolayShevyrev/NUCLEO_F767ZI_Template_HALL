/*
 * insulation_measurement.cpp
 *
 *  Created on: 10 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 *
 */


#include "main.h"
#include "systick.h"
#include <array>


/**
 * @brief Measure vpos or vneg
 *
 * @return uint32_t
 */
float measure_voltage(ADC_TypeDef * adc_base)
{
	uint32_t result = 0;

	SET_BIT(adc_base->CR2, ADC_CR2_JSWSTART);
	while(!READ_BIT(adc_base->SR, ADC_SR_JEOC));

	result += READ_REG(adc_base->JDR1);
	result += READ_REG(adc_base->JDR2);
	result += READ_REG(adc_base->JDR3);
	result += READ_REG(adc_base->JDR4);

	result = result >> 2;

	return ((float)result) * ADC_COEFF;
}

extern uint32_t iso_meas_delay;
extern std::array<float, 4> iso_pos;
extern std::array<float, 4> iso_neg;

/**
 * @brief Isolation measurement
 *
 */
bool insulation_measurement(void)
{

	delay_ms(iso_meas_delay);

	/* Check initial values */
	iso_pos[0] = measure_voltage(VPOS);
	iso_neg[0] = measure_voltage(VNEG);

	delay_ms(iso_meas_delay);

	/* Turn on Pos Relay */
	RELAY_CLOSE(RELAY_POS);

	delay_ms(iso_meas_delay);

	iso_pos[1] = measure_voltage(VPOS);
	iso_neg[1] = measure_voltage(VNEG);

	delay_ms(iso_meas_delay);

	/* Turn on Neg Relay (Both Relays closed) */
	RELAY_CLOSE(RELAY_NEG);

	delay_ms(iso_meas_delay);

	iso_pos[2] = measure_voltage(VPOS);
	iso_neg[2] = measure_voltage(VNEG);

	delay_ms(iso_meas_delay);

	/* Turn off Pos Relay (Only Neg Relay Closed) */
	RELAY_OPEN(RELAY_POS);

	delay_ms(iso_meas_delay);

	iso_pos[3] = measure_voltage(VPOS);
	iso_neg[3] = measure_voltage(VNEG);

	delay_ms(iso_meas_delay);

	/* Turn off Neg Relay */
	RELAY_OPEN(RELAY_NEG);
	
	return true;
}



