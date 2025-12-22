
#include "System.h"
#include "stm32f10x.h"                  // Device header


//中断配置
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // 设置NVIC优先级分组（整个工程只需调用一次）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // 配置EXTI15_10中断（保留原有按键中断配置）
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    // 配置USART2中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  // 抢占优先级更高
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
//延迟
void Delay_ms(uint32_t ms)
{
	uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 7200; j++)
			;
	}
}

