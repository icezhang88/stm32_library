#ifndef __USART_H__
#define __USART_H__

#include "stdint.h"
#include "stdbool.h"

#define USART2_BUFFER_SIZE 128
#define BAUDRATE 115200

uint8_t USART2_RxBuffer[USART2_BUFFER_SIZE];
uint16_t USART2_RxLen=0;

bool USART2_RxFlag=false;

void USART2_Config(void);


void USART2_SendData(uint8_t *pData,uint16_t len);



#endif