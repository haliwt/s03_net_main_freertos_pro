#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp.h"


uint8_t TCMQTTRCVPUB[40];

uint8_t rx_app_timer_power_on_flag;


 
//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
void Receive_Data_FromCloud_Data(int type, char *str)
{
   uint8_t   iNameLen = 0;
    char  *p_cName = 0, *p_cPos = str;

       net_t.rx_data_len=strlen(str);

      if (type == JSOBJECT) {
        /* Get Key */
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        p_cName = ++p_cPos;
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        iNameLen = p_cPos - p_cName;
      net_t.rx_data_name_len=iNameLen;

        /* Get Value */
        p_cPos = strchr(p_cPos, ':');
      
      
       }

   
   }

 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
   uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
          gctl_t.randomName[0]=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, gctl_t.randomName[0]);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000); 
         free(device_massage);
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{
     
    switch(net_t.rx_data_state)
      {
      case 0: //#0

	  	if((wifi_rx_inputBuf[0] == 'p') ||wifi_rx_inputBuf[0]=='E' || wifi_rx_inputBuf[0] =='T') //hex :54 - "T" -fixed
            net_t.rx_data_state=1; //=1
          else{
               net_t.rx_counter=0;
               net_t.rx_data_state=0; 
            }
        break;

      case 1:
      
         if((wifi_rx_inputBuf[0] == 'a')  ||wifi_rx_inputBuf[0]=='R' || wifi_rx_inputBuf[0] =='C')//hex :54 - "T" -fixed
            net_t.rx_data_state=2; //=1
          else{
               net_t.rx_counter=0;
               net_t.rx_data_state=1;
            }
            
         break;
      case 2: //#1
             if((wifi_rx_inputBuf[0] == 'r')||wifi_rx_inputBuf[0]=='R'|| wifi_rx_inputBuf[0] =='M')  //hex :4B - "K" -fixed
            net_t.rx_data_state=3; //=1
         else{
            net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
         break;
            
        case 3:
            if((wifi_rx_inputBuf[0] == 'a')||wifi_rx_inputBuf[0]=='o'|| wifi_rx_inputBuf[0] =='Q')    //hex :4B - "K" -fixed
            net_t.rx_data_state=4; //=1
         else{
           net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
        
        break;
        
        case 4:
            if((wifi_rx_inputBuf[0] == 'm')  ||wifi_rx_inputBuf[0]=='R'|| wifi_rx_inputBuf[0] =='T')  //hex :4B - "K" -fixed
            net_t.rx_data_state=5; //=1
         else{
            net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
        
        break;

      case 5:
       if((wifi_rx_inputBuf[0] == 's') ||wifi_rx_inputBuf[0]==':'|| wifi_rx_inputBuf[0] =='T')   //hex :4B - "K" -fixed
         net_t.rx_data_state=6; //=1
         else{
           net_t.rx_data_state=0;
            net_t.rx_counter=0;
         }
            
      break;

      
      case 6:
       if((wifi_rx_inputBuf[0] == '"')||wifi_rx_inputBuf[0]=='2' || wifi_rx_inputBuf[0] =='S')    //hex :4B - "K" -fixed
         net_t.rx_data_state=7; //=1
         else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }
            
      break;

      case 7:
       if((wifi_rx_inputBuf[0] == ':')||wifi_rx_inputBuf[0]=='0' ||wifi_rx_inputBuf[0]=='T' ){  //hex :4B - "K" -fixed
         net_t.rx_data_state=8; //=1
    	}
		else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }
           
      break;

       case 8:
       if((wifi_rx_inputBuf[0] == '{') ||wifi_rx_inputBuf[0]=='8' ||wifi_rx_inputBuf[0]=='A' ){ //hex :4B - "K" -fixed
         if(wifi_rx_inputBuf[0]=='8')  net_t.rx_data_state =11;
         else if(wifi_rx_inputBuf[0]=='A')  net_t.rx_data_state =12;
         else
             net_t.rx_data_state =9; //=1

        }
         else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }

		
            
      break;


      case 9:

       if(net_t.rx_data_success ==0){

            gpro_t.wifi_rx_data_array[net_t.rx_counter] = wifi_rx_inputBuf[0];
            net_t.rx_counter++ ;

            if(wifi_rx_inputBuf[0]=='}' || wifi_rx_inputBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
                net_t.rx_data_success=1;
                net_t.rx_data_state=0;
                wifi_t.received_data_from_tencent_cloud = net_t.rx_counter;
                net_t.rx_counter=0;


            }


       }
       else{
            net_t.rx_data_success=0;
            net_t.rx_data_state =0;
            net_t.rx_counter=0;
            wifi_t.received_data_from_tencent_cloud =0;


       }

     break;

     
      case 11:
		 net_t.wifi_link_net_success =0; //wifi_t.esp8266_login_cloud_success =0;
         gpro_t.get_beijing_time_success = 0;
         net_t.rx_data_state =0;
         net_t.rx_counter=0;

      break;

      case 12:
		if(wifi_rx_inputBuf[0]== 'T'){   //hex :4B - "K" -fixed
         net_t.rx_data_state=13; //=1
        }
         else{
            net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }

      break;

	  case 13:

       if(wifi_rx_inputBuf[0]== 'E'){   //hex :4B - "K" -fixed
          net_t.rx_data_state=14; //=1
        }
        else{
             net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }

      break;

	  case 14:
		if(wifi_rx_inputBuf[0]== ':'){   //hex :4B - "K" -fixed
          net_t.rx_data_state=15; //=1
         }
         else{
             net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }

      break;

	  case 15:
		if(wifi_rx_inputBuf[0]== '0'){   //hex :4B - "K" -fixed
            net_t.wifi_link_net_success = 0;//wifi_t.esp8266_login_cloud_success =0;
            gpro_t.get_beijing_time_success = 0;
            net_t.rx_data_state =0;
            net_t.rx_counter=0;
        }
        else if(wifi_rx_inputBuf[0]== '1'){

            net_t.wifi_link_net_success = 1;//net_t.esp8266_login_cloud_success =1;
       
             net_t.rx_data_state =0;
             net_t.rx_counter=0;
        }
        else{

           net_t.rx_data_state =0;
           net_t.rx_counter=0;


        }

      break;

	

	  default:
      break;
      }
 

}
 
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
  
   

           if(strstr((const char*)gpro_t.wifi_rx_data_array,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		net_t.soft_ap_config_success=1;
					
            }
           
            if(strstr((const char*)gpro_t.wifi_rx_data_array,"+TCMQTTCONN:OK")){
	              net_t.wifi_link_net_success=1;
	              net_t.linking_tencent_cloud_doing=0;
				  gctl_t.auto_link_cloud_flag=0xff;
	
				  wifi_t.soft_ap_config_flag=0;
			  }
              else if(strstr((char*)gpro_t.wifi_rx_data_array,"+CME ERROR:208")){
						
			       net_t.linking_tencent_cloud_doing=0; //release this flag. usart
				   //wifi_t.esp8266_login_cloud_success =0;
                   net_t.wifi_link_net_success=1;
				
				   wifi_t.gTimer_auto_detected_net_state_times=0;
				
				   wifi_t.get_rx_beijing_time_enable=0;
		    }
            else if(strstr((char*)gpro_t.wifi_rx_data_array,"+TCPRDINFOSET:1,\"EHQB1P53IH\"")){ //WT.EDIT 2024.07.22

                    // wifi_t.serch_for_wifi_flag=1;
                   //  wifi_t.gTime_link_time_start =0;

              }
              else  if(strstr((char*)gpro_t.wifi_rx_data_array,"+TCMQTTCONN:FAIL,202")){

                      //  wifi_t.esp8266_login_cloud_success =0;
                       net_t.wifi_link_net_success=0;
                       gpro_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31
                        
                      //wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                      net_t.linking_tencent_cloud_doing=0;
                      wifi_t.get_rx_beijing_time_enable=0;
              }
		  else if(strstr((const char*)gpro_t.wifi_rx_data_array,"+TCMQTTCONN:OK")){
	              net_t.wifi_link_net_success=1;
	              net_t.linking_tencent_cloud_doing=0;
				  gctl_t.auto_link_cloud_flag=0xff;
				
				  wifi_t.soft_ap_config_flag=0;
			  }
             else if(strstr((char*)gpro_t.wifi_rx_data_array,"+CME ERROR:208")){
				
			
				  net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
			     net_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
			     wifi_t.get_rx_beijing_time_enable=0;
			
             }
              else if(strstr((char*)gpro_t.wifi_rx_data_array,"+TCMQTTCONN:FAIL,202")){

                        net_t.wifi_link_net_success=0; //wifi_t.esp8266_login_cloud_success =0;
                        gpro_t.wifi_led_fast_blink_flag=0;   //WT.EDIT .2024.07.31
                        
                      net_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                          
                      wifi_t.get_rx_beijing_time_enable=0;
              }
              else if(strstr((char*)gpro_t.wifi_rx_data_array,"+TCMQTTDISCON")){
          
                       net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
                       wifi_t.get_rx_beijing_time_enable=0;
                       net_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
              }
              else if(strstr((char*)gpro_t.wifi_rx_data_array,"+TCSAP:WIFI_CONNECT_FAILED")){
          
                       net_t.wifi_link_net_success=0;//wifi_t.esp8266_login_cloud_success =0;
                      wifi_t.get_rx_beijing_time_enable=0;
                      net_t.linking_tencent_cloud_doing=1;//wifi_t.linking_tencent_cloud_doing=1; //release this flag. usart
                    
              

              }
  
    gpro_t.wifi_rx_data_counter=0;
    wifi_t.once_rx_data_done = 1;
         
}


void clear_rx_copy_data(void)
{
   //uint8_t i;
   #if 0
   if( wifi_t.once_rx_data_done == 1 && wifi_link_net_state() == 0){

      wifi_t.once_rx_data_done ++;

      for(i=0;i<150;i++){
        
       wifi_t.wifi_data[i]=0;
      // wifi_t.auto_det_data[i]=0;
      }
    }
   #endif 
   if( wifi_t.once_rx_data_done == 1){
      wifi_t.once_rx_data_done ++; //WT.EDIT 2024.08.17
      memset(gpro_t.wifi_rx_data_array,'\0',150);
    }
}

/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{
    
   
    if(net_t.rx_data_success==1){
            net_t.rx_data_success=0;
        
	    
	if(wifi_t.received_data_from_tencent_cloud > 22){
	    wifi_t.received_data_from_tencent_cloud=0;
		wifi_t.get_rx_beijing_time_enable=0;
		gctl_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
         rx_app_timer_power_on_flag=1;
	   // __HAL_UART_CLEAR_OREFLAG(&huart2);
		strncpy((char*)TCMQTTRCVPUB,(char *)gpro_t.wifi_rx_data_array,40);
	    
	
	}
	else{
	if(strstr((char *)gpro_t.wifi_rx_data_array,"open\":0")){
		  gctl_t.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	else if(strstr((char *)gpro_t.wifi_rx_data_array,"open\":1")){
	   
	   gctl_t.response_wifi_signal_label = OPEN_ON_ITEM;
	}


	
	if(strstr((char *)gpro_t.wifi_rx_data_array,"ptc\":0")){
            if(gpro_t.gpower_on ==power_on){
				  gctl_t.gDry=0;
	           gctl_t.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"ptc\":1")){
            if(gpro_t.gpower_on ==power_on){
	          gctl_t.gDry=1;
			  gctl_t.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"Anion\":0")){
          if(gpro_t.gpower_on ==power_on){
	          //  gctl_t.gPlasma=0;
			gctl_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"Anion\":1")){
            if(gpro_t.gpower_on ==power_on){
            //gctl_t.gPlasma=1;
			gctl_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"sonic\":0")){
            if(gpro_t.gpower_on ==power_on){
           // gctl_t.gUlransonic=0;
			gctl_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"sonic\":1")){
            if(gpro_t.gpower_on ==power_on){
            gctl_t.gUlransonic=1;
			gctl_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }

	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"state\":1")){
           if(gpro_t.gpower_on ==power_on){
            gctl_t.gModel=1;
			gctl_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}
		  
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"state\":2")){
            if(gpro_t.gpower_on ==power_on){
            gctl_t.gModel=2;
			gctl_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }
			
    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"temperature")){

	        if(gpro_t.gpower_on ==power_on){
			gctl_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)gpro_t.wifi_rx_data_array,"find")){

		 if(gpro_t.gpower_on ==power_on){

			gctl_t.response_wifi_signal_label= FAN_ITEM;
		}
	}
 
    }
    }
 }

/****************************************************************************
*****************************************************************************/
void Json_Parse_Command_Fun(void)
{

  uint8_t i;
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	static uint8_t buzzer_temp_on;


   switch(gctl_t.response_wifi_signal_label){
  
	   wifi_t.get_rx_beijing_time_enable=0; //enab
	

     case OPEN_ON_ITEM:
      
		MqttData_Publish_SetOpen(1);  
		HAL_Delay(100);//osDelay(100);//HAL_Delay(100);

        Publish_Data_ToTencent_Initial_Data();
	    HAL_Delay(200);

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
		gctl_t.ptc_remove_warning_send_data =0;
		gpro_t.gpower_on = power_on;//gctl_t.rx_command_tag= POWER_ON;
		gctl_t.power_off_ref_value_flag=1; // app power on 
	    SendWifiData_To_Cmd(0x01,0x01);
		HAL_Delay(5);
       
		buzzer_temp_on=0;
		gctl_t.response_wifi_signal_label = 0xff;

	  break;

       case OPEN_OFF_ITEM:

      
		 	MqttData_Publish_SetOpen(0);  
			HAL_Delay(100);

            gpro_t.gpower_on = power_off;
	
            SendWifiData_To_Cmd(0x01,0x0);
			HAL_Delay(5);
          
			buzzer_temp_on=0;
	
         
        gctl_t.response_wifi_signal_label = 0xff;
        
	  break;

	  case PTC_ON_ITEM:
	  if(gpro_t.gpower_on ==power_on){
	    if(gctl_t.ptc_warning ==0){
         MqttData_Publish_SetPtc(0x01);
	  	 osDelay(100);//HAL_Delay(350);

         if(gctl_t.interval_time_two_hours_stop_flag ==0){
	       gctl_t.gDry=1;
         }
		
		 SendWifiData_To_Cmd(0x02,0x01);
		 HAL_Delay(5);
		
	     }
		 else{
			gctl_t.gDry=0;
			MqttData_Publish_SetPtc(0);
		    osDelay(100); //HAL_Delay(350);
			SendWifiData_To_Cmd(0x02,0x0);
            HAL_Delay(5);
		 }
         
		  buzzer_temp_on=0;
          gctl_t.response_wifi_signal_label=0xff;
	  	}
	    
	   break;

	  case PTC_OFF_ITEM:
	  	if(gpro_t.gpower_on ==power_on){
	
         MqttData_Publish_SetPtc(0);
		 osDelay(100);//HAL_Delay(350);
	     gctl_t.gDry=0;
		
		 SendWifiData_To_Cmd(0x02,0x0);
         HAL_Delay(5);
	 
		buzzer_temp_on=0;
	     gctl_t.response_wifi_signal_label = 0xff;

	  	}
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(gpro_t.gpower_on ==power_on){
			
            MqttData_Publish_SetPlasma(0);
			osDelay(100);//HAL_Delay(350);
            gctl_t.gPlasma=0;
			
			SendWifiData_To_Cmd(0x03,0x0);
	  	   HAL_Delay(5);
	  	}
      
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(gpro_t.gpower_on ==power_on){
            MqttData_Publish_SetPlasma(1);
			osDelay(100);//HAL_Delay(350);
			if(gctl_t.interval_time_two_hours_stop_flag ==0){
              gctl_t.gPlasma=1;
            }
			
			SendWifiData_To_Cmd(0x03,0x01);
	  	   HAL_Delay(5);
	  	}
  
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(gpro_t.gpower_on ==power_on){

            MqttData_Publish_SetUltrasonic(0);
			osDelay(100);	//HAL_Delay(350);
			
              gctl_t.gUlransonic=0;
            
	
			SendWifiData_To_Cmd(0x04,0x0);
			HAL_Delay(5);
        }
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	  	break;

	  case SONIC_ON_ITEM://ultransonic off
	    if(gpro_t.gpower_on ==power_on){
		
             MqttData_Publish_SetUltrasonic(1);
			 osDelay(100);	//HAL_Delay(350);

            if(gctl_t.interval_time_two_hours_stop_flag ==0){
                gctl_t.gUlransonic=1;

              }
		
			SendWifiData_To_Cmd(0x04,0x01);
			HAL_Delay(5);
        }
        
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	  	break;

	  case STATE_TIMER_MODEL_ITEM: //timer timing mode 0x0A
	  if(gpro_t.gpower_on ==power_on){
	         gctl_t.gModel=2;
            MqttData_Publish_SetState(2);
			osDelay(100);//HAL_Delay(350);
        
			SendWifiData_To_Cmd(0x27,0x02);
		   HAL_Delay(5);
        }
    
	    buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	  break;
		
	  case STATE_AI_MODEL_ITEM: // beijing timing 0x09
	  	 if(gpro_t.gpower_on ==power_on){
		
		    gctl_t.gModel=1;
            MqttData_Publish_SetState(1);
			osDelay(100);//HAL_Delay(350);
          
			SendWifiData_To_Cmd(0x27,0x01);
		   HAL_Delay(5);
        }
     
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(gpro_t.gpower_on ==power_on){
		

            temp_decade=gpro_t.wifi_rx_data_array[14]-0x30;
            temp_unit=gpro_t.wifi_rx_data_array[15]-0x30;
            gctl_t.set_temperature_value = temp_decade*10 +  temp_unit;
            if( gctl_t.set_temperature_value > 40)  gctl_t.set_temperature_value=40;
            if( gctl_t.set_temperature_value <20 )  gctl_t.set_temperature_value=20;
            MqttData_Publis_SetTemp(gctl_t.set_temperature_value);
			osDelay(100);//HAL_Delay(350);
			SendWifiData_To_Data(0x3A, gctl_t.set_temperature_value); //smart phone set temperature value .
			osDelay(5);//HAL_Delay(10);
          
       }
     
	  buzzer_temp_on=0;
	  gctl_t.response_wifi_signal_label = 0xff;
	  break;

	  case FAN_ITEM:
	    if(gpro_t.gpower_on ==power_on){

		     if(gctl_t.fan_warning ==0){

           		 wind_hundred =gpro_t.wifi_rx_data_array[7]-0x30;
	       		 wind_decade=gpro_t.wifi_rx_data_array[8]-0x30;
                 wind_unit = gpro_t.wifi_rx_data_array[9]-0x30;
            
                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0)gctl_t.set_wind_speed_value=100;
                else
                     gctl_t.set_wind_speed_value = wind_hundred*10 + wind_decade;
			
         
			MqttData_Publis_SetFan(gctl_t.set_wind_speed_value);
			osDelay(100);//HAL_Delay(350);
    		SendWifiData_To_PanelWindSpeed(gctl_t.set_wind_speed_value);
			HAL_Delay(10);
          
		    }
			else{
				gctl_t.set_wind_speed_value=0;

			    MqttData_Publis_SetFan(gctl_t.set_wind_speed_value);
				osDelay(100);//HAL_Delay(350);


			}
            
		}
     
	  	buzzer_temp_on=0;
	    gctl_t.response_wifi_signal_label = 0xff;
	  	break;

	  case APP_TIMER_POWER_ON_REF :

	  
		   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	  	
		   if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		   
			  gctl_t.app_timer_power_on_flag = 1;
              gctl_t.power_off_ref_value_flag=1; // app power on 
		
			   MqttData_Publish_SetOpen(1);  
			   osDelay(100);//HAL_Delay(350);
		
		
			   gpro_t.gpower_on = power_on;
			   SendWifiData_To_Cmd(0x21,0x01); //smart phone is open 
			   HAL_Delay(10);
            
			   buzzer_temp_on=0;
		         

				
				
			}
		    if(strstr((char *)TCMQTTRCVPUB,"open\":0")){
		   
		   
			    gctl_t.app_timer_power_on_flag = 0;
                __HAL_UART_CLEAR_OREFLAG(&huart2);
		 			MqttData_Publish_SetOpen(0);  
			       osDelay(100);//HAL_Delay(350);
	
	            gpro_t.gpower_on = power_off;

			SendWifiData_To_Cmd(0x21,0x0); //turn off power off
			HAL_Delay(10);
       
		      buzzer_temp_on=0;
				
			}

	     gctl_t.response_wifi_signal_label=0xff;

	  break;


   }


   if(gctl_t.response_wifi_signal_label==0xff){
        
        if(buzzer_temp_on ==0){
			buzzer_temp_on++;
   	        buzzer_sound();
        }
         
		

//		for(i=0;i<20;i++){
//		   gpro_t.wifi_rx_data_array[i]=0;
//		   
//
//        }

         memset(gpro_t.wifi_rx_data_array,'\0',20);
      
		gctl_t.response_wifi_signal_label=0xf0;
	}

  

  }


void Parse_Json_Statement(void)
{

   //if(rx_app_timer_power_on_flag == 1){
    
     if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){
				
			gctl_t.gDry=0;
				  
		}
		else if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){
				
				    gctl_t.gDry=1;
				  
					
		}
		
		if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
			
				   gctl_t.gPlasma=0;
				
				
			 
		}
		else if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
			
				gctl_t.gPlasma=1;
				
			
				
		}
		
		if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){
			
			     gctl_t.gUlransonic=0;
				
			
		}
		else if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){
			
				gctl_t.gUlransonic=1;
				
		   }



      rx_app_timer_power_on_flag ++;

    //  memset(TCMQTTRCVPUB,'\0',40);

  
   // }
   


}



