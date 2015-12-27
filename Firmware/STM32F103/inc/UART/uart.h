#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "utils/FIFO.h"
#include "uart.h"

/////////////////////////////////////////////////////////////////////////////////
#define USARTx USART3
#define ENABLE_CLOCKS() RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); \
                        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
#define GPIO_PORT (GPIOB)
#define GPIO_PIN_TX GPIO_Pin_10
#define GPIO_PIN_RX GPIO_Pin_11
#define IRQ_CHANNEL USART3_IRQn
#define IRQ_HANDLER USART3_IRQHandler


#define CLASS_NAME USART3_Dev_t
#define DEV_NAME USART3_Dev

class CLASS_NAME {
public:
	FIFO_t<char, 50> Tx_FIFO;

	volatile uint8_t rx_overrun = 0;
	volatile uint8_t uart_transmit_restart = 1;

	void init(uint32_t baudrate) {
		GPIO_InitTypeDef  GPIO_InitStructure;
		USART_InitTypeDef USART_InitStruct;
		NVIC_InitTypeDef NVIC_InitStructure;

		ENABLE_CLOCKS();

		/* Configure USART pins:  Rx and Tx ----------------------------*/
		GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_TX;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin =  GPIO_PIN_RX;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(GPIO_PORT, &GPIO_InitStructure);

		USART_InitStruct.USART_BaudRate = baudrate;
		USART_InitStruct.USART_WordLength = USART_WordLength_8b;
		USART_InitStruct.USART_StopBits = USART_StopBits_1;
		USART_InitStruct.USART_Parity = USART_Parity_No;
		USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(USARTx, &USART_InitStruct);


		NVIC_InitStructure.NVIC_IRQChannel = IRQ_CHANNEL;		 // we want to configure the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // this sets the priority group of the USART1 interrupts
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
		NVIC_Init(&NVIC_InitStructure);

		USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
		USART_ITConfig(USARTx, USART_IT_TXE, DISABLE); // enable the USART1 transmit empty interrupt
		uart_transmit_restart=1;

		USART_Cmd(USARTx, ENABLE);
	}
//#undef putc
	void put_char(char data) {
		Tx_FIFO.push(data);
		if(uart_transmit_restart) {
			uart_transmit_restart = 0;
			USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
		}
	}
	void puts(const char *string) {
		while(*string) {
			this->put_char(*string);
			string++;
		}
	}
};
static CLASS_NAME DEV_NAME;

#include "CLI.h"
extern CLI PCConn;

bool possible_char(char x) {
	if( (x >= 32 && x <= 126) ) {
		return true;
	}
	return false;
}

GPIO_t UARTLED(GPIOB, GPIO_Pin_15);

extern "C" {
void USART3_IRQHandler(void){
	UARTLED.on();
	if(USART_GetITStatus(USARTx, USART_IT_RXNE)) {
		//receive interrupt
		static char buf[100];
		static uint8_t buf_iter = 0;
		char now = USART_ReceiveData(USARTx);
		if( possible_char(now) ) {
			buf[buf_iter++] = now;
		} else {
			buf[buf_iter] = 0;
			PCConn.parse_line(buf);
			buf_iter = 0;
		}
		USART_ClearITPendingBit(USARTx, USART_IT_RXNE);
	}
	if(USART_GetITStatus(USARTx, USART_IT_TXE)) {
		//transmit interrupt
		if( DEV_NAME.Tx_FIFO.size() > 0 ) {
			USART_SendData(USARTx, DEV_NAME.Tx_FIFO.pop());
		} else {
			DEV_NAME.uart_transmit_restart = 1;
			USART_ITConfig(USARTx, USART_IT_TXE, DISABLE);
		}
		USART_ClearITPendingBit(USARTx, USART_IT_TXE);
	}
	UARTLED.off();
}
}

extern "C" {
int __io_putchar(int ch) {
	DEV_NAME.put_char((char)ch);
}
}

#endif//__UART_H
