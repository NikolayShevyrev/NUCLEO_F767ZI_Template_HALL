/*
 * mctrl.h
 *
 *  Created on: 22 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_MCTRL_H_
#define INC_MCTRL_H_

#include "main.h"

class MotorStartUp {
public:
	struct {
		uint16_t alignment;
		uint16_t start;
		uint16_t ramp;
		uint16_t sust;
		uint32_t current;
		uint32_t sector;
	} Time;

	uint16_t duty;
	uint16_t rpm;
	uint32_t sec_C;

};

class MotorControl {
private:
	MotorStartUp StartUp;

	struct {
		bool trainPI 	= true;
		bool startUp 	= false;
		bool runMotor	= false;
		bool diraction	= true;
	} Flags;

	struct {
		uint8_t input;
		uint8_t newInput;
		uint8_t count;
		uint8_t limit = 6;
	} Hall;

	uint8_t commSector;

	uint8_t stallCount;

	uint16_t currentRPM;
	uint16_t desiredRPM;

	uint16_t currentDuty;
	uint16_t maxDuty;
	uint16_t minDuty;

public:
	MotorControl();

	void Init();

	void Start();

	void Run();

	void Stop();

private:
    void SpeedLoopController();

};



#endif /* INC_MCTRL_H_ */
