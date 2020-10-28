/*
 * delay.h
 *
 *  Created on: 16 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

/* Functions prototypes ---------------------------------------------*/
void DelayUS(uint32_t us);
void DelayMS(uint32_t ms);

class NonBlockingDelay {
private:
	uint16_t limit;
	uint16_t counter;
	bool flag;
public:
	NonBlockingDelay() :
		limit(0), counter(0), flag(false) {
	}

	void Init(uint16_t input){
		limit = input;
		counter = 0;
		flag = true;
	}

	void Tick(){
		if((++counter) >= limit){
			flag = false;
			limit = 0;
			counter = 0;
		}
	}

	bool GetFlag(){
		return flag;
	}

};

#endif /* INC_DELAY_H_ */
