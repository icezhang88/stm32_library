#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "key.h"  // 补充key相关头文件

// 串口接收回调函数（用户自定义处理逻辑）
void USART2_RxProcess(uint8_t *rx_buf, uint16_t rx_len)
{
    // 示例：回显接收到的数据
    USART2_SendData((uint8_t*)"Received: ", 10);
    USART2_SendData(rx_buf, rx_len);
    USART2_SendData((uint8_t*)"\r\n", 2);
	
    // 可添加自定义处理逻辑（如解析指令、控制外设等）
    if(rx_buf[0] == 'L' && rx_buf[1] == 'E' && rx_buf[2] == 'D')
    {
        toggle_led();  // 接收到LED指令，点亮LED
    }
}

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

int main(void)
{
    // 初始化外设
    init_led();
    USART2_Config();
    key_init();
    NVIC_Configuration();
	
    // 注册串口接收回调函数（核心：替代while轮询）
    USART2_RegisterRxCallback(USART2_RxProcess);

    while (1)
    {
        // 主循环仅处理非中断任务（如LED闪烁），无需轮询串口接收
//        turn_on_led();
//        Delay_ms(500);  // 延长闪烁时间，便于观察
//        turn_off_led();
//        Delay_ms(500);
    }
}