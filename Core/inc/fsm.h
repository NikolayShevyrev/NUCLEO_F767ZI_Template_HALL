/*
 * fsm.h
 *
 *  Created on: 22 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_FSM_H_
#define INC_FSM_H_

#include "main.h"

enum SM_States {
	StoppedState,
	StartingState,
	RunningState,
	StoppingState,
	FaultState
};


void RunFSM(void);


#endif /* INC_FSM_H_ */
