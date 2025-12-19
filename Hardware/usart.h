#ifndef __USART_H__
#define __USART_H__

#include "stdint.h"
#include "stdbool.h"

#define USART2_BUFFER_SIZE 128
#define BAUDRATE 115200

extern uint8_t USART2_RxBuffer[USART2_BUFFER_SIZE];
extern uint16_t USART2_RxLen;

// 定义串口接收回调函数类型
typedef void (*USART2_RxCallback_t)(uint8_t *rx_buf, uint16_t rx_len);

// 注册接收回调函数
void USART2_RegisterRxCallback(USART2_RxCallback_t cb);

void USART2_Config(void);
void USART2_SendData(uint8_t *pData, uint16_t len);

#endif