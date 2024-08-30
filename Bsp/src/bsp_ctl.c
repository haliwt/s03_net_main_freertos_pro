#include "bsp.h"


_run_t gctl_t; 



static void Single_Command_ReceiveCmd(uint8_t cmd); 
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
static void Single_Command_ReceiveCmd(uint8_t cmd)
{
    static uint8_t no_buzzer_sound_dry_off;
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	        gctl_t.noBuzzer_sound_dry_flag =1;

       case DRY_ON:
         gctl_t.gDry = 1;
	      gctl_t.gFan_continueRun =0;
	   if(gctl_t.noBuzzer_sound_dry_flag !=1){
		     buzzer_sound();
		 }
		if(esp8266data.esp8266_login_cloud_success==1)
		 MqttData_Publish_SetPtc(0x01);
		 HAL_Delay(200);
		 
       break;

	   case DRY_OFF_NO_BUZZER :

	         no_buzzer_sound_dry_off=1;

	  case DRY_OFF:
 			gctl_t.gDry = 0;
			if( no_buzzer_sound_dry_off !=1)
			     buzzer_sound();
			 if(gctl_t.gPlasma ==0){ //plasma turn off flag
			  gctl_t.gFan_counter =0;
			   gctl_t.gFan_continueRun =1;

		     }
			if(esp8266data.esp8266_login_cloud_success==1)
			MqttData_Publish_SetPtc(0x0);
			HAL_Delay(200);
			
       break;

       case PLASMA_ON:
       		gctl_t.gPlasma=1;
       		gctl_t.gUlransonic =1;
	         buzzer_sound();
	   if(esp8266data.esp8266_login_cloud_success==1){
	        MqttData_Publish_SetPlasma(1) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(1); //超声波
	        HAL_Delay(200);
	   	}
	   
       break;

       case PLASMA_OFF:
           gctl_t.gPlasma=0;
           gctl_t.gUlransonic =0;
	       buzzer_sound();
	   if(esp8266data.esp8266_login_cloud_success==1){
	       MqttData_Publish_SetPlasma(0) ;//杀菌
	        HAL_Delay(200);
	        MqttData_Publish_SetUltrasonic(0); //超声波
	        HAL_Delay(200);
	   	}
	   
       break;

	   case MODE_AI_NO_BUZZER :
	   	  gctl_t.gModel =1;  //AI_Works_Model 
		  MqttData_Publish_SetState(0x1); //Ai model->beijing_time
		  HAL_Delay(200);

	   break;

	   case WIFI_CONNECT_FAIL:

	       gctl_t.dp_link_wifi_fail =1;


	   break;

	   case DISPLAY_PANNEL_CONNECT_WIFI_SUCCESS:

	      
	        gctl_t.dp_link_wifi_fail =0;


	   break;




      default :
        cmd =0;

      break; 


    }



}
/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
void SystemReset(void)
{
    
		
		__set_PRIMASK(1) ;
		HAL_NVIC_SystemReset();
		

}

/**********************************************************************
	*
	*Functin Name: 
	*Function : be check key of value 
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void power_on_handler(void)
{
   uint8_t i;
   static uint8_t run_flag;
    
    switch(run_flag){

	case 0: //1
	     
		 SetPowerOn_ForDoing();

         
         gctl_t.gPower_flag = POWER_ON;
         gctl_t.gFan_continueRun =0;
         gctl_t.gPower_On=POWER_ON;
         gctl_t.gmt_time_flag=0;
          gctl_t.wifi_gPower_On = 1;

    
         gctl_t.gTImer_send_data_to_disp=0;
	     gctl_t.gTimer_senddata_panel=0;
		 gctl_t.gTimer_app_power_on=0;
		 gctl_t.app_timer_power_off_flag =0;
		 gctl_t.gTimer_continuce_works_time=0;
		 //error detected times 
		 gctl_t.ptc_warning =0;
		 gctl_t.fan_warning =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;
		 gctl_t.ptc_first_detected_times=0;
		 gctl_t.set_wind_speed_value= 100;

        gctl_t.fan_continuce=0;
        if(wifi_link_net_state() ==1){
            Subscriber_Data_FromCloud_Handler();
    		osDelay(100);//HAL_Delay(350);

         }
	    run_flag= UPDATE_TO_PANEL_DATA;

        
	   
    
	//break;
        
    

   case UPDATE_TO_PANEL_DATA: //5

    switch(gctl_t.interval_time_stop_run){

	 case 0: //works timing 
	 

	 if(gctl_t.gTimer_ptc_adc_times > 28 ){ //65s//3 minutes 120s
         gctl_t.gTimer_ptc_adc_times=0;
		 
		 Get_Ptc_ADC_Fun(ADC_CHANNEL_1,1);
	     

	 }
     else if(gctl_t.gTimer_fan_adc_times > 45){ //2 minute 180s
	     gctl_t.gTimer_fan_adc_times =0;
	     Get_Fan_ADC_Fun(ADC_CHANNEL_0,1);
	     

		 
	 }
	
	 
	if(gctl_t.gTimer_app_power_on >37 &&	 gctl_t.app_timer_power_on_flag == 1){
		gctl_t.gTimer_app_power_on=0;
		gctl_t.app_timer_power_on_flag++;
		   for(i=0;i<36;i++){
				 TCMQTTRCVPUB[i]=0;
			 }
		}
	
       if(gctl_t.first_link_tencent_cloud_flag ==1 && wifi_link_net_state() ==1){
	
		  gctl_t.first_link_tencent_cloud_flag++;
			MqttData_Publish_SetOpen(0x01);
			HAL_Delay(100);
			Publish_Data_ToTencent_Initial_Data();
			HAL_Delay(350);
	
		   Subscriber_Data_FromCloud_Handler();
		   HAL_Delay(350);
	   }
    

	if(gctl_t.gTimer_continuce_works_time > 7200){//if(gctl_t.gTimer_continuce_works_time > 600){
	
	     gctl_t.gTimer_continuce_works_time =0;
         gctl_t.interval_time_stop_run =1;
	     gctl_t.gFan_continueRun =1;
		 gctl_t.gFan_counter=0;
    }
    
	 break;

	 case 1:  //interval times 10 minutes,stop works
	 	
		PLASMA_SetLow(); //
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
		PTC_SetLow();

	
	  if(gctl_t.gTimer_continuce_works_time > 600){
             gctl_t.gTimer_continuce_works_time=0;
		    gctl_t.interval_time_stop_run =0;
      }

	 if(gctl_t.gFan_continueRun ==1){

	      if(gctl_t.gFan_counter < 60){
	  
	              Fan_One_Power_Off_Speed();//Fan_RunSpeed_Fun();// FAN_CCW_RUN();
	          }       

	       if(gctl_t.gFan_counter > 59){
	           
			   gctl_t.gFan_counter=0;
			
			   gctl_t.gFan_continueRun=0;
			   FAN_Stop();
	       }

	  }
	 

	 break;

     }
     
    


	
    break;

	case POWER_ON_FAN_CONTINUCE_RUN_ONE_MINUTE:
  
	    
	 if(gctl_t.gPower_On ==POWER_ON && gctl_t.gFan_continueRun ==1){

              if(gctl_t.gFan_counter < 60){
          
                      Fan_One_Power_Off_Speed();//Fan_RunSpeed_Fun();// FAN_CCW_RUN();
                  }       

	           if(gctl_t.gFan_counter > 59){
		           
				   gctl_t.gFan_counter=0;
				
				   gctl_t.gFan_continueRun++;
				   FAN_Stop();
	           }

	 }


	break;

   }
}

void power_off_handler(void)
{
        if(gctl_t.fan_continuce==0){

        gctl_t.gPower_On=POWER_OFF;
        gctl_t.gPower_flag = POWER_OFF;
        gctl_t.RunCommand_Label = POWER_OFF;
		 gctl_t.set_wind_speed_value=10;
		 gctl_t.gModel =1;
		gctl_t.app_timer_power_on_flag =0;
		
		gctl_t.interval_time_stop_run =0;
		gctl_t.gTimer_continuce_works_time=0;
		wifi_t.gTimer_subscriber_send=0;

		  gctl_t.ptc_warning =0;
		 gctl_t.fan_warning =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;
		 gctl_t.ptc_first_detected_times=0;
          SetPowerOff_ForDoing();
		
		  gctl_t.fan_continuce ++;
          gctl_t.gFan_counter=0;
         }

       if(wifi_link_net_state() == 1){

          MqttData_Publish_PowerOff_Ref(); 
          osDelay(200);//HAL_Delay(200);

          if( gctl_t.ptc_remove_warning_send_data ==0){
		 	gctl_t.ptc_remove_warning_send_data++;
		  	Publish_Data_Warning(ptc_temp_warning,0);
		  	osDelay(200);
			Publish_Data_Warning(fan_warning,0);
			osDelay(200);
			
          }

           Subscriber_Data_FromCloud_Handler();
		  osDelay(200);
		 


       }

       

	   
		  if(gctl_t.gFan_counter < 60 && gctl_t.fan_continuce==1){
          
                   
			Fan_One_Power_Off_Speed();
                  
           }       
           else{
		          gctl_t.fan_continuce =5;
				  
				   FAN_Stop();
                  
				  
	         }
	  
}
/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
    static uint8_t self_power_on_flag=0,send_power_off_flag=0;
    

	if(self_power_on_flag==0){
        self_power_on_flag ++ ;
	
        buzzer_sound();
    
		InitWifiModule_Hardware();//InitWifiModule();
		HAL_Delay(1000);
        SmartPhone_TryToLink_TencentCloud();
         gctl_t.gTimer_ptc_adc_times=0;
		if(esp8266data.esp8266_login_cloud_success==1){
			
			
			wifi_t.runCommand_order_lable= wifi_tencent_subscription_data;//04
	

			SendWifiData_To_Cmd(0x20,0x01) ;
            HAL_Delay(5);
			

			
		}
       gctl_t.gTimer_ptc_adc_times=0;
    }

	 if(esp8266data.esp8266_login_cloud_success==1 && gctl_t.gPower_On  !=POWER_ON ){
       
           if(send_power_off_flag==0){
            send_power_off_flag++;
		    //gctl_t.RunCommand_Label=POWER_OFF;
		    gctl_t.rx_command_tag= POWER_OFF;
			//wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data
			SendWifiData_To_Cmd(0x20,0x01) ;
			HAL_Delay(50);
               
           }
   			
	}
    
   
}
void main_function_detected_handler(void)
{

if(gctl_t.gTimer_senddata_panel >0 ){ //300ms
         gctl_t.gTimer_senddata_panel=0;
           ActionEvent_Handler();
     }


}

    
