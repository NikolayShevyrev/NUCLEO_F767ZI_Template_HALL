/*
 * uart.h
 *
 *  Created on: 20 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "uart.h"

class UART {
private:
public:
	uint16_t received_data;
public:
	//UART();

	void Init();
	void Receive();
	void Send();

};


#endif /* INC_UART_H_ */
