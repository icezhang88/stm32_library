#ifndef __ADC_COLLECT_DMA__
#define __ADC_COLLECT_DMA__


#include "stm32f10x.h"                  // Device header

#include "stdint.h"

#define ADC_DMA_BUF_LEN  100 //缓冲区长度  存储100组双通道的数据

extern u16 ADC_DMA_Buf[2*ADC_DMA_BUF_LEN];  //双通道 CH8 CH9 交替存储

//函数生命
void ADC_Config(void);

void ADC_DMA_Config(void);

void ADC_Start_DMA_Collect(void);

u16 ADC_GetAvgValue(u8 ch,u8 times);




#endif