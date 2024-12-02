#include "bsp.h"


_run_t gctl_t; 
uint8_t powerOffFanRun_flag ;
uint8_t powerOffTunrOff_flag;
uint8_t gTimer_powerOffRunFan;
uint8_t stopHours_flag;
uint8_t g_dry_open_flag;

uint8_t timer_fan_flag;

uint8_t warning_array[2];

uint8_t g_plasma[1];
uint8_t g_ultra[1];

void power_off_stop_fun(void);


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

         
         gctl_t.gTImer_send_data_to_disp=0; //temp and humidity data of times
         
	     gctl_t.gTimer_senddata_panel=0; //main board function run action.
		 gctl_t.set_temperature_value=40; //WT.EDIT 2024.11.30
		 save_set_temp[0] =40;           //WT.EDIT 2024.12.01

		 //error detected times 
		 gctl_t.ptc_warning =0;
		 warning_array[1] =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;
		
		 gctl_t.set_wind_speed_value= 100;
        //POWER OFF REF 
     
        powerOffTunrOff_flag = 1;
        powerOffFanRun_flag =1;
        //
        gctl_t.first_link_tencent_cloud_flag=1;
        check_time=0;
        gpro_t.stopTwoHours_flag =0;
        stopHours_flag =0;
        gpro_t.gTimer_detect_fan_error=0;

        disp_seconds=0;
        disp_minutes=0;
        disp_seconds = 0;
      
       
         Update_DHT11_Value();
	    gpro_t.process_run_step= UPDATE_TO_PANEL_DATA;

        every_power_on_run();
        if(wifi_link_net_state() ==1){

          Update_Dht11_Totencent_Value();
          osDelay(20);//HAL_Delay(200) //WT.EDIT 2024.08.10
        }
	   
    
	break;
        
    case UPDATE_TO_PANEL_DATA: //5

  
	if(gpro_t.wifi_led_fast_blink_flag==0){
    if(gctl_t.first_link_tencent_cloud_flag ==1 && wifi_link_net_state() ==1 && gctl_t.app_timer_power_on_flag==0){
	
		  gctl_t.first_link_tencent_cloud_flag++;

            
            

			Publish_Data_ToTencent_Initial_Data();
			HAL_Delay(200);

            MqttData_Publish_SetOpen(0x01);
			HAL_Delay(100);

            Subscriber_Data_FromCloud_Handler();
    		HAL_Delay(100);//HAL_Delay(350);

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
   //static uint8_t timer_fan_flag;

   if(stopHours_flag ==1){

    stopHours_flag++;
   
    check_time=0;
    PLASMA_SetLow(); //
    HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
     gctl_t.gTimer_fan_run_one_minute=0;
     gpro_t.stopTwoHours_flag = 1;
     timer_fan_flag=1;


    }

  
    
    if(gpro_t.stopTwoHours_flag ==1){

	

      if(check_time  > 10){ //10
               
         check_time=0;
         gctl_t.gTimer_fan_adc_times =0; //ADC be detected must be run 60s,after be detected ADC
         stopHours_flag=0;
         gpro_t.stopTwoHours_flag=0;
         ActionEvent_Handler();
        
                
        }

    

	 if(timer_fan_flag ==1){

	      if(gctl_t.gTimer_fan_run_one_minute < 60){
	  
	              Fan_One_Power_Off_Speed();//Fan_RunSpeed_Fun();// FAN_CCW_RUN();
	          }       

	       if(gctl_t.gTimer_fan_run_one_minute > 59){
	           
			   gctl_t.gTimer_fan_run_one_minute=0;
			
			  timer_fan_flag=0;
              gctl_t.fan_stop_flag = 1;
			   FAN_Stop();
	       }

	  }
	 

   
     }
     else{

        if(gctl_t.gTimer_senddata_panel >5 ){ //300ms
             gctl_t.gTimer_senddata_panel=0;
               ActionEvent_Handler();
              compare_temp_value();
         }
    

    }
 
}
/**********************************************************************
    *
    *Functin Name: void power_off_handler(void)
    *Function : 
    *Input Ref:  key of value
    *Return Ref: NO
    *
************************************************************************/
void power_off_handler(void)
{

   // static uint8_t fan_run_one_minute_flag;


      if(powerOffTunrOff_flag==1){

          powerOffTunrOff_flag++;
          gTimer_powerOffRunFan=0;
          gctl_t.gTimer_fan_run_one_minute=0;
       
      
          gctl_t.set_wind_speed_value=10;
		 gctl_t.gModel =1;
		gctl_t.app_timer_power_on_flag =0;
		
		stopHours_flag =0;
	    check_time =0;
        
	    gpro_t.stopTwoHours_flag=0;

		  gctl_t.ptc_warning =0;
		 warning_array[1] =0;
		 gctl_t.gTimer_ptc_adc_times=0;
		 gctl_t.gTimer_fan_adc_times=0;

         
		
          SetPowerOff_ForDoing();
		
         }
        
      /**************************************************************/
       if(wifi_link_net_state() == 1 &&    powerOffTunrOff_flag== 2){

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
		 
           powerOffTunrOff_flag++;


       }

       
        gpro_t.process_run_step=0;//gpro_t.process_run_step
	   
		if(gTimer_powerOffRunFan < 60 && powerOffFanRun_flag ==1){
          
                   
			Fan_One_Power_Off_Speed();
                  
        }       
        else if(gTimer_powerOffRunFan > 59   ){ //WT.EDTI 2024.11.19
		   
			       powerOffFanRun_flag=2;
				   FAN_Stop();
         }

        if(gTimer_powerOffRunFan > 61){
              gTimer_powerOffRunFan =0;

              powerOffFanRun_flag=2;
              gctl_t.fan_stop_flag =1;
              FAN_Stop();


        }
        gpro_t.stopTwoHours_flag =0;
        check_time=0;
        power_off_stop_fun();

      
	  
}

/**********************************************************************
    *
    *Functin Name: void main_function_detected_handler(uint8_t cmd)
    *Function : 
    *Input Ref:  key of value
    *Return Ref: NO
    *
************************************************************************/
void main_function_detected_handler(uint8_t cmd)
{

if(gctl_t.gTimer_senddata_panel >1  &&  cmd ==0){ //300ms
         gctl_t.gTimer_senddata_panel=0;
         
           ActionEvent_Handler();
     }


}


void power_off_stop_fun(void)
{
      
      PLASMA_SetLow(); //
      HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
      PTC_SetLow();
      

}


void compare_temp_value(void)
{

  static uint8_t default_tem_value ;

  if(gctl_t.app_timer_power_on_flag > 1)gctl_t.app_timer_power_on_flag =0;
  if(gctl_t.ptc_warning  >0) gctl_t.ptc_warning =0;

  
  if(save_set_temp[0] ==   gctl_t.set_temperature_value ){
           
     if(gctl_t.set_temperature_value==40)default_tem_value=39;


      if(gctl_t.gDht11_temperature >  default_tem_value){
                 PTC_SetLow();
                g_dry_open_flag = 0;
                gctl_t.gDry=0;
               if(gpro_t.wifi_led_fast_blink_flag==0){
                SendWifiData_To_Cmd(0x02, 0);
                }

      }
      else if(gctl_t.gDht11_temperature >  gctl_t.set_temperature_value){
                        //ptc off
               PTC_SetLow();
               g_dry_open_flag = 0;
               gctl_t.gDry=0;
              if(gpro_t.wifi_led_fast_blink_flag==0){
               SendWifiData_To_Cmd(0x02, 0);
               }

       }
  }
  if((gctl_t.gDht11_temperature <   save_set_temp[0]  || gctl_t.gDht11_temperature==save_set_temp[0])
                                     && gctl_t.app_timer_power_on_flag==0 && gctl_t.ptc_warning ==0 && warning_array[1]==0) {

              PTC_SetHigh();       //PTC ON
              g_dry_open_flag = 1;
               gctl_t.gDry=1;
              if(gpro_t.wifi_led_fast_blink_flag==0){
              SendWifiData_To_Cmd(0x02,0x01);

              }

   }
}




    
