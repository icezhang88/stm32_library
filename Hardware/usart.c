#include "usart.h"
#include "stm32f10x.h"                  // Device header
#include "stdlib.h"
uint8_t USART2_RxBuffer[USART2_BUFFER_SIZE];
uint16_t USART2_RxLen = 0;

// 静态回调函数指针（默认空）
static USART2_RxCallback_t USART2_RxCallback = NULL;

// 注册回调函数
void USART2_RegisterRxCallback(USART2_RxCallback_t cb)
{
    USART2_RxCallback = cb;
}

void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    USART_InitTypeDef USART_InitStuct;
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
    // 配置TX引脚 PA2 复用推挽输出
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
    // 配置RX引脚 PA3 浮空输入
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
	
    // 配置UART参数
    USART_InitStuct.USART_BaudRate = BAUDRATE;
    USART_InitStuct.USART_WordLength = USART_WordLength_8b;
    USART_InitStuct.USART_StopBits = USART_StopBits_1;
    USART_InitStuct.USART_Parity = USART_Parity_No;
    USART_InitStuct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStuct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStuct);
	
    // 开启中断
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  // 空闲中断（接收完成）
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  // 接收非空中断（字节接收）
	
    // 使能USART2
    USART_Cmd(USART2, ENABLE);
}

void USART2_SendData(uint8_t *pData, uint16_t len)
{
    uint16_t i;
    for(i = 0; i < len; i++)
    {
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
        USART_SendData(USART2, pData[i]);
    }
    while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
}

void USART2_IRQHandler(void)
{
    uint8_t temp_data;
    uint32_t temp;
	
    // 接收非空中断（单个字节接收）
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        temp_data = USART_ReceiveData(USART2);
        // 防止缓冲区溢出
        if(USART2_RxLen < USART2_BUFFER_SIZE)
        {
            USART2_RxBuffer[USART2_RxLen++] = temp_data;
        }
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
	
    // 空闲中断（接收完成，触发回调）
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        // 清除空闲中断标志（必须读SR+DR）
        temp = USART2->SR;
        temp = USART2->DR;
        (void)temp;  // 防止编译器警告
		
        // 若注册了回调函数，则调用
        if(USART2_RxCallback != NULL && USART2_RxLen > 0)
        {
            USART2_RxCallback(USART2_RxBuffer, USART2_RxLen);
            USART2_RxLen = 0;  // 清空接收长度，准备下次接收
        }
    }
}