/*
 * insulation_measurement.cpp
 *
 *  Created on: 10 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 *
 */


#include "insulation_measurement.h"

insulation_measurement::insulation_measurement(float r_s1, float r_s2, float dc_voltage, uint32_t meas_delay)
        : r_s1_(r_s1), r_s2_(r_s2), dc_voltage_(dc_voltage), iso_meas_delay_(meas_delay)
{

}

/**
 * @brief Measure vpos or vneg
 *
 * @return uint32_t
 */
float insulation_measurement::measure_voltage(ADC_TypeDef * adc_base)
{
	uint32_t result = 0;

	SET_BIT(adc_base->CR2, ADC_CR2_JSWSTART);
	while(!READ_BIT(adc_base->SR, ADC_SR_JEOC));

	result += READ_REG(adc_base->JDR1);
	result += READ_REG(adc_base->JDR2);
	result += READ_REG(adc_base->JDR3);
	result += READ_REG(adc_base->JDR4);

	CLEAR_BIT(adc_base->SR, ADC_SR_JEOC);

	result = result >> 2;

	return ((float)result) * ADC_COEFF;
}



/**
 * @brief Isolation measurement
 *
 */
bool insulation_measurement::run(void)
{

	delay_ms(iso_meas_delay_);

	/* Check initial values */
	iso_pos.ref = measure_voltage(VPOS);
	iso_neg.ref = measure_voltage(VNEG);

	delay_ms(iso_meas_delay_);

	/* Turn on Pos Relay */
	RELAY_CLOSE(RELAY_POS);

	delay_ms(iso_meas_delay_);

	iso_pos.s1 = measure_voltage(VPOS);
	iso_neg.s1 = measure_voltage(VNEG);

	delay_ms(iso_meas_delay_);

	/* Turn on Neg Relay (Both Relays closed) */
	RELAY_CLOSE(RELAY_NEG);

	delay_ms(iso_meas_delay_);

	iso_pos.s1_s2 = measure_voltage(VPOS);
	iso_neg.s1_s2 = measure_voltage(VNEG);

	delay_ms(iso_meas_delay_);

	/* Turn off Pos Relay (Only Neg Relay Closed) */
	RELAY_OPEN(RELAY_POS);

	delay_ms(iso_meas_delay_);

	iso_pos.s2 = measure_voltage(VPOS);
	iso_neg.s2 = measure_voltage(VNEG);

	delay_ms(iso_meas_delay_);

	/* Turn off Neg Relay */
	RELAY_OPEN(RELAY_NEG);
	
	delay_ms(iso_meas_delay_);

	calc_alpha();
	calc_beta();

	calc_r_iso_p();
	calc_r_iso_n();

	return true;
}


/**
 * @brief Calculate voltage at hv battery positive terminal refered to chassis ground
 * 
 * @return float 
 */
void insulation_measurement::calc_alpha()
{
	alpha_ = iso_pos.ref * (1.F + r_s2_/r_s1_) - (r_s2_/r_s1_) * iso_pos.s1;
}

/**
 * @brief Calculate voltage at hv battery negative terminal refered to chassis ground
 * 
 * @return float 
 */
void insulation_measurement::calc_beta()
{
	beta_ = iso_neg.ref * (1.F + r_s2_/r_s1_) - (r_s2_/r_s1_) * iso_neg.s2;
}

/**
 * @brief Calculate insulation resistance for hv battery positive terminal to chassis ground
 * 
 * @return float 
 */
void insulation_measurement::calc_r_iso_p()
{
	r_iso_p_ = (dc_voltage_ * dc_voltage_ * r_s2_ + dc_voltage_ * r_s2_ * beta_ - dc_voltage_ * r_s2_ * alpha_)/
		   	   (dc_voltage_ * iso_pos.ref - dc_voltage_ * beta_ + iso_pos.ref * beta_ - iso_pos.ref * alpha_);
}

/**
 * @brief Calculate insulation resistance for hv battery negative terminal to chassis ground
 * 
 * @return float 
 */
void insulation_measurement::calc_r_iso_n()
{
	r_iso_n_ =  (dc_voltage_ * dc_voltage_ * r_s2_ + dc_voltage_ * r_s2_ * beta_ - dc_voltage_ * r_s2_ * alpha_)/
		   	    (dc_voltage_ * iso_neg.ref - dc_voltage_ * alpha_ + iso_neg.ref * beta_ - iso_neg.ref * alpha_);
}