#include "Key.h"
#include "Led.h"
#include "System.h"

void key_init(void){
		GPIO_InitTypeDef GPIO_InitStructure;
	// 使能GPIOB和AFIO时钟（AFIO用于外部中断引脚映射）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);

	// 配置PB11为上拉输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  // 上拉输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 输出速率（输入模式下无实际意义，可随便设）
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//中断配置
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
