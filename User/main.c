#include "stm32f10x.h"
#include "LED.h"
#include "usart.h"
#include "key.h"  // ????key???????
#include "adc_collect_dma.h"
#include "string.h"
#include "FreeRTOS.h"
#include "Task.h"

void USART2_RxProcess(uint8_t *rx_buf, uint16_t rx_len)
{
    // ???????????????????
    USART2_SendData((uint8_t*)"Received: ", 10);
    USART2_SendData(rx_buf, rx_len);
    USART2_SendData((uint8_t*)"\r\n", 2);
	
    // ??????????崦?????????????????????????
    if(rx_buf[0] == 'L' && rx_buf[1] == 'E' && rx_buf[2] == 'D')
    {
        toggle_led();  // ?????LED???????LED
    }
}
 u16 avg_pb0, avg_pb1;
    float volt_pb0, volt_pb1;
char send_buf[64];
TaskHandle_t adc_handler;
void adc_task(){
	
	while(1){
			 USART2_SendData("system start",strlen("system start"));
			 vTaskDelay(pdMS_TO_TICKS(200));
		//			      // 从DMA缓冲区中取10次平均值（无需CPU参与采集，直接读取缓冲区）
        avg_pb0 = ADC_GetAvgValue(ADC_Channel_8, 10);
        avg_pb1 = ADC_GetAvgValue(ADC_Channel_9, 10);

        // 转换为电压值
        volt_pb0 = (float)avg_pb0 * 3.3f / 4095.0f;
        volt_pb1 = (float)avg_pb1 * 3.3f / 4095.0f;

        // 格式化并发送数据
        sprintf(send_buf, "DMA采集：PB0=%d(%.2fV)  PB1=%d(%.2fV)\r\n",
                avg_pb0, volt_pb0, avg_pb1, volt_pb1);
        USART2_SendData((uint8_t*)send_buf, strlen(send_buf));
		
	}
	
}

int main(void)
{
	
	 
    // ?????????
    init_led();
    USART2_Config();
    key_init();
    NVIC_Configuration();
	   
	
	ADC_Config();      // ADC基础配置
    ADC_DMA_Config();  // DMA配置
    ADC_Start_DMA_Collect(); // 启动ADC+DMA自动采集
    // ????????????????????????while?????
    USART2_RegisterRxCallback(USART2_RxProcess);
		 USART2_SendData("system start",strlen("system start"));
	
		 u16 adc_pb0, adc_pb1;               // PB0(CH8)/PB1(CH9)????
    u16 buf_pb0[50], buf_pb1[50];       // ?????????????
		 
	
		xTaskCreate(adc_task,"adc_task",2*1024,NULL,1,&adc_handler);
		vTaskStartScheduler();
	
    while (1)
    {
			 
//			      // 从DMA缓冲区中取10次平均值（无需CPU参与采集，直接读取缓冲区）
//        avg_pb0 = ADC_GetAvgValue(ADC_Channel_8, 10);
//        avg_pb1 = ADC_GetAvgValue(ADC_Channel_9, 10);

//        // 转换为电压值
//        volt_pb0 = (float)avg_pb0 * 3.3f / 4095.0f;
//        volt_pb1 = (float)avg_pb1 * 3.3f / 4095.0f;

//        // 格式化并发送数据
//        sprintf(send_buf, "DMA采集：PB0=%d(%.2fV)  PB1=%d(%.2fV)\r\n",
//                avg_pb0, volt_pb0, avg_pb1, volt_pb1);
//        USART2_SendData((uint8_t*)send_buf, strlen(send_buf));
//			
//				Delay_ms(300);
    }
}