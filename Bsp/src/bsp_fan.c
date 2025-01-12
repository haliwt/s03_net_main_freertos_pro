#include "bsp.h"


static void SetLevel_Fan_PWMA(uint8_t levelval);


void fan_run_fun(void)
{
    FAN_COM_SetLow();
	FAN_RUN_SetHigh();
	

}



void FAN_Stop(void)
{
   FAN_COM_SetLow(); //brake
   FAN_RUN_SetLow();//SetLevel_Fan_PWMA(0);//SetLevel_Fan_PWMA(16);
}

void Fan_One_Power_Off_Speed(void)
{
	
	 fan_run_fun();//SetLevel_Fan_PWMA(10);
	


}


void Fan_One_Speed(void)
{

	 fan_run_fun();//SetLevel_Fan_PWMA(8);


}

void Fan_Two_Speed(void)
{
	 fan_run_fun();//SetLevel_Fan_PWMA(9);

}

 void Fan_Full_Speed(void)
{
    
     fan_run_fun();//SetLevel_Fan_PWMA(10);

}




void ShutDown_AllFunction(void)
{
	
	PLASMA_SetLow();
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	PTC_SetLow();
	FAN_Stop();



}
//"杀毒" 
void ultrasonic_fun(uint8_t sel)
{
    if(sel==0){//open 
		
		
		 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 

	}
	else{ //close

			
		
		HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off
	

	}



}

void Dry_Function(void)
{
 
  switch(dry_open_flag ){

   case 1:

      if(gctl_t.ptc_warning ==0){
  
          PTC_SetHigh();
           
        }
         
      break;
    
      case 0 :
       
            PTC_SetLow();
    
      }
             
      
}


void plasma_fun(uint8_t sel)
{
     if(sel ==0){
        PLASMA_SetHigh();

     }
     else{

        PLASMA_SetLow();
     }

}



void Fan_RunSpeed_Fun(void)
{

   if(gctl_t.set_wind_speed_value < 34 ){
              fan_run_fun();//Fan_One_Speed();
		 }
		 else if(gctl_t.set_wind_speed_value > 33  && gctl_t.set_wind_speed_value < 67 ){

             fan_run_fun();//Fan_Two_Speed();

		 }
		 else if(gctl_t.set_wind_speed_value > 66){

         

		 	fan_run_fun();//Fan_Full_Speed();

          }

      

}




/********************************************************
*
*Function Name:void SetLevel_Fan_PWMA(uint8_t levelval)
*Function: 
*
*
********************************************************/
static void SetLevel_Fan_PWMA(uint8_t levelval)
{
     gctl_t.gFan_pwm_duty_level = levelval;
     FAN_COM_SetLow();
	 //MX_TIM16_Init();
	 ///HAL_TIM_PWM_Start(&htim16,TIM_CHANNEL_1);
	 FAN_RUN_SetHigh();
}


#if 0
void fan_start_fun(void)
{

   SetLevel_Fan_PWMA(10);
   osDelay(100);
   FAN_Stop();
   SetLevel_Fan_PWMA(10);
   osDelay(100);
   FAN_Stop();
   SetLevel_Fan_PWMA(10);
   osDelay(200);


}
#endif 

