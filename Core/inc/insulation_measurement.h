/*
 * insulation_measurement.h
 *
 *  Created on: 23 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_INSULATION_MEASUREMENT_H_
#define INC_INSULATION_MEASUREMENT_H_

#include "main.h"
#include "systick.h"

struct iso_voltage
{
    float ref;
    float s1;
    float s1_s2;
    float s2;
};

class insulation_measurement
{
private:
    iso_voltage iso_pos;
    iso_voltage iso_neg;

    float offset_p_;
    float offset_n_;

    /* Hardware parameters */
    float r_s1_;
    float r_s2_;

    /* Insulation resistances */
    float r_iso_p_;
    float r_iso_n_;

    /* High-battery terminal voltages referred to chassis ground */
    float alpha_;
    float beta_;

    float dc_voltage_;

    uint32_t iso_meas_delay_;
    
public:
    insulation_measurement(float r_s1, float r_s2, float dc_voltage, uint32_t meas_delay);

    ~insulation_measurement(){}

    insulation_measurement(const insulation_measurement& obj) = delete;

    insulation_measurement operator=(const insulation_measurement& obj) = delete;

    bool run(void);

    void calibrate(uint32_t count);

    float get_r_iso_p(void) const { return r_iso_p_; }

    float get_r_iso_n(void) const { return r_iso_n_; }

private:
    float measure_voltage(ADC_TypeDef * adc_base);

    void calc_alpha(void);

    void calc_beta(void);

    void calc_r_iso_p(void);

    void calc_r_iso_n(void);

};

#endif /* INC_INSULATION_MEASUREMENT_H_ */
