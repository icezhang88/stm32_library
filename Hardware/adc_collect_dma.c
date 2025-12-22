#include "adc_collect_dma.h"

u16 ADC_DMA_Buf[2*ADC_DMA_BUF_LEN]={0};

void ADC_Config(void){
	
	
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef ADC_InitSturct;
	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	
	ADC_InitSturct.ADC_Mode=ADC_Mode_Independent;
  ADC_InitSturct.ADC_ScanConvMode=ENABLE;
	ADC_InitSturct.ADC_ContinuousConvMode=ENABLE;
	ADC_InitSturct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitSturct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitSturct.ADC_NbrOfChannel=2;//双通道 ch9 ch9
	
	ADC_Init(ADC1,&ADC_InitSturct);
	
	
	// 关键补充1：配置CH8和CH9的排序+采样时间（必须步骤）
	// CH8（PB0）：排序1，采样时间239.5周期（高精度）
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
	// CH9（PB1）：排序2，采样时间239.5周期
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
	
	// 关键补充2：使能ADC1
	ADC_Cmd(ADC1, ENABLE);
	
	// 关键补充3：ADC校准（上电必做）
	ADC_ResetCalibration(ADC1); // 复位校准
	while(ADC_GetResetCalibrationStatus(ADC1)); // 等待复位校准完成
	ADC_StartCalibration(ADC1); // 启动校准
	while(ADC_GetCalibrationStatus(ADC1)); // 等待校准完成
 
}

void ADC_DMA_Config(void){

	   DMA_InitTypeDef DMA_InitStruct;

    // 1. 使能DMA1时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    // 2. 配置DMA1_Channel1（ADC1对应DMA通道为Channel1）
    DMA_DeInit(DMA1_Channel1); // 复位DMA通道
    DMA_InitStruct.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; // 外设地址：ADC1数据寄存器
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)ADC_DMA_Buf; // 内存地址：DMA缓冲区
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC; // 数据方向：外设→内存
    DMA_InitStruct.DMA_BufferSize = 2 * ADC_DMA_BUF_LEN; // 缓冲区大小（双通道×采集组数）
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable; // 外设地址不递增（固定ADC1->DR）
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable; // 内存地址递增（依次存入缓冲区）
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据宽度：16位
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; // 内存数据宽度：16位
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; // 循环模式：缓冲区存满后自动从头开始
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium; // 中等优先级
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable; // 禁用内存到内存模式
    DMA_Init(DMA1_Channel1, &DMA_InitStruct);

    // 3. 使能DMA1_Channel1
    DMA_Cmd(DMA1_Channel1, ENABLE);
	
}




void ADC_Start_DMA_Collect(void){
	
	 // 1. 使能ADC1的DMA请求（核心：允许ADC触发DMA搬运数据）
    ADC_DMACmd(ADC1, ENABLE);

    // 2. 软件触发ADC开始连续转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}


/**
 * @brief  单通道多次采集取平均值（兼容原有逻辑，可基于DMA缓冲区计算）
 */
u16 ADC_GetAvgValue(u8 ch, u8 times)
{
    u32 sum = 0;
    u16 i, start_idx;

    // 确定通道对应的缓冲区起始索引：CH8对应偶数索引，CH9对应奇数索引
    if(ch == ADC_Channel_8)
        start_idx = 0;
    else if(ch == ADC_Channel_9)
        start_idx = 1;
    else
        return 0;

    // 从DMA缓冲区中取前times个对应通道的值求平均
    for(i=0; i<times; i++)
    {
        sum += ADC_DMA_Buf[start_idx + 2*i]; // 步长2：跳过另一个通道的数据
    }

    return sum / times;
}



















