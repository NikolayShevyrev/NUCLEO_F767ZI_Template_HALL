/*
 * mctrl.cpp
 *
 *  Created on: 22 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include <sixstepcomm.h>
#include "timers.h"
#include "delay.h"
#include "dfilter.h"

void SixStepCommutation::Init(SixStepCommSettings& settings, uint16_t timer_psc, uint16_t pwm_freq, uint16_t pwm_period){
	StartUp.duty =(uint16_t)((float)pwm_period * settings.startup_duty);
	StartUp.rpm = settings.startup_rpm;
	pole_pairs = settings.pole_pairs;
	StartUp.Time.start = 20;
	StartUp.Time.alignment = settings.rotor_align_t;
	StartUp.Time.ramp = settings.startup_ramp_t;
	StartUp.Time.sust = settings.startup_sust_t;
	StartUp.sector_constant =(uint32_t)((float)1000000 * (float)((float)StartUp.Time.ramp/((float)StartUp.rpm * (float)pole_pairs)));

	minRPM = StartUp.rpm;
	maxRPM = settings.max_rpm;
	desiredRPM = settings.desired_rpm;

	timer_to_rpm = (uint32_t)((float)10 * (float)SystemCoreClock/((float)pole_pairs * (float)timer_psc));
	timer_min 	 = (uint32_t)((float)10 * (float)SystemCoreClock/((float)pole_pairs * (float)timer_psc * (float)maxRPM));
	timer_max 	 = (uint32_t)((float)10 * (float)SystemCoreClock/((float)pole_pairs * (float)timer_psc * (float)StartUp.rpm));

	maxDuty = (uint16_t)((float)pwm_period * (float)0.1);
	minDuty = pwm_period;

}

void SixStepCommutation::Start(){
	if(StartUp.fRampOn == false){
		Align();
	} else {
		Ramp();
	}
}

void SixStepCommutation::Run(){
	extern Timer7 timer7;
	extern Timer6 timer6;
	extern dFilter<uint16_t, 4> rpmFilter;

	if(Flags.preCommutation == true) { return; }

	if((++blankingCount) < blankingLimit) { return; }

	if(BEMFDetection() == true){

		stallCount = 0;
		Flags.bemfDetection = true;
		Flags.preCommutation = true;

		timer7.Stop();
		timer_avg = rpmFilter.Calc(timer7.GetCNT());
		timer7.ResetCNT();
		timer7.Start();
		currentRPM = timer_to_rpm/timer_avg;

		commutationTime = timer_avg << 1;
		if(commutationTime < 1){
			commutationTime = 1;
		}
		timer6.SetARR(commutationTime);
		timer6.Start();

		SpeedLoopController();

	} else {
		stallCount++;
	}
}

void SixStepCommutation::Stop(){
	extern Timer1 timer1;
	extern Timer7 timer7;
	extern NonBlockingDelay nbDelay;
	extern dFilter<uint16_t, 4> rpmFilter;

	timer1.PWMOutputsOff();
	nbDelay.Stop();
	timer7.Stop();
	rpmFilter.Reset();
}

void SixStepCommutation::Align(){

	extern Timer7 timer7;
	extern Timer1 timer1;
	extern NonBlockingDelay nbDelay;
	extern dFilter<uint16_t, 4> rpmFilter;

	if(nbDelay.GetState() == On) { return; }

	timer1.PWMOutputsOff();

	timer7.Stop();
	timer7.ResetCNT();

	Flags.trainPI 	= true;
	stallCount 		= 0;

	timer1.SetDiraction(Flags.diraction);

	// Initialize TMR7 and rpm Filter average with the value corresponding to the minimum motor speed
	timer7.SetCNT(timer_max);
	rpmFilter.Reset();
	rpmFilter.FillBuffer(timer_max);

	currentDuty = StartUp.duty;
	timer1.SetDuty(currentDuty);

	commSector = 5;
	timer1.SwitchCommSector(commSector);

	StartUp.Time.current = StartUp.Time.start;
	StartUp.Time.sector = 0;
	StartUp.state = AlignmentOn;

	// Non-blocking delay for alingment
	nbDelay.Start(StartUp.Time.alignment * timer1.Getpwm100usFactor() * 10);

	timer1.PWMOutputsOn();

}

void SixStepCommutation::Ramp(){

	extern Timer1 timer1;
	extern NonBlockingDelay nbDelay;

	if(nbDelay.GetState() == On) { return; }

	//Change commutation sector
	if((++commSector) > 5) { commSector = 0; }

	timer1.SwitchCommSector(commSector);

	CalcSector();

	nbDelay.Start(StartUp.Time.sector * timer1.Getpwm100usFactor());

}


void SixStepCommutation::CalcSector(){
	if(StartUp.Time.current < (uint32_t)(StartUp.Time.ramp*10)){

		StartUp.Time.sector = StartUp.sector_constant/StartUp.Time.current;
		if(StartUp.Time.sector <= 0) StartUp.Time.sector = 1;
		StartUp.state = RampOn;

	} else if(StartUp.Time.current < (uint32_t)(StartUp.Time.ramp*10 + StartUp.Time.sust*10)){

		StartUp.state = SustOn;

	} else {

		StartUp.state = StartUpOver;
	}

	StartUp.Time.current += StartUp.Time.sector;

	if(StartUp.Time.current > (uint32_t)4000000000) StartUp.Time.current = (uint32_t)4000000000;

}

bool SixStepCommutation::BEMFDetection(){
	if((comparatorOutputs ^ bemfFilter.XOR[commSector]) & bemfFilter.mask[commSector]){
		bemfFilter.value |= 0x01;
	}
	bemfFilter.value = bemfFilter.filter[bemfFilter.value];
	return (bemfFilter.value & 0x01);
}

void SixStepCommutation::SpeedLoopController(){
	extern Timer1 timer1;

	if(Flags.trainPI == true) { return; }

	if(currentRPM < desiredRPM){
		if(currentDuty > minDuty) currentDuty--;
	} else if(currentRPM > desiredRPM){
		if(currentDuty < maxDuty) currentDuty++;
	}

	timer1.SetDuty(currentDuty);
}

