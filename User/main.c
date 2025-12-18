#include "stm32f10x.h" // Device header
#include "LED.h"
#include <stm32f10x_rcc.h>

  

 
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// 设置NVIC优先级分组（整个工程只需调用一次，需与其他中断保持一致）
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// 配置EXTI10_15中断通道（注意：EXTI10~EXTI15共享一个中断通道EXTI15_10_IRQn）
	// 原因：STM32F1的EXTI中断通道划分：EXTI0~EXTI4为独立通道，EXTI5~9共享EXTI9_5_IRQn，EXTI10~15共享EXTI15_10_IRQn
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	  // EXTI11属于EXTI10~15组，对应此通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级0（数值越小优先级越高）
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 响应优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // 使能该中断通道
	NVIC_Init(&NVIC_InitStructure);
}

 

 

int main(void)
{

	init_led();

	GPIO_Configuration();
	EXTI_Configuration();
	NVIC_Configuration();

	while (1)
	{
		turn_on_led();
		Delay_ms(50);
		turn_off_led();
		Delay_ms(50);
	}
}
