#include "bsp.h"

process_t gpro_t;


static void Auto_InitWifiModule_Hardware(void);
static void Auto_SmartPhone_TryToLink_TencentCloud(void);

uint8_t power_on_login_tencent_cloud_flag  ;

 

void bsp_init(void)
{
   delay_init(24);
   sensor_dht11_init();//Update_DHT11_Value();//dht11_init();
   buzzer_init();
   wifi_init();

}


void link_wifi_to_tencent_handler(uint8_t data)
{
    if(data == 1){//if(gpro_t.wifi_led_fast_blink_flag==1){
        if(gctl_t.gTimer_linkTencentCounter  > 119){

           gpro_t.wifi_led_fast_blink_flag =0;//gpro_t.wifi_led_fast_blink_flag =0;
           if(wifi_link_net_state()==0){

              gpro_t.get_beijing_flag = 10;
              wifi_t.gTimer_auto_detected_net_state_times = 120;
               net_t.linking_tencent_cloud_doing =1;
               wifi_t.soft_ap_config_flag =1; //WE.EIDT 

           }

    }
    else{

       link_wifi_net_handler();
    }
        

    }
    else{
       send_data_to_disp();
       adc_detected_hundler();
       if(wifi_link_net_state() ==1 && gpro_t.gTimer_publis_dht11_data > 59){
        gpro_t.gTimer_publis_dht11_data=0;

        Update_Dht11_Totencent_Value();
        osDelay(10);//HAL_Delay(200) //WT.EDIT 2024.08.10
        }
      }
       

}

/**********************************************************************
    *
    *Function Name:void send_data_to_disp(void)
    *Function: 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void send_data_to_disp(void)
{

    if(gctl_t.gTImer_send_data_to_disp > 5){
	
	   gctl_t.gTImer_send_data_to_disp=0;
	   Update_DHT11_Value();
       
		
	}


}


/**********************************************************************
    *
    *Function Name:uint8_t bcc_check(const unsigned char *data, int len) 
    *Function: BCC校验函数
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
uint8_t bcc_check(const unsigned char *data, int len) 
{
    unsigned char bcc = 0;
    for (int i = 0; i < len; i++) {
        bcc ^= data[i];
    }
    return bcc;
}


/********************************************************************************
	*
	*Functin Name:void wifi_communication_tnecent_handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void wifi_communication_tnecent_handler(void)
{

   if(wifi_t.get_rx_beijing_time_enable==0){
    
         Tencent_Cloud_Rx_Handler();
    	 Json_Parse_Command_Fun();
  
   }
}




/**********************************************************************
	*
	*Functin Name: void adc_detected_hundler(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void adc_detected_hundler(void)
{
   

   if(gctl_t.gTimer_ptc_adc_times > 10 && gpro_t.stopTwoHours_flag==0){ //65s//3 minutes 120s
        gctl_t.gTimer_ptc_adc_times=0;
        
       Get_Ptc_ADC_Fun(ADC_CHANNEL_1,20);
        
        

    }
    if(gctl_t.gTimer_fan_adc_times > 19 && gpro_t.stopTwoHours_flag ==0 && fan_warning_flag  == 0){ //detected 3 times is 60s 
        gctl_t.gTimer_fan_adc_times =0;
        Get_Fan_ADC_Fun(ADC_CHANNEL_0,20);
        
    }

    fan_warning_sound();

}


/**********************************************************************
	*
	*Functin Name: void wifi_auto_detected_link_state(void)
	*Function :
	*Input Ref:  NO
	*Return Ref: NO
	*
**********************************************************************/
void wifi_auto_detected_link_state(void)
{
    static uint8_t dc_power_on;
	if(power_on_login_tencent_cloud_flag     <  5 && wifi_link_net_state()==0){
		
      net_t.linking_tencent_cloud_doing = 1;
      gpro_t.gTimer_dc_power_on_auto_link_net = 0;

      Auto_InitWifiModule_Hardware();//InitWifiModule();
      Auto_SmartPhone_TryToLink_TencentCloud();
	 
    }
    if(wifi_link_net_state()==1    && gpro_t.gTimer_dc_power_on_auto_link_net > 1 && dc_power_on ==0 ){
              
             dc_power_on ++ ;
           //wifi_t.linking_tencent_cloud_doing = 0;
           net_t.linking_tencent_cloud_doing  =0;
           gpro_t.process_run_step=0;
        
          if(gpro_t.gpower_on == power_off){
		     MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);

          }
          
          Subscriber_Data_FromCloud_Handler();
          HAL_Delay(200);
         

          SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
   }
   

   
   
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: power on auto link net fun
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
static void Auto_InitWifiModule_Hardware(void)
{
  
	//WIFI_IC_ENABLE();
	if(power_on_login_tencent_cloud_flag ==0){
	   power_on_login_tencent_cloud_flag=1;
	   gpro_t.gTimer_power_on_first_link_tencent=0;
       gpro_t.wifi_rx_data_counter=0;
       net_t.linking_tencent_cloud_doing = 1;
	   //at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n")); //
	   at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
       HAL_Delay(1000);

	}
	if(gpro_t.gTimer_power_on_first_link_tencent > 2 &&  power_on_login_tencent_cloud_flag==1 ){
	   gpro_t.gTimer_power_on_first_link_tencent=0;
	   power_on_login_tencent_cloud_flag=2;
	   
	  }
}

static void Auto_SmartPhone_TryToLink_TencentCloud(void)
{
    
  if(power_on_login_tencent_cloud_flag==2 && gpro_t.gTimer_power_on_first_link_tencent >3){
   	power_on_login_tencent_cloud_flag++;
	net_t.linking_tencent_cloud_doing  = 1;
    gpro_t.wifi_rx_data_counter=0;
	wifi_t.soft_ap_config_flag =0;
   }
  
   if(power_on_login_tencent_cloud_flag==3 ){
		
	   gpro_t.gTimer_power_on_first_link_tencent=0;
       power_on_login_tencent_cloud_flag++;
       HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开
       HAL_Delay(1000);
	  
	}
   
   if(wifi_link_net_state()==1 && power_on_login_tencent_cloud_flag ==4){
			//wifi_t.linking_tencent_cloud_doing =0;
			net_t.linking_tencent_cloud_doing= 0;
            power_on_login_tencent_cloud_flag++;
            SendWifiData_To_Cmd(0x1F,0x01); //link wifi order 1 --link wifi net is success.
	}
    else if(wifi_link_net_state()==0 && power_on_login_tencent_cloud_flag ==4){
       power_on_login_tencent_cloud_flag++;
        SendWifiData_To_Cmd(0x1F,0x00);
    }
}





