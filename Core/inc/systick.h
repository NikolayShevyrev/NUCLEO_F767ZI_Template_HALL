/*
 * systick.h
 *
 *  Created on: 8 дек. 2021 г.
 *      Author: Nikolay Shevyrev
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include "main.h"

#ifdef __cplusplus
 extern "C" {
#endif


/* configure systick */
void systick_config(void);
/* delay a time in milliseconds */
void delay_ms(uint32_t count);
/* delay decrement */
void delay_decrement(void);

#ifdef __cplusplus
}
#endif


#endif /* INC_SYSTICK_H_ */
