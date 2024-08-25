#include "bsp.h"

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
   uint32_t temp;
	if(huart->Instance==USART2){

		if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_ORE)!=RESET){

             __HAL_UART_CLEAR_OREFLAG(&huart2);
		
			UART_Start_Receive_IT(&huart2,wifi_usart_data.wifi_inputBuf,1);

		}
		__HAL_UNLOCK(&huart2);
		   
       
          temp = USART2->RDR;
		UART_Start_Receive_IT(&huart2,wifi_usart_data.wifi_inputBuf,1);


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

    static uint8_t tm0;
    if(htim->Instance==TIM17){
		
	   tm0 ++ ;
       run_t.gTimer_senddata_panel++;
	 if(tm0 > 99){//100ms *10 = 1000ms =1s
        tm0 =0;
	
        wifi_t.gTimer_get_beijing_time++;

	    run_t.gTimer_publish_dht11++;
	    run_t.gTimer_app_power_on++;
	    run_t.gTimer_read_beijing_time++;
	   run_t.gTimer_auto_detected_net_link_state++;

	   run_t.gTimer_usart_error++;
	   wifi_t.gTimer_reconnect_wifi++;
	   

	   run_t.gTimer_to_publish_updata++;
	   run_t.gTimer_send_dit++;
	   wifi_t.gTimer_reconnect_wifi_order++;
	   wifi_t.gTimer_power_off++;
	   wifi_t.gTimer_subscriber_send ++;
	   run_t.gTimer_continuce_works_time++;
	   run_t.gTimer_fan_adc_times++;
	   run_t.gTimer_ptc_adc_times++;

	  run_t.gTimer_usart2_error++;
	  run_t.gTimer_linking_tencen_counter++;
	  run_t.gFan_counter++;
	  }
 	}
 }
