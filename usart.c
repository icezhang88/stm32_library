#include "usart.h"

#include "stm32f10x.h"                  // Device header

void USART2_Config(void){

	GPIO_InitTypeDef GPIO_InitStruct;	
	USART_InitTypeDef USART_InitStuct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//串口tx 配置为复用推挽输出
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_2;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;//配置为浮空输入 串口接收
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	//配置UART参数
	USART_InitStuct.USART_BaudRate=BAUDRATE;
	USART_InitStuct.USART_WordLength=USART_WordLength_8b;
	USART_InitStuct.USART_StopBits=USART_Parity_No;
	USART_InitStuct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStuct.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART2,&USART_InitStuct);
	
	//开启发送和接收中断
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);//空闲中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);//接收中断
	
	//使能usart2
	USART_Cmd(USART2,ENABLE);
	
	
	
	
	
}
