#include "bsp.h"


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
   static uint8_t timer_fan_flag,times_flag;

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

	  #if TEST_UNIT 
	 if(check_time  > 2){ //10
           
             check_time=0;
             gctl_t.gTimer_fan_adc_times =0; //ADC be detected must be run 60s,after be detected ADC
		     stopHours_flag=0;
             gpro_t.stopTwoHours_flag=0;
             ActionEvent_Handler();
            
      }
     #else 

      if(check_time  > 10){ //10
               
         check_time=0;
         gctl_t.gTimer_fan_adc_times =0; //ADC be detected must be run 60s,after be detected ADC
         stopHours_flag=0;
         gpro_t.stopTwoHours_flag=0;
         ActionEvent_Handler();
                
        }

      #endif 

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

      if(gctl_t.gTimer_senddata_panel >5 ){ //300ms
          gctl_t.gTimer_senddata_panel=0;
          times_flag ++;
           if(times_flag > 2){
               times_flag =0;
                  Update_DHT11_Value();
           }
	 
      }
   
     }
     else{

        if(gctl_t.gTimer_senddata_panel >5 ){ //300ms
             gctl_t.gTimer_senddata_panel=0;
             times_flag ++;
              ActionEvent_Handler();

              if(times_flag > 2){
                  times_flag =0;
                  Update_DHT11_Value();
              }
         }
    

    }
}





