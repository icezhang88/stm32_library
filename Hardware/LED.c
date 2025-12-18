#include "LED.h"
#include "stm32f10x.h"

void init_led(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void turn_on_led(void)
{

    GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void turn_off_led(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_13);
}

// 新增：翻转LED电平（标准库函数读写，逻辑直观）
void toggle_led(void)
{
    // 读取PC13当前输出电平
    if (GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == Bit_RESET)
    {
        // 当前是低电平，设置为高电平
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
    else
    {
        // 当前是高电平，重置为低电平
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    }
}