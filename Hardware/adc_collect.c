//#include "adc_collect.h"


//void ADC_Config(void){
//	
//	
//	GPIO_InitTypeDef GPIO_InitStruct;
//	ADC_InitTypeDef ADC_InitStruct;
//	//1.使能时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
//	//ADC 时钟分频
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	
//	
//	//2.配置PB0 PB1为模拟输入 AIN模式  无上下拉
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
//	GPIO_Init(GPIOB,&GPIO_InitStruct);
//	//3.配置ADC 核心参数
//	
//	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
//	ADC_InitStruct.ADC_ScanConvMode=ENABLE;
//	ADC_InitStruct.ADC_ContinuousConvMode=DISABLE;//默认单次转换
//  ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//软件触发
//	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;//数据右对齐
//	ADC_InitStruct.ADC_NbrOfChannel=2;//ADC 通道数  2个
//	
//	ADC_Init(ADC1,&ADC_InitStruct);
//	
//	//开启adc并执行校准
//	ADC_Cmd(ADC1,ENABLE);
//	ADC_ResetCalibration(ADC1);
//	while(  ADC_GetResetCalibrationStatus(ADC1));
//	//adc开始校准
//	ADC_StartCalibration(ADC1);
//	while( ADC_GetCalibrationStatus(ADC1));//等待校准完成
//	
//}


///**
// * @brief  切换ADC连续/单次模式（寄存器直接操作，兼容所有库版本）
// * @param  mode: ENABLE(连续)/DISABLE(单次)
// */
//void ADC_SetContinuousMode(u8 mode)
//{
//    if(mode == ENABLE)
//    {
//        ADC1->CR2 |= ADC_CR2_CONT; // 置位CONT位：连续模式
//    }
//    else
//    {
//        ADC1->CR2 &= ~ADC_CR2_CONT; // 清零CONT位：单次模式
//    }
//}


//void ADC_GetDualChannelValue(uint16_t *ch8_val,uint16_t *ch9_val){
//	
// // 确保为单次模式
//    ADC_SetContinuousMode(DISABLE);
//    
//    // 配置规则通道：CH8(排序1)、CH9(排序2)，采样时间239.5周期（高精度）
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
//    
//    // 软件触发ADC转换
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//    
//    // 等待所有通道转换完成（EOC标志）
//    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//    
//    // 读取转换值（按排序顺序）
//    *ch8_val = ADC_GetConversionValue(ADC1); // CH8(PB0)
//    *ch9_val = ADC_GetConversionValue(ADC1); // CH9(PB1)
//    
//    // 清除EOC标志，停止触发
//    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//	

//}



///**
// * @brief  连续采集PB0/PB1双通道多组数据（无任何非标准函数）
// */
//void ADC_ContinuousDualChannel(u16 *buf_ch8, u16 *buf_ch9, u16 len)
//{
//    u16 i;
//    
//    // 切换为连续模式
//    ADC_SetContinuousMode(ENABLE);
//    
//    // 配置通道排序
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
//    
//    // 软件触发转换
//    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//    
//    // 连续采集len次
//    for(i=0; i<len; i++)
//    {
//        // 等待单次双通道转换完成
//        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//        
//        // 读取值
//        buf_ch8[i] = ADC_GetConversionValue(ADC1);
//        buf_ch9[i] = ADC_GetConversionValue(ADC1);
//        
//        // 清空EOC标志
//        ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//    }
//    
//    // 恢复单次模式，停止触发
//    ADC_SetContinuousMode(DISABLE);
//    ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//}

///**
// * @brief  单通道多次采集取平均值（优化噪声）
// */
//u16 ADC_GetAvgValue(u8 ch, u8 times)
//{
//    u32 sum = 0;
//    ADC_SetContinuousMode(DISABLE); // 确保单次模式
//    
//    for(u8 i=0; i<times; i++)
//    {
//        ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);
//        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
//        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
//        sum += ADC_GetConversionValue(ADC1);
//        ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
//        ADC_SoftwareStartConvCmd(ADC1, DISABLE);
//    }
//    return sum / times;
//}











