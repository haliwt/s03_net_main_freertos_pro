#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "bsp.h"


uint8_t TCMQTTRCVPUB[40];



 
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
      static uint8_t det_wifi_link;
    switch(net_t.rx_data_state)
      {
      case 0: //#0

	  	if((wifi_rx_inputBuf[0] == '"') ||wifi_rx_inputBuf[0]=='+') //hex :54 - "T" -fixed
            net_t.rx_data_state=1; //=1
          else{
               net_t.rx_counter=0;
            
            }
        break;

      case 1:
      
         if((wifi_rx_inputBuf[0] == 'p')  ||wifi_rx_inputBuf[0]=='T')//hex :54 - "T" -fixed
            net_t.rx_data_state=2; //=1
          else{
               net_t.rx_counter=0;
            
            }
            
         break;
      case 2: //#1
             if((wifi_rx_inputBuf[0] == 'a')||wifi_rx_inputBuf[0]=='C')  //hex :4B - "K" -fixed
            net_t.rx_data_state=3; //=1
         else{
            net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
         break;
            
        case 3:
            if((wifi_rx_inputBuf[0] == 'r')||wifi_rx_inputBuf[0]=='M')    //hex :4B - "K" -fixed
            net_t.rx_data_state=4; //=1
         else{
           net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
        
        break;
        
        case 4:
            if((wifi_rx_inputBuf[0] == 'a')  ||wifi_rx_inputBuf[0]=='Q')  //hex :4B - "K" -fixed
            net_t.rx_data_state=5; //=1
         else{
            net_t.rx_data_state =0;
             net_t.rx_counter=0;
         }
        
        break;

      case 5:
       if((wifi_rx_inputBuf[0] == 'm') ||wifi_rx_inputBuf[0]=='T')   //hex :4B - "K" -fixed
         net_t.rx_data_state=6; //=1
         else{
           net_t.rx_data_state=0;
            net_t.rx_counter=0;
         }
            
      break;

      
      case 6:
       if((wifi_rx_inputBuf[0] == 's')||wifi_rx_inputBuf[0]=='T')    //hex :4B - "K" -fixed
         net_t.rx_data_state=7; //=1
         else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }
            
      break;

      case 7:
       if((wifi_rx_inputBuf[0] == '"')||wifi_rx_inputBuf[0]=='R' ||wifi_rx_inputBuf[0]=='C' ){  //hex :4B - "K" -fixed
         net_t.rx_data_state=8; //=1
    	}
		else if(wifi_rx_inputBuf[0]==':' ){

             net_t.rx_data_state=8;
			 det_wifi_link=1;//wifi_t.wifi_reconnect_read_flag = gpro_t.wifi_rx_inputBuf[0];
		}
		else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }
           
      break;

       case 8:
       if((wifi_rx_inputBuf[0] == ':') ||wifi_rx_inputBuf[0]=='E' ||wifi_rx_inputBuf[0]=='O' ) //hex :4B - "K" -fixed
         net_t.rx_data_state=9; //=1
         else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }

		
            
      break;


      case 9:
       if((wifi_rx_inputBuf[0] == '{') ||wifi_rx_inputBuf[0]=='C' ||wifi_rx_inputBuf[0]=='N'){ //hex :4B - "K" -fixed
         net_t.rx_data_state=10; //=1
       	}
         else{
           net_t.rx_data_state =0;
            net_t.rx_counter=0;
         }
            
      break;
         
     case 10:
        
         if(net_t.rx_data_success==0){
		 	
         	gpro_t.wifi_rx_data_array[net_t.rx_counter] = wifi_rx_inputBuf[0];
            net_t.rx_counter++ ;
	
		    
            
         if(wifi_rx_inputBuf[0]=='}' || wifi_rx_inputBuf[0]==0x0A) //0x7D='}' // end
         {
            net_t.rx_data_success=1;
            net_t.rx_data_state=0;
			wifi_t.received_data_from_tencent_cloud = net_t.rx_counter;
            net_t.rx_counter=0;
            
          
         }
		 else if(wifi_rx_inputBuf[0]=='O' || wifi_rx_inputBuf[0]=='N'){ //auto reconect be detected 

                  net_t.rx_data_state=11; //=1


		 }
		 else 
		   net_t.rx_data_state=10; 
         }
         else{
			net_t.rx_data_success=0;

            net_t.rx_data_state =0;
            net_t.rx_counter=0;
			wifi_t.received_data_from_tencent_cloud =0;

         }
       
            
      break;


	  case 11:
		if(wifi_rx_inputBuf[0]=='N' ||wifi_rx_inputBuf[0]==':'){
		
			net_t.rx_data_state=12; //=1
		
		
		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

      case 12:
		if(wifi_rx_inputBuf[0]=='N' || wifi_rx_inputBuf[0]=='O'){
		
			net_t.rx_data_state=13; //=1
		
		
		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 13:
		if(wifi_rx_inputBuf[0]=='E' ){
		
			net_t.rx_data_state=14; //=1
		
		
		}
		else if(wifi_rx_inputBuf[0]=='K'){


            wifi_t.wifi_reconnect_read_flag = 0;
		    net_t.rx_data_state =0;
            net_t.rx_counter=0;

		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 14:
		if(wifi_rx_inputBuf[0]=='C'){
		
			net_t.rx_data_state=15; //=1
		
		
		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 15:
		if(wifi_rx_inputBuf[0]=='T'){
		
			net_t.rx_data_state=16; //=1
		
		
		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 16:
		if(wifi_rx_inputBuf[0]=='I'){
		
			net_t.rx_data_state=17; //=1
		
		
		}
		else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 17:
		if(wifi_rx_inputBuf[0]=='N'){
		
				net_t.rx_data_state=18; //=1
		
		
		}
        else{
		  net_t.rx_data_state=0; 

		}

      break;

	  case 18:
		if(wifi_rx_inputBuf[0]=='G'){
		
			wifi_t.wifi_reconnect_read_flag = 1;
		    net_t.rx_data_state =0;
            net_t.rx_counter=0;
		
		
		}
        else{
		  net_t.rx_data_state=0; 

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
    
    strcpy((char *)net_t.data, (const char *)gpro_t.wifi_rx_data_array);
          net_t.data_size = gpro_t.wifi_counter;


		   if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)net_t.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		net_t.soft_ap_config_success=1;
					wifi_t.soft_ap_config_flag=0;
               	}

			
            else{
				  if(strstr((const char*)net_t.data,"+TCMQTTCONN:OK")){
	              net_t.esp8266_login_cloud_success=1;
	              net_t.linking_tencent_cloud_doing=0;
				  gctl_t.auto_link_cloud_flag=0xff;
				  wifi_t.wifi_reconnect_read_flag = 0;
				  wifi_t.soft_ap_config_flag=0;
			  }
           
           }
		  }
		  else{

		     if(strstr((const char*)net_t.data,"+TCMQTTCONN:OK")){
	              net_t.esp8266_login_cloud_success=1;
	              net_t.linking_tencent_cloud_doing=0;
				  gctl_t.auto_link_cloud_flag=0xff;
				  wifi_t.wifi_reconnect_read_flag = 0;
				  wifi_t.soft_ap_config_flag=0;
			  }



		  }
         gpro_t.wifi_rx_data_done_flag = 0;
         gpro_t.wifi_counter=0;
         
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
    

    if( net_t.rx_data_success==1){
            net_t.rx_data_success=0;
        
	      gctl_t.set_beijing_time_flag =0; //WT.EDIT 2023.06.12
		 // wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	
     if(wifi_t.received_data_from_tencent_cloud ==0x25){
	    wifi_t.received_data_from_tencent_cloud=0;
		wifi_t.get_rx_beijing_time_enable=0;
		gctl_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
	    __HAL_UART_CLEAR_OREFLAG(&huart2);
		strcpy((char*)TCMQTTRCVPUB,(char *)gpro_t.wifi_rx_data_array);
	    
	
	}
	else{
	if(strstr((char *)gpro_t.wifi_rx_data_array,"open\":0")){
		  gctl_t.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	else if(strstr((char *)gpro_t.wifi_rx_data_array,"open\":1")){
	   
	   gctl_t.response_wifi_signal_label = OPEN_ON_ITEM;
	}


	
	if(strstr((char *)gpro_t.wifi_rx_data_array,"ptc\":0")){
            if(gctl_t.gPower_flag ==POWER_ON){
				  gctl_t.gDry=0;
	           gctl_t.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"ptc\":1")){
            if(gctl_t.gPower_flag ==POWER_ON){
	          gctl_t.gDry=1;
			  gctl_t.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"Anion\":0")){
          if(gctl_t.gPower_flag ==POWER_ON){
	          //  gctl_t.gPlasma=0;
			gctl_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"Anion\":1")){
            if(gctl_t.gPower_flag ==POWER_ON){
            //gctl_t.gPlasma=1;
			gctl_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"sonic\":0")){
            if(gctl_t.gPower_flag ==POWER_ON){
           // gctl_t.gUlransonic=0;
			gctl_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"sonic\":1")){
            if(gctl_t.gPower_flag ==POWER_ON){
            gctl_t.gUlransonic=1;
			gctl_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }

	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"state\":1")){
           if(gctl_t.gPower_flag ==POWER_ON){
            gctl_t.gModel=1;
			gctl_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}
		  
    }
    else if(strstr((char *)gpro_t.wifi_rx_data_array,"state\":2")){
            if(gctl_t.gPower_flag ==POWER_ON){
            gctl_t.gModel=2;
			gctl_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }
			
    }
	
    if(strstr((char *)gpro_t.wifi_rx_data_array,"temperature")){

	        if(gctl_t.gPower_flag ==POWER_ON){
			gctl_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)gpro_t.wifi_rx_data_array,"find")){

		 if(gctl_t.gPower_flag ==POWER_ON){

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
       gctl_t.set_beijing_time_flag =0;
	   wifi_t.get_rx_beijing_time_enable=0; //enab
	

      case OPEN_OFF_ITEM:

           if(gctl_t.app_timer_power_off_flag ==0){
		 	MqttData_Publish_SetOpen(0);  
			HAL_Delay(350);

			gctl_t.rx_command_tag= POWER_OFF;//RUN_COMMAND;
	       // gctl_t.RunCommand_Label=POWER_OFF;
            SendWifiData_To_Cmd(0x01,0x0);
			HAL_Delay(5);
            gctl_t.wifi_power_on_flag=0;
			buzzer_temp_on=0;
	
           }
		   else
		      buzzer_temp_on++;
        gctl_t.response_wifi_signal_label = 0xff;
        
	  break;

	  case OPEN_ON_ITEM:
      
		MqttData_Publish_SetOpen(1);  
		HAL_Delay(300);

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
		gctl_t.ptc_remove_warning_send_data =0;
		gctl_t.rx_command_tag= POWER_ON;
	    SendWifiData_To_Cmd(0x01,0x01);
		HAL_Delay(5);
        gctl_t.wifi_power_on_flag =1;
		buzzer_temp_on=0;
		gctl_t.response_wifi_signal_label = 0xff;

	  break;

	  case PTC_ON_ITEM:
	  if(gctl_t.gPower_flag ==POWER_ON){
	    if(gctl_t.ptc_warning ==0){
         MqttData_Publish_SetPtc(0x01);
	  	 HAL_Delay(350);
	     gctl_t.gDry=1;
		
		 SendWifiData_To_Cmd(0x02,0x01);
		 HAL_Delay(5);
		
	     }
		 else{
			gctl_t.gDry=0;
			MqttData_Publish_SetPtc(0);
		     HAL_Delay(350);
			SendWifiData_To_Cmd(0x02,0x0);
            HAL_Delay(5);
		 }
           gctl_t.wifi_power_on_flag=0;  
		  buzzer_temp_on=0;
          gctl_t.response_wifi_signal_label=0xff;
	  	}
	    
	   break;

	  case PTC_OFF_ITEM:
	  	if(gctl_t.gPower_flag ==POWER_ON){
	
         MqttData_Publish_SetPtc(0);
		 HAL_Delay(350);
	     gctl_t.gDry=0;
		
		 SendWifiData_To_Cmd(0x02,0x0);
         HAL_Delay(5);
	  	 gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	     gctl_t.response_wifi_signal_label = 0xff;

	  	}
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(gctl_t.gPower_flag ==POWER_ON){
			
            MqttData_Publish_SetPlasma(0);
			HAL_Delay(350);
            gctl_t.gPlasma=0;
			
			SendWifiData_To_Cmd(0x03,0x0);
	  	   HAL_Delay(5);
	  	}
         gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(gctl_t.gPower_flag ==POWER_ON){
            MqttData_Publish_SetPlasma(1);
			HAL_Delay(350);
            gctl_t.gPlasma=1;
			
			SendWifiData_To_Cmd(0x03,0x01);
	  	   HAL_Delay(5);
	  	}
         gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(gctl_t.gPower_flag ==POWER_ON){

            MqttData_Publish_SetUltrasonic(0);
				HAL_Delay(350);
            gctl_t.gUlransonic=0;
	
			SendWifiData_To_Cmd(0x04,0x0);
			HAL_Delay(5);
        }
         gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	  	break;

	  case SONIC_ON_ITEM://ultransonic off
	    if(gctl_t.gPower_flag ==POWER_ON){
		
             MqttData_Publish_SetUltrasonic(1);
			 	HAL_Delay(350);
            gctl_t.gUlransonic=1;
		
			SendWifiData_To_Cmd(0x04,0x0);
			HAL_Delay(5);
        }
         gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label=0xff;
	  	break;

	  case STATE_TIMER_MODEL_ITEM:
	  if(gctl_t.gPower_flag ==POWER_ON){
	         gctl_t.gModel=2;
            MqttData_Publish_SetState(2);
			HAL_Delay(350);
        
			SendWifiData_To_Cmd(0x07,0x0);
		   HAL_Delay(5);
        }
        gctl_t.wifi_power_on_flag=0;
	    buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	  break;
		
	  case STATE_AI_MODEL_ITEM:
	  	 if(gctl_t.gPower_flag ==POWER_ON){
		
		    gctl_t.gModel=1;
            MqttData_Publish_SetState(1);
			HAL_Delay(350);
          
			SendWifiData_To_Cmd(0x07,0x0);
		   HAL_Delay(5);
        }
        gctl_t.wifi_power_on_flag=0;
		buzzer_temp_on=0;
	   gctl_t.response_wifi_signal_label = 0xff;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(gctl_t.gPower_flag ==POWER_ON){
		

            temp_decade=gpro_t.wifi_rx_data_array[14]-0x30;
            temp_unit=gpro_t.wifi_rx_data_array[15]-0x30;
            gctl_t.set_temperature_value = temp_decade*10 +  temp_unit;
            if( gctl_t.set_temperature_value > 40)  gctl_t.set_temperature_value=40;
            if( gctl_t.set_temperature_value <20 )  gctl_t.set_temperature_value=20;
            MqttData_Publis_SetTemp(gctl_t.set_temperature_value);
			HAL_Delay(350);
			SendWifiData_To_WifiSetTemp(gctl_t.set_temperature_value);
			HAL_Delay(10);
          
       }
      gctl_t.wifi_power_on_flag=0;
	  buzzer_temp_on=0;
	  gctl_t.response_wifi_signal_label = 0xff;
	  break;

	  case FAN_ITEM:
	    if(gctl_t.gPower_flag ==POWER_ON){

		     if(gctl_t.fan_warning ==0){

           		 wind_hundred =gpro_t.wifi_rx_data_array[7]-0x30;
	       		 wind_decade=gpro_t.wifi_rx_data_array[8]-0x30;
                 wind_unit = gpro_t.wifi_rx_data_array[9]-0x30;
            
                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0)gctl_t.set_wind_speed_value=100;
                else
                     gctl_t.set_wind_speed_value = wind_hundred*10 + wind_decade;
			
         
			MqttData_Publis_SetFan(gctl_t.set_wind_speed_value);
			HAL_Delay(350);
    		SendWifiData_To_PanelWindSpeed(gctl_t.set_wind_speed_value);
			HAL_Delay(10);
          
		    }
			else{
				gctl_t.set_wind_speed_value=0;

			    MqttData_Publis_SetFan(gctl_t.set_wind_speed_value);
				HAL_Delay(350);


			}
            
		}
         gctl_t.wifi_power_on_flag=0;
	  	buzzer_temp_on=0;
	    gctl_t.response_wifi_signal_label = 0xff;
	  	break;

	  case APP_TIMER_POWER_ON_REF :

	       gctl_t.set_beijing_time_flag=0;
		   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	  	
		   if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		   
			  gctl_t.app_timer_power_on_flag = 1;
		      gctl_t.app_timer_power_off_flag = 0;
			   MqttData_Publish_SetOpen(1);  
			   HAL_Delay(350);
				gctl_t.rx_command_tag= RUN_COMMAND;
			   gctl_t.RunCommand_Label=POWER_ON;
			   SendWifiData_To_Cmd(0x20,0x01);
			   HAL_Delay(10);
                gctl_t.wifi_power_on_flag=0;
			   buzzer_temp_on=0;
		         

				
				
			}
		   
            if(strstr((char *)TCMQTTRCVPUB,"open\":0")){
		   
		        gctl_t.app_timer_power_off_flag = 1;
			    gctl_t.app_timer_power_on_flag = 0;
                __HAL_UART_CLEAR_OREFLAG(&huart2);
		 			MqttData_Publish_SetOpen(0);  
			       HAL_Delay(350);
			gctl_t.rx_command_tag= RUN_COMMAND;
	         gctl_t.RunCommand_Label=POWER_OFF;

			SendWifiData_To_Cmd(0x01,0x0); //turn off power off
			HAL_Delay(10);
             gctl_t.wifi_power_on_flag=0;
		      buzzer_temp_on=0;
				
			}

	     gctl_t.response_wifi_signal_label=0xff;

	  break;


   }


   if(gctl_t.response_wifi_signal_label==0xff){
        
        if(buzzer_temp_on ==0 &&  gctl_t.wifi_power_on_flag ==0){
			buzzer_temp_on++;
   	        buzzer_sound();
        }
         
		gctl_t.response_wifi_signal_label=0xf0;

		for(i=0;i<20;i++){
		   gpro_t.wifi_rx_data_array[i]=0;
		   

        }
      
		
	}

  

  }
void Wifi_Rx_Beijing_Time_Handler(void)
{

   
    switch(net_t.rx_data_state)
      {
      case 0:  //#0

            
         if(wifi_rx_inputBuf[0] == 0x01 || wifi_rx_inputBuf[0] == 0x02 ||wifi_rx_inputBuf[0] == 0x03 ||wifi_rx_inputBuf[0] == 0x04 
		 	|| wifi_rx_inputBuf[0] == 0x05 ||wifi_rx_inputBuf[0] == 0x06 ||wifi_rx_inputBuf[0] == 0x07 ||wifi_rx_inputBuf[0] == 0x08 
		 	|| wifi_rx_inputBuf[0] == 0x09 ||wifi_rx_inputBuf[0] == 0x0a ||wifi_rx_inputBuf[0] == 0x0b ||wifi_rx_inputBuf[0] == 0x0c ){ 
             net_t.rx_data_state=1; //=1
		  }
		  else{
               net_t.rx_counter=0;
               gpro_t.wifi_rx_data_done_flag = 0;
               gpro_t.wifi_counter=0;              
            }
         break;

	  case 1:
	  	 if(wifi_rx_inputBuf[0] == ' ')
		  	 net_t.rx_data_state=2; //=1
		  else{
		      net_t.rx_data_state=0; //=1

          }
	  	break;

      case 2:
      
           wifi_t.real_hours=wifi_rx_inputBuf[0];
           net_t.rx_data_state=3; //=1
         break;

	  case 3:
	  	  if(wifi_rx_inputBuf[0] == ':')
		  	 net_t.rx_data_state=4; //=1
		  else{
		      net_t.rx_data_state=0; //=1

          }
	  	break;
      case 4: //#1
      
            wifi_t.real_minutes = wifi_rx_inputBuf[0];
            net_t.rx_data_state=5; //=1
       
         break;

	  case 5:
	  	 if(wifi_rx_inputBuf[0] == ':')
		  	 net_t.rx_data_state=6; //=1
		  else{
		      net_t.rx_data_state=0; //=1

          }
	  break;
            
        case 6:
           wifi_t.real_seconds = wifi_rx_inputBuf[0];
           net_t.rx_data_state=7; //=1
           
           
        
        break;

		case 7:
			if(wifi_rx_inputBuf[0] == ' ')
		  	 net_t.rx_data_state=8; //=1
		  else{
		      net_t.rx_data_state=0; //=1

          }

		break;
		  
		case 8:
			 if(wifi_rx_inputBuf[0] ==20){
                wifi_t.get_rx_beijing_time_enable=0 ;
				net_t.rx_data_state=0; //=1
				  
		     }
			 
			
			
		break;
        
        

        default:
         
         
       break;
      }




}

void Wifi_Get_Beijing_Time_Handler(void)
{
 /*AT+CIPSNTPTIME?+CIPSNTPTIME:Wed Jan 11 19:31:04 2023 OK */



}

void Parse_Json_Statement(void)
{

 
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







}



