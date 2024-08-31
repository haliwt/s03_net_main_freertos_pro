#include "bsp.h"

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
   uint32_t temp;
	if(huart->Instance==USART2){

		if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_ORE)!=RESET){

             __HAL_UART_CLEAR_OREFLAG(&huart2);
		
			UART_Start_Receive_IT(&huart2,wifi_rx_inputBuf,1);

		}
		__HAL_UNLOCK(&huart2);
		   
       
          temp = USART2->RDR;
		UART_Start_Receive_IT(&huart2,wifi_rx_inputBuf,1);


	}
	if(huart->Instance==USART1){
	
		if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE)!=RESET){

		__HAL_UART_CLEAR_OREFLAG(&huart1);
		UART_Start_Receive_IT(&huart1,inputBuf,1);

		}
		__HAL_UNLOCK(&huart1);
		//  temp = USART1 ->ISR;
		temp = USART1->RDR;
		UART_Start_Receive_IT(&huart1,inputBuf,1);
	
		}




}
/********************************************************************************
	**
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    static uint16_t tm0;
    static uint8_t tm1;
    if(htim->Instance==TIM17){
		
	   tm0 ++ ;
     
	 if(tm0 > 999){//1ms *1000 = 1000ms =1s
        tm0 =0;
	    gctl_t.gTimer_senddata_panel++;
        wifi_t.gTimer_get_beijing_time++;

	    gctl_t.gTimer_publish_dht11++;
	    gctl_t.gTimer_app_power_on++;
	    gctl_t.gTimer_read_beijing_time++;
	   gctl_t.gTimer_auto_detected_net_link_state++;

	   gctl_t.gTimer_usart_error++;
	   wifi_t.gTimer_reconnect_wifi++;
	   

	   gctl_t.gTimer_to_publish_updata++;
	   gctl_t.gTImer_send_data_to_disp++;
	   wifi_t.gTimer_reconnect_wifi_order++;
	   wifi_t.gTimer_power_off++;
	   wifi_t.gTimer_subscriber_send ++;
	   gctl_t.gTimer_continuce_works_time++;
	   gctl_t.gTimer_fan_adc_times++;
	   gctl_t.gTimer_ptc_adc_times++;

	  gctl_t.gTimer_usart2_error++;
	  gctl_t.gTimer_linking_tencen_counter++;
	  gctl_t.gFan_counter++;
	  }
 	}
 }
