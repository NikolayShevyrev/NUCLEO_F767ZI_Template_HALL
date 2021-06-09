/*
 * vector_math.h
 *
 *  Created on: 25 янв. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_VECTOR_MATH_H_
#define INC_VECTOR_MATH_H_

#include "main.h"
#include "arm_math.h"
#include "arm_common_tables.h"
#include <utility>
#include <array>

#define ONEbySQRT3		(float)0.57735026918963 	/* 1/sqrt(3) */
#define DEG60toRADIAN 	(float)1.0471975512    		/* 60 degrees in radians */

inline std::pair<float, float> ClarkTransform(float a, float b){

	float alpha = a;
	float beta = (2.f * b + a) * ONEbySQRT3;

	return std::make_pair(alpha, beta);
}

inline std::pair<float, float> ParkTransform(float alpha, float beta, float sin, float cos){

	float d = alpha * cos + beta * sin;
	float q = beta * cos - alpha * sin;

	return std::make_pair(d, q);
}

inline std::pair<float, float> InvParkTransform(float d, float q, float sin, float cos){

	float alpha = d * cos - q * sin;
	float beta = d * sin + q * cos;

	return std::make_pair(alpha, beta);
}

inline void CalcSVPWM(float alpha, float ref_voltage, float dc_voltage,
		uint16_t pwm_period, std::array<uint16_t, 3>& pwm_pulse_times){

	int32_t sector = (int32_t)(alpha / DEG60toRADIAN);
	float beta = alpha - (float)sector * DEG60toRADIAN;

	float tb1 = M_SQRT3 * (ref_voltage / dc_voltage) * arm_sin_f32(DEG60toRADIAN - beta);
	float tb2 = M_SQRT3 * (ref_voltage / dc_voltage) * arm_sin_f32(beta);
	float tb0 = (1.f - tb1 - tb2) / 2.f;

	std::array<float, 3> pulse_times;
	switch(sector)
	{
	case 0:
		pulse_times[0] = tb1 + tb2 + tb0;
		pulse_times[1] = tb2 + tb0;
		pulse_times[2] = tb0;
		break;
	case 1:
		pulse_times[0] = tb1 + tb0;
		pulse_times[1] = tb1 + tb2 + tb0;
		pulse_times[2] = tb0;
		break;
	case 2:
		pulse_times[0] = tb0;
		pulse_times[1] = tb1 + tb2 + tb0;
		pulse_times[2] = tb2 + tb0;
		break;
	case 3:
		pulse_times[0] = tb0;
		pulse_times[1] = tb1 + tb0;
		pulse_times[2] = tb1 + tb2 + tb0;
		break;
	case 4:
		pulse_times[0] = tb2 + tb0;
		pulse_times[1] = tb0;
		pulse_times[2] = tb1 + tb2 + tb0;
		break;
	case 5:
		pulse_times[0] = tb1 + tb2 + tb0;
		pulse_times[1] = tb0;
		pulse_times[2] = tb1 + tb0;
		break;
	default:
		break;
	}

	for(int i = 0; i < 3; i++)
	{
		if (pulse_times[i] < 0.f){
			pulse_times[i] = 0.f;
		} else if (pulse_times[i] > 1.f){
			pulse_times[i] = 1.f;
		}
		pwm_pulse_times[i] = (uin16_t)(pulse_times[i] * pwm_period);
	}

}

#endif /* INC_VECTOR_MATH_H_ */
