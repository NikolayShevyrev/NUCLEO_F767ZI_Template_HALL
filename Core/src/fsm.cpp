/*
 * fsm.cpp
 *
 *  Created on: 22 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#include "fsm.h"

SM_States currentState = StoppedState;

void RunFSM(){
	switch(currentState){
		case StoppedState:

			break;
		case StartingState:

			break;
		case RunningState:

			break;
		case StoppingState:

			break;
		case FaultState:

			break;
		default:
			break;
	}
}

