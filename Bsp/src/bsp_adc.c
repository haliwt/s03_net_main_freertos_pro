#include "bsp.h"
#include "adc.h"


/* USER CODE BEGIN 0 */

#define ADC_CHANNEL_NUMBER               2

static uint16_t Get_Adc_Channel_0(void) ;
static uint16_t Get_Adc_Channel_1(void) ; 
static uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);


uint16_t fan_detect_voltage;
uint16_t motor_detect_voltage;
__IO uint32_t ADC_ConvertedValueLocal[ADC_CHANNEL_NUMBER];

uint32_t ADC_ConvertedValue[ADC_CHANNEL_NUMBER];



/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Channel_0(void)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
   

	ADC1_ChanConf.Channel=ADC_CHANNEL_0;                                   //ĂÂ¨ÂľĂ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_1;                 //设置规则组的，ADC1转换排名                          
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //Â˛ĂĂĂšĂÂąÂźĂ¤               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        ///* 通道配置 */
	
   // HAL_ADC_Start(&hadc1);                               //start ADC transmit

    HAL_ADC_Start_DMA(&hadc1,&ADC_ConvertedValue[0],1);  //启动ADC DMA 传输
	
    HAL_ADC_PollForConversion(&hadc1,10);                /* 轮询转换 */
 
	return (uint16_t)HAL_ADC_GetValue(&hadc1);	       /* 返回最近一次ADC1规则组的转换结果 */
}

/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Channel_1(void)   
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;
   
    //配置采样通道
	ADC1_ChanConf.Channel=ADC_CHANNEL_1;        //ĂÂ¨ÂľĂ
    ADC1_ChanConf.Rank= ADC_REGULAR_RANK_2;   //设置规则组的，ADC1转换排名                          
    ADC1_ChanConf.SamplingTime=ADC_SAMPLETIME_1CYCLE_5;//ADC_SAMPLETIME_239CYCLES_5;      //Â˛ĂĂĂšĂÂąÂźĂ¤               


	HAL_ADC_ConfigChannel(&hadc1,&ADC1_ChanConf);        //ĂÂ¨ÂľĂĂĂ¤ĂĂ
	
   // HAL_ADC_Start(&hadc1);                             /* 开启ADC */

    
    HAL_ADC_Start_DMA(&hadc1,&ADC_ConvertedValue[1],2);  //启动ADC DMA 传输
	
    HAL_ADC_PollForConversion(&hadc1,10);              /* 轮询转换 */
 
	//return (uint16_t)HAL_ADC_GetValue(&hadc1);	        	  /* 返回最近一次ADC1规则组的转换结果 */
}



/* USER CODE BEGIN 1 */
/*****************************************************************
*
	*Function Name: static uint16_t Get_Adc(uint32_t ch)  
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?
	*Return Ref: No
	*
	*
*****************************************************************/
static uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)
{

    uint32_t temp_val=0;
	uint8_t t;

   if(ch == 0){

   
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc_Channel_0()  ; 
		//osDelay(2);
	}
	return temp_val/times;
    }
    else{

        for(t=0;t<times;t++)
        {
            temp_val+=Get_Adc_Channel_1()  ; 
           // osDelay(2);
        }
        return temp_val/times;
    }
} 

//uint16_t Get_Adc_Voltage_Value(uint8_t times) 
//{
//    // 定义一个变量，用于存储计算出的电压值
//    uint16_t temp_voltage_value;
//    // 定义一个变量，用于存储读取到的模拟电压的十六进制数
//    uint16_t temp_hex_value;
//    // 读取times次，计算平均值
//    temp_hex_value = Get_Adc_Hex_Average(times);
//    // 使用公式计算出实际的电压值
//    temp_voltage_value = (temp_hex_value * 3300) / 4096; // amplification -> 1000 multiple
//    // 返回实际的电压值
//    return temp_voltage_value;
//}

/*****************************************************************
	*
	*Function Name: void Get_Fan_Adc_Fun(uint8_t channel,uint8_t times)
	*Function ADC input channel be selected "which one channe"
	*Input Ref: which one ? AC_Channel_?, hexadecimal of average
	*Return Ref: No
	*
	*
*****************************************************************/
void Get_Fan_ADC_Fun(uint8_t channel,uint8_t times)
{
	volatile uint16_t adc_fan_hex;
	static uint8_t detect_error_times;
	 ADC_ChannelConfTypeDef sConfig = {0};
   // if(gpro_t.works_time_out_flag == 0){
   // adc_fan_hex = Get_Adc_Average(channel,times);
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
   
    HAL_ADC_Start_DMA(&hadc1,&ADC_ConvertedValue[0],1);  //启动ADC DMA 传输

    fan_detect_voltage =(uint16_t)(((ADC_ConvertedValue[0]& 0xfff) * 33000)/4096); //amplification 1000 ,3.111V -> 3111
	//HAL_Delay(5);

	if( fan_detect_voltage >0 ){
           detect_error_times=0;
		   #if DEBUG
             printf("adc= %d",run_t.fan_detect_voltage);
		   #endif 
          
    }
	else{

	          
			   if(detect_error_times >0){
			   		detect_error_times=0;
		       
			       buzzer_sound();//Buzzer_KeySound();
			        osDelay(100);
				   buzzer_sound();//Buzzer_KeySound();
			        osDelay(100);
				   buzzer_sound();//Buzzer_KeySound();
			        osDelay(100);
				   buzzer_sound();//Buzzer_KeySound();
			      osDelay(100);
				 
				  

			   	}
	           detect_error_times++;

     }
   /// }
}

void Get_Ptc_ADC_Fun(uint8_t channel,uint8_t times)
{
  //  volatile uint16_t adc_motor_hex;
	static uint8_t detect_error_times;
	 ADC_ChannelConfTypeDef sConfig = {0};
   // if(gpro_t.works_time_out_flag == 0){
    //adc_motor_hx = Get_Adc_Average(channel,times);

  sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = ADC_REGULAR_RANK_2;
  sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
 // ADC_ConvertedValue[1]=0;
    HAL_ADC_Start_DMA(&hadc1,&ADC_ConvertedValue[1],1);  //启动ADC DMA 传输

    motor_detect_voltage=(uint16_t)(((ADC_ConvertedValue[1]    & 0xfff)* 33000)/4096); //amplification 1000 ,3.111V -> 3111
	//HAL_Delay(5);

	if(motor_detect_voltage >0 ){
           detect_error_times=0;
		   #if DEBUG
             printf("adc= %d",run_t.fan_detect_voltage);
		   #endif 
          
    }
	else{

	          
			   if(detect_error_times >0){
			   		detect_error_times=0;
		           buzzer_sound();//Buzzer_KeySound();
			       osDelay(50);
				   buzzer_sound();//Buzzer_KeySound();
			       osDelay(50);
				   buzzer_sound();//Buzzer_KeySound();
			       osDelay(50);
				   buzzer_sound();//Buzzer_KeySound();
			       osDelay(50);
				   
				  

			   	}
	           detect_error_times++;

     }
   /// }


}




