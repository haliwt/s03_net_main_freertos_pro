#include "bsp.h"


_run_t gctl_t; 


//static void smartphone_app_timer_power_on_handler(void);


/**********************************************************************
	*
	*Functin Name: void Single_ReceiveCmd(uint8_t cmd)
	*Function : resolver is by usart port receive data  from display panle  
	*Input Ref:  usart receive data
	*Return Ref: NO
	*
**********************************************************************/
#if 0
static void Single_Command_ReceiveCmd(uint8_t cmd)
{
    static uint8_t no_buzzer_sound_dry_off;
	switch(cmd){

	    case DRY_ON_NO_BUZZER:

	        gctl_t.noBuzzer_sound_dry_flag =1;

       case DRY_ON:
         gctl_t.gDry = 1;
	      gctl_t.interval_2_hous_fan_one_minute_flag =0;
	   if(gctl_t.noBuzzer_sound_dry_flag !=1){
		     buzzer_sound();
		 }
		if(net_t.wifi_link_net_success==1)
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
			  gctl_t.gTimer_fan_run_one_minute =0;
			   gctl_t.interval_2_hous_fan_one_minute_flag =1;

		     }
			if(net_t.wifi_link_net_success==1)
			MqttData_Publish_SetPtc(0x0);
			HAL_Delay(200);
			
       break;

       case PLASMA_ON:
       		gctl_t.gPlasma=1;
       		gctl_t.gUlransonic =1;
	         buzzer_sound();
	   if(net_t.wifi_link_net_success==1){
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
	   if(net_t.wifi_link_net_success==1){
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
#endif 
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
  // uint8_t i;

    
    switch(gpro_t.process_run_step){

	case 0: //1
	     

         smartphone_timer_power_on_and_normal_handler();

         

         gctl_t.interval_2_hous_fan_one_minute_flag =0;
    
        
         gctl_t.gTImer_send_data_to_disp=0; //temp and humidity data of times
         
	     gctl_t.gTimer_senddata_panel=0; //main board function run action.
		
	    gctl_t.gTimer_continuce_works_time=0;
		 //error detected times 
		 gctl_t.ptc_warning =0;
		 gctl_t.fan_warning =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;
		
		 gctl_t.set_wind_speed_value= 100;

        gctl_t.power_off_ref_value_flag=1;
        gctl_t.first_link_tencent_cloud_flag=1;
      

         Update_DHT11_Value();
	    gpro_t.process_run_step= UPDATE_TO_PANEL_DATA;

        
	   
    
	break;
        
    case UPDATE_TO_PANEL_DATA: //5

  
	 
    if(gctl_t.first_link_tencent_cloud_flag ==1 && wifi_link_net_state() ==1 && gctl_t.app_timer_power_on_flag==0){
	
		  gctl_t.first_link_tencent_cloud_flag++;

            
            MqttData_Publish_SetOpen(0x01);
			HAL_Delay(100);
			Subscriber_Data_FromCloud_Handler();
    		HAL_Delay(100);//HAL_Delay(350);
			Publish_Data_ToTencent_Initial_Data();
			HAL_Delay(200);

             SendWifiData_To_Data(0x1F,0x01);
             osDelay(20);

             Update_DHT11_Value();
              osDelay(20);
	
	  }
      else if(gctl_t.first_link_tencent_cloud_flag ==1 && wifi_link_net_state() ==0){

           gctl_t.first_link_tencent_cloud_flag++;
           Update_DHT11_Value();
           osDelay(20);
      }


     break;
  }
}
      
/**********************************************************************
    *
    *Functin Name: 
    *Function : be check key of value 
    *Input Ref:  key of value
    *Return Ref: NO
    *
************************************************************************/
void works_run_two_hours_state(void)
{

	if(gctl_t.gTimer_continuce_works_time > 119){//if(gctl_t.gTimer_continuce_works_time > 600){
	
	     gctl_t.gTimer_continuce_works_time =0;
         gctl_t.interval_time_two_hours_stop_flag =1;
	     gctl_t.interval_2_hous_fan_one_minute_flag =1;
		 gctl_t.gTimer_fan_run_one_minute=0;
    }
    if(gctl_t.interval_time_two_hours_stop_flag ==1){

	
	 	
		PLASMA_SetLow(); //
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
		PTC_SetLow();

	
	  if(gctl_t.gTimer_continuce_works_time > 10){
             gctl_t.gTimer_continuce_works_time=0;
		    gctl_t.interval_time_two_hours_stop_flag =0;
      }

	 if(gctl_t.interval_2_hous_fan_one_minute_flag ==1){

	      if(gctl_t.gTimer_fan_run_one_minute < 60){
	  
	              Fan_One_Power_Off_Speed();//Fan_RunSpeed_Fun();// FAN_CCW_RUN();
	          }       

	       if(gctl_t.gTimer_fan_run_one_minute > 59){
	           
			   gctl_t.gTimer_fan_run_one_minute=0;
			
			   gctl_t.interval_2_hous_fan_one_minute_flag=0;
			   FAN_Stop();
	       }

	  }
	 

    }

}
 


void power_off_handler(void)
{
        if(gctl_t.power_off_ref_value_flag==1){

       
          gctl_t.set_wind_speed_value=10;
		 gctl_t.gModel =1;
		gctl_t.app_timer_power_on_flag =0;
		
		gctl_t.interval_time_two_hours_stop_flag =0;
		gctl_t.gTimer_continuce_works_time=0;
	

		  gctl_t.ptc_warning =0;
		 gctl_t.fan_warning =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;
		
          SetPowerOff_ForDoing();
		
		  gctl_t.power_off_ref_value_flag ++;
          gctl_t.gTimer_fan_run_one_minute=0;
         }
        
      /**************************************************************/
       if(wifi_link_net_state() == 1 && gctl_t.power_off_ref_value_flag == 2){

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
		 
           gctl_t.power_off_ref_value_flag ++;


       }

       
        gpro_t.process_run_step=0;
	   
		  if(gctl_t.gTimer_fan_run_one_minute < 60 && gctl_t.power_off_ref_value_flag==3){
          
                   
			Fan_One_Power_Off_Speed();
                  
           }       
           else{
		          gctl_t.power_off_ref_value_flag =5;
				  
				   FAN_Stop();
                  
				  
	         }
	  
}

void main_function_detected_handler(uint8_t cmd)
{

if(gctl_t.gTimer_senddata_panel >1  &&  cmd ==0){ //300ms
         gctl_t.gTimer_senddata_panel=0;
           ActionEvent_Handler();
     }


}



    
