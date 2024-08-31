#include "bsp.h"



WIFI_FUN   wifi_t;
uint8_t sub_send_power_on_times;


void (*PowerOn)(void);
void (*PowerOff)(void);
void (*Ai_Fun)(uint8_t sig);


void (*SetTimes)(void);
void (*SetTemperature)(void);
uint8_t get_rx_beijing_time_enable;


static void AutoReconnect_Wifi_Neware_Function(void);


void PowerOn_Host(void (* poweronHandler)(void))
{
    PowerOn = poweronHandler;  

}


void PowerOff_Host(void(*poweroffHandler)(void))
{
   PowerOff = poweroffHandler;

}

uint8_t first_connect;
/****************************************************************
     * 
     * Function Name:void AI_Function_Host(void(*AIhandler)(uint8_t sig))
     * Function: take with reference of function pointer
     * 
     * 
****************************************************************/
void AI_Function_Host(void(*AIhandler)(uint8_t sig))
{
    Ai_Fun=AIhandler;
}

void SetTimeHost(void(*timesHandler)(void))
{
    SetTimes = timesHandler;

}


void SetTemperatureHost(void(*temperatureHandler)(void))
{
    SetTemperature = temperatureHandler ;

}

/***********************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *
   *
***********************************************/
void RunWifi_Command_Handler(void)
{
    uint8_t i,sub_to_tencent_flag;
   
    switch(wifi_t.runCommand_order_lable){

       case wifi_link_tencent_cloud: //02

			Wifi_SoftAP_Config_Handler();
	        SmartPhone_LinkTencent_Cloud();
	     
	      if(net_t.wifi_link_net_success==1){
			
				 SendWifiData_To_Cmd(0x1F,0x01) ;	//Link wifi net is success .WT.EDIT.2024.08.31
				gctl_t.first_link_tencent_cloud_flag =1;
				wifi_t.get_rx_beijing_time_enable=0;
                gpro_t.wifi_led_fast_blink_flag=0;
			    wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;
              
				
		  }
		  else{
             if(gctl_t.gTimer_linking_tencen_total_counter < 120){
                 wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
             }
             else{
              gpro_t.wifi_led_fast_blink_flag=0;
              wifi_t.runCommand_order_lable = wifi_null;
              SendWifiData_To_Cmd(0x1F,0x00) ;	 //Link wifi net is fail .WT.EDTI .2024.08.31
              }
           
           }
	    break;

	  	
	  	case wifi_tencent_publish_init_data://03
		  
				do {
			 
				 MqttData_Publish_SetOpen(0x01);
		         //HAL_Delay(200);
		         osDelay(100);
		         Publish_Data_ToTencent_Initial_Data();
				 //HAL_Delay(200);
                  osDelay(100);

				Subscriber_Data_FromCloud_Handler();
				//HAL_Delay(200);
	             osDelay(100);

			   
			     sub_to_tencent_flag=0;
				
				
	           
				}while(sub_to_tencent_flag);


              wifi_t.runCommand_order_lable= wifi_tencent_subscription_data;
		
		 
       	break;

		
		case wifi_tencent_subscription_data://04
           
		
				 
				 SendWifiData_To_Cmd(0x1F,0x01) ;	//link wiif net is success .
				 osDelay(100);
		
		
			 	
				  wifi_t.gTimer_get_beijing_time=0;
			

				wifi_t.runCommand_order_lable= 0xff;
                  for(i=0;i<150;i++){
	               gpro_t.wifi_rx_data_array[i]=0;

                  }
					
			
		break;

	  
	   
	  default:

	   break;
	 
     }
  
}
 
/*****************************************************************************
    *
    *Function Name: static void AutoReconnect_Wifi_Neware_Function(void)
    *Function: 
    *Input Ref: NO
    *Return Ref:NO
    *
*****************************************************************************/	 	



