/*
 * mctrl.cpp
 *
 *  Created on: 22 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "mctrl.h"
#include "timers.h"
#include "delay.h"

void MotorControl::Init(){

	extern Timer7 timer7;
	extern Timer1 timer1;
	extern NonBlockingDelay nbDelay;

	if(nbDelay.GetFlag() == true) return;

	timer7.Stop();
	timer7.ResetCNT();

	Flags.trainPI = true;
	Hall.count = 0;
	Hall.input = 0;
	Hall.newInput = 0;
	stallCount = 0;

	timer1.SetDiraction(Flags.diraction);

	// Initialize TMR7 and TMR7 average with the 0
	timer7.SetCNT(0);
	timer7.value = 0;
	timer7.average = 0;

	currentDuty = StartUp.duty;
	timer1.SetDuty(currentDuty);

	commSector = 5;
	timer1.SwitchCommSector(commSector);

	// Non-blocking delay for alingment
	nbDelay.Init(StartUp.Time.alignment * timer1.Getpwm100usFactor() * 10);

}

void MotorControl::Start(){

	extern NonBlockingDelay nbDelay;

	if(nbDelay.GetFlag() == true) return;

	if((++commSector) > 5) { commSector = 0; }


}

void MotorControl::Run(){

}

void MotorControl::Stop(){

}

void MotorControl::SpeedLoopController(){
	extern Timer1 timer1;

	if(Flags.trainPI == true) return;

	if(currentRPM < desiredRPM){
		if(currentDuty > minDuty) currentDuty--;
	} else if(currentRPM > desiredRPM){
		if(currentDuty < maxDuty) currentDuty++;
	}

	timer1.SetDuty(currentDuty);
}

