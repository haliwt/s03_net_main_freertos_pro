#include "bsp.h"


void (*Single_Usart_ReceiveData)(uint8_t cmd);

void SetPowerOn_ForDoing(void)
{
    
  switch(gctl_t.app_timer_power_on_flag){
		case 0:
        gctl_t.gModel=1;
	    gctl_t.gFan = 1;
		gctl_t.gDry = 1;
		gctl_t.gPlasma =1;       //"杀菌"
		gctl_t.gUlransonic = 1; // "驱虫"
	    gctl_t.gFan_counter=0;

         if(wifi_link_net_state() ==1){
    
		   MqttData_Publish_SetOpen(1);  
			HAL_Delay(200);
		     Update_DHT11_Value();
			 HAL_Delay(200);
	         gctl_t.set_wind_speed_value =100;
			gctl_t.wifi_gPower_On=1;
			MqttData_Publish_Update_Data();
			 HAL_Delay(200);

         }
       	
			
	    Fan_RunSpeed_Fun();//FAN_CCW_RUN();
	    PLASMA_SetHigh(); //
	    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	    PTC_SetHigh();
  
	break;

	case 1: //app timer timing power of 
	       gctl_t.gModel =1;
		  

	       Parse_Json_Statement();

	  
		    if( gctl_t.gPlasma==1){ //Anion
				gctl_t.gPlasma=1;

				SendWifiData_To_Cmd(0x03,0x01);
				HAL_Delay(2);
			}
			else{
				gctl_t.gPlasma =0;
				SendWifiData_To_Cmd(0x03,0x0);
				HAL_Delay(2);
			}


			if(gctl_t.gUlransonic==1){

					SendWifiData_To_Cmd(0x04,0x01);
					HAL_Delay(2);
			}
			else {
					gctl_t.gUlransonic=0;
					SendWifiData_To_Cmd(0x04,0x0);
					HAL_Delay(2);
			}



			if(gctl_t.gDry==1){

				SendWifiData_To_Cmd(0x02,0x01);
				HAL_Delay(2);
			}
			else{
					gctl_t.gDry=0;
					SendWifiData_To_Cmd(0x02,0x0);
					HAL_Delay(2);

			}
		 
			 HAL_Delay(100);

		     gctl_t.set_wind_speed_value =100;
			 gctl_t.wifi_gPower_On=1;
		     MqttData_Publish_Update_Data();
		     HAL_Delay(200);

			
	     break;
		}
			
        


	
 }

void SetPowerOff_ForDoing(void)
{
   
	gctl_t.gPower_flag = 0; //bool 
	gctl_t.gFan_continueRun =1; //the fan still run 60s
	gctl_t.gPower_On = POWER_OFF;
	gctl_t.wifi_gPower_On = 0;
    gctl_t.set_wind_speed_value =10;
 
    gctl_t.gFan = 0;
    gctl_t.gDry = 0;
	gctl_t.gPlasma =0;       //"杀菌"
	gctl_t.gUlransonic = 0; // "驱虫"
	gctl_t.gModel =1;


    
	PLASMA_SetLow(); //
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off 
	PTC_SetLow();
	FAN_Stop();
	HAL_Delay(10);

}

void Single_Usart_RxData(void(*rxHandler)(uint8_t dat))
{

      Single_Usart_ReceiveData=  rxHandler;

}

void ActionEvent_Handler(void)
{
     

if(gctl_t.works_break_power_on==0) { 

    
	if(gctl_t.gDry == 1 && gctl_t.ptc_warning ==0){

	   PTC_SetHigh();

	}
	else{
		   PTC_SetLow();
		  
		   
	}
	//kill
	if(gctl_t.gPlasma == 1){
		
	     PLASMA_SetHigh();
	}
	else{

		PLASMA_SetLow();
	}
	//driver bug
	if(gctl_t.gUlransonic ==1){
	
	 
		HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic ON 
	}
	else{
	  HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic off

	}

	if(gctl_t.gPlasma ==0 && gctl_t.gDry==0){

        gctl_t.gFan_counter=0;
		gctl_t.gFan_continueRun=1;        

	}

	Fan_RunSpeed_Fun();
		
   }


}


