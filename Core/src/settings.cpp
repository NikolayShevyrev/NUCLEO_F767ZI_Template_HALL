/*
 * settings.cpp
 *
 *  Created on: 29 мар. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#include "settings.h"

void FillSixStepCommSettings(SixStepCommSettings& settings){
	settings.pole_pairs 	= 4;
	settings.startup_duty 	= 0.3;
	settings.startup_rpm 	= 500;
	settings.max_rpm 		= 3000;
	settings.desired_rpm 	= 1000;
	settings.rotor_align_t	= 100;
	settings.startup_ramp_t = 200;
	settings.startup_sust_t = 50;
}
