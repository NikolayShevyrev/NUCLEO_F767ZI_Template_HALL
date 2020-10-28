/*
 * uart.cpp
 *
 *  Created on: 20 окт. 2020 г.
 *      Author: Nikolay Shevyrev
 */



#include "uart.h"
#include "main.h"
#include "gpio.h"


void UART::Init(void){

	/* UART4 clock enable */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_UART4EN);

	/*
	 * UART4 GPIO Configuration
	 *
	 * PD0 -> UART4_RX
	 * PD1 -> UART4_TX
	 */
	GPIO_PortClockInit(UART4_PORT);
	GPIO_PortClockInit(UART4_PORT);

	GPIO_AFPinInit(UART4_RX_PIN, UART4_PORT, PushPull, VeryHigh, NoPull, AF8);
	GPIO_AFPinInit(UART4_TX_PIN, UART4_PORT, PushPull, VeryHigh, NoPull, AF8);

	/* Disable UART */
	CLEAR_BIT(UART4->CR1, USART_CR1_UE);

	/*
	 * USART CR1 Configuration
	 *
	 * WordLength: 8 data bits
	 * Parity control disabled
	 * Receiver is enabled
	 * Transmitter is enabled
	 * Oversampling by 16
	 */
	CLEAR_REG(UART4->CR1);
	SET_BIT(UART4->CR1, USART_CR1_RE | USART_CR1_TE );

	/*
	 * USART CR2 Configuration
	 *
	 * 1 stop bit
	 */
	CLEAR_BIT(UART4->CR2, USART_CR2_STOP);

	/*
	 * USART CR3 Configuration
	 *
	 * Hardware flow control disabled
	 * One sample bit method enable
	 */
	CLEAR_BIT(UART4->CR3, USART_CR3_RTSE | USART_CR3_CTSE);
	SET_BIT(UART4->CR3, USART_CR3_ONEBIT);

	/*
	 * UART4 Clock Configuration
	 *
	 * APB1 clock (PLCLK1) is selected as UART 4 clock
	 *
	 */
	CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_UART4SEL);
	uint32_t baud_ = 9600;
	uint32_t pclk = SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos];
	uint32_t usartdiv = (uint16_t)((pclk + (baud_/2U))/baud_);
	if ((usartdiv >= 0x10U) && (usartdiv <= 0x0000FFFFU))
	{
		WRITE_REG(UART4->BRR, usartdiv);
	}

	/*
	 * In asynchronous mode, the following bits must be kept cleared:
	 * LINEN and CLKEN bits in the USART_CR2 register,
	 * SCEN, HDSEL and IREN  bits in the USART_CR3 register
	 */
	CLEAR_BIT(UART4->CR2, (USART_CR2_LINEN | USART_CR2_CLKEN));
	CLEAR_BIT(UART4->CR3, (USART_CR3_SCEN | USART_CR3_HDSEL | USART_CR3_IREN));

	/* Enable UART */
	SET_BIT(UART4->CR1, USART_CR1_UE);

}

void UART::Receive(){
	while(!(UART4->ISR&USART_ISR_RXNE));
	received_data = UART4->RDR;
}
