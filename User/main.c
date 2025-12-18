#include "stm32f10x.h" // Device header
#include "LED.h"
#include <stm32f10x_rcc.h>

void GPIO_Configuration(void); // GPIO配置（PB11上拉输入）
void EXTI_Configuration(void); // 外部中断配置（PB11映射到EXTI11）
void NVIC_Configuration(void);
void Delay_ms(uint32_t ms);

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	// 使能GPIOB和AFIO时钟（AFIO用于外部中断引脚映射）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// 配置PB11为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  // 上拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出速率（输入模式下无实际意义，可随便设）
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	// 配置PB11为EXTI11的中断源（AFIO引脚映射）
	// 注意：GPIO_PinSource11对应引脚11，与EXTI11线一一对应
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);

	// 配置EXTI11线参数
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;				// PB11对应EXTI11线（引脚号=EXTI线号）
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		// 中断模式（非事件模式）
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发（按键按下时电平从高到低）
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				// 使能EXTI11线
	EXTI_Init(&EXTI_InitStructure);
}
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

void EXTI15_10_IRQHandler(void)
{
	// 先检查EXTI11的中断标志位，确认是PB11触发的中断
	if (EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		// 按键消抖：延时20ms后再次检测引脚电平，排除机械抖动
		Delay_ms(20);
		if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == RESET) // 确认按键确实按下（电平为低）
		{
			// *************************
			// 此处添加按键按下的处理逻辑
			// 示例：翻转LED、计数、发送数据等
			// *************************
			toggle_led();
		}

		// 清除EXTI11的中断挂起位（必须操作，否则会反复触发中断）
		EXTI_ClearITPendingBit(EXTI_Line11);
	}
}

void Delay_ms(uint32_t ms)
{
	uint32_t i, j;
	for (i = 0; i < ms; i++)
	{
		for (j = 0; j < 7200; j++)
			;
	}
}

int main(void)
{

	init_led();

	GPIO_Configuration();
	EXTI_Configuration();
	NVIC_Configuration();

	while (1)
	{
	}
}
