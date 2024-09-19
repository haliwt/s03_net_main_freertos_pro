#include "bsp.h"

process_t gpro_t;


static void Auto_InitWifiModule_Hardware(void);
static void Auto_SmartPhone_TryToLink_TencentCloud(void);


 

void bsp_init(void)
{
   delay_init(64);
    dht11_init();
   buzzer_init();
   wifi_init();

}

uint8_t  beijing_step;

uint8_t real_hours,real_minutes,real_seconds;

uint8_t auto_link_net_flag;

uint8_t power_on_login_tencent_cloud_flag  ;


void link_wifi_to_tencent_handler(uint8_t data)
{

    
    if(data == 1){//if(gpro_t.wifi_led_fast_blink_flag==1){
        if(gctl_t.gTimer_linking_tencen_total_counter  > 119){

           gpro_t.wifi_led_fast_blink_flag =0;//gpro_t.wifi_led_fast_blink_flag =0;
           if(wifi_link_net_state()==0){

              gpro_t.get_beijing_flag = 10;
              wifi_t.gTimer_auto_detected_net_state_times = 120;
               net_t.linking_tencent_cloud_doing =1;
               wifi_t.soft_ap_config_flag =1; //WE.EIDT 

           }

        }
        else
        link_wifi_net_handler();
        

    }
    else{
       send_data_to_disp();
       adc_detected_hundler();
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
    *Function Name:void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
    *Function: receive usart touchkey of command 
    *Input Ref:NO
    *Return Ref:NO
    *
**********************************************************************/
void receive_data_fromm_display(uint8_t *pdata)
{

   if(pdata[1] == 0x01){

    switch(pdata[2]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[3] == 0x01){ //open
           buzzer_sound_fun();
           

           gpro_t.gpower_on = power_on;

        }
        else if(pdata[3] == 0x0){ //close 
           buzzer_sound();
           gpro_t.gpower_on = power_off;


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[3] == 0x01){
          buzzer_sound();
        

       if(gctl_t.interval_time_two_hours_stop_flag ==0){
          gctl_t.gDry = 1;
          Dry_Function(0);
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
       }
       }
       else if(pdata[3] == 0x0){
          buzzer_sound();
          
       
         gctl_t.gDry =0;
         Dry_Function(1);
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
          }

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[3] == 0x01){
           
            buzzer_sound();
           
           gctl_t.gPlasma = 1;
          
           PLASMA_SetHigh();
        }
        else if(pdata[3] == 0x0){
           buzzer_sound();
           
          gctl_t.gPlasma = 0;
        
          PLASMA_SetLow();

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[3] == 0x01){  //open 
          
           gctl_t.gUlransonic =1;

        }
        else if(pdata[3] == 0x0){ //close 

           gctl_t.gUlransonic = 0;

        }


     break;

      case 0x05: // link wifi command

       if(pdata[3] == 0x01){  // link wifi 
           buzzer_sound();
           gpro_t.link_net_step =0;
	      net_t.wifi_link_net_success=0;
          gpro_t.wifi_led_fast_blink_flag =1;
          gctl_t.wifi_config_net_lable=wifi_set_restor;
		  wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 
		  
          gctl_t.gTimer_linking_tencen_total_counter=0; //total times is 120s

        }
        else if(pdata[3] == 0x0){ //don't link wifi 

        }


     break;

     case 0x06: //buzzer sound done

        if(pdata[3] == 0x01){  //buzzer sound 
             //buzzer_sound();
            gpro_t.buzzer_sound_flag = 1;

        }
        else if(pdata[3] == 0x0){ // don't buzzer sound .



        }


     break;


      case 0x1A: //温度数据

        if(pdata[3] == 0x0F){ //数据

            gctl_t.set_temperature_value = pdata[5] ;

          MqttData_Publis_SetTemp(gctl_t.set_temperature_value);
		  osDelay(20);//HAL_Delay(350);

        }
      break;

      case 0x1B: //湿度数据

        if(pdata[3] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[3] == 0x0F){ //数据

          


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[3] == 0x0F){ //数据

             
            

        }
      break;

     case 0x22: //PTC打开关闭指令,没有蜂鸣器声音。

      if(pdata[3] == 0x01){
        
        if(gctl_t.interval_time_two_hours_stop_flag ==0){
          gctl_t.gDry = 1;
          Dry_Function(0);
         if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x01);
	  	      osDelay(100);//HAL_Delay(350);
           }
          
         }
       }
       else if(pdata[3] == 0x0){
        
         gctl_t.gDry =0;
         Dry_Function(1);
          if(wifi_link_net_state()==1){
              MqttData_Publish_SetPtc(0x0);
	  	      osDelay(100);//HAL_Delay(350);
           }

       }

     break;

     case 0x27:

      if(pdata[3] == 0x02){
       
         gctl_t.gModel=2;
         MqttData_Publish_SetState(2);
	     osDelay(100);//HAL_Delay(350);
        
        
          
       }
       else if(pdata[3] == 0x01){ //AI mode 
       
         gctl_t.gModel=1;
         MqttData_Publish_SetState(1);
	     osDelay(100);//HAL_Delay(350);
        
        

       }


     break;
        
     
     }

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
	*Functin Name:void void wifi_get_beijing_time_handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void wifi_get_beijing_time_handler(void)
{

    static uint8_t alternate_flag,flag_switch;

     if(wifi_t.get_rx_beijing_time_enable==0){
    
         Tencent_Cloud_Rx_Handler();
    	 Json_Parse_Command_Fun();
  
      }
    switch(gpro_t.get_beijing_flag){

     case 0: //WT.EDIT .2024.08.10

      if(wifi_link_net_state()==1 && gpro_t.gTimer_get_data_from_tencent_data > 9){
       
          gpro_t.gTimer_get_data_from_tencent_data =0;
           flag_switch++;

       

         if(flag_switch == 1){
            wifi_t.get_rx_beijing_time_enable=0;
            Subscriber_Data_FromCloud_Handler();
            osDelay(100);//HAL_Delay(200)
            gpro_t.get_beijing_flag = 1;

            
         }
        else if(flag_switch >  1 && gpro_t.gpower_on == power_off){
            flag_switch=0;
            wifi_t.get_rx_beijing_time_enable=0;
            Update_Dht11_Totencent_Value();
            osDelay(50);//HAL_Delay(200) //WT.EDIT 2024.08.10
            
             gpro_t.get_beijing_flag = 1;

         }
         else{ //WT.EDIT 2024.08.10 ADD ITEM
             if(flag_switch > 1){
                flag_switch=0;
                wifi_t.get_rx_beijing_time_enable=0; 
               gpro_t.get_beijing_flag = 1;
             }

         }
       
    }
    else
     gpro_t.get_beijing_flag = 1;

   break;


   case 1:

        if(wifi_t.gTimer_get_beijing_time > 100){

         wifi_t.gTimer_get_beijing_time=0;
        
          if(wifi_link_net_state()==1){

     
    		    gpro_t.get_beijing_flag = 2;
           
                alternate_flag++;
                net_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
                SendWifiData_To_Data(0x1F,0x01);
                //gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .

            }
            else{
              
               SendWifiData_To_Data(0x1F,0x0); //0x1F: 0x1=wifi link net is succes ,0x0 = wifi link net is fail
               gpro_t.get_beijing_flag = 10;
               net_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
             }
	
       }
       else{

            gpro_t.get_beijing_flag = 0;
       }

      break;


      case 2:


	    if(gpro_t.get_beijing_time_success == 0){
		

         gpro_t.get_beijing_flag = 3;
         beijing_step =0; //WT.EDIT 2024.08.10
	      
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
	
       }
	   else{

          
         if(alternate_flag == 1){
		  
	        wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		    gpro_t.get_beijing_flag = 3;
			beijing_step =0; //WT.EDIT 2024.08.10

        }
		else{
              alternate_flag=0;
             
			  gpro_t.get_beijing_flag = 6;

		}

	   }

     
    break;


     case 3:

         switch(beijing_step){

         case 0:
            //disable publish data to tencent cloud.
            gpro_t.gTimer_get_data_from_tencent_data=0;

    		
    		Get_BeiJing_Time_Cmd();
    	    osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10//HAL_Delay(20);
    		//wifi_t.gTimer_read_beijing_time=0;
    
            
            beijing_step =1;

           

         break;

         case 1:
             //if(wifi_t.gTimer_read_beijing_time > 0 ){//
    		
                wifi_t.gTimer_read_beijing_time=0;
                 //disable publish data to tencent cloud.
                gpro_t.gTimer_get_data_from_tencent_data=0;
                wifi_t.get_rx_beijing_time_enable=1;
        		gpro_t.wifi_rx_data_counter =0;
        		Get_Beijing_Time();
              
        	     osDelay(100);//HAL_Delay(20); //WT.EDIT .2024.08.10
                
        		wifi_t.gTimer_read_beijing_time=0;
                beijing_step =2;
          
          //  }

          break; 


          case 2:

            if(gpro_t.wifi_rx_data_array[50] > 0x31 ){
           
                real_hours = (gpro_t.wifi_rx_data_array[41]-0x30)*10 + gpro_t.wifi_rx_data_array[42]-0x30;
                real_minutes =(gpro_t.wifi_rx_data_array[44]-0x30)*10 + gpro_t.wifi_rx_data_array[45]-0x30;
                real_seconds = (gpro_t.wifi_rx_data_array[47]-0x30)*10 + gpro_t.wifi_rx_data_array[48]-0x30;

                wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
                gpro_t.disp_works_hours = real_hours;    
                    gpro_t.disp_works_minutes = real_minutes;

                    gpro_t.disp_works_time_seconds = real_seconds;
                    gpro_t.get_beijing_time_success = 1;

                    SendWifiData_To_PanelTime(gpro_t.disp_works_hours,gpro_t.disp_works_minutes,gpro_t.disp_works_time_seconds);
                    osDelay(50);

                    gpro_t.get_beijing_flag = 0;
                
                }
                else if(gpro_t.wifi_rx_data_array[50] == 0x31){  //"0x31" ASCII = '1'

                   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
                   if(wifi_link_net_state()==1){
                       gpro_t.get_beijing_flag = 0;
                       gpro_t.gTimer_get_data_from_tencent_data=0;
                       wifi_t.gTimer_get_beijing_time = 50;
                      

                    }
                    else 
                       gpro_t.get_beijing_flag = 0;
            }
            else{

              gpro_t.get_beijing_flag = 0;
              

            }
            break;
         }
    break;


    case 6:

  
       Wifi_Link_Tencent_Net_State();
    

       wifi_t.gTimer_auto_detected_net_state_times=0;  

       gpro_t.get_beijing_flag = 0;
    
    break;

    //auto link net 

    case 10:

     if(wifi_t.gTimer_auto_detected_net_state_times > 100){

		wifi_t.gTimer_auto_detected_net_state_times=0;

    

         if(wifi_link_net_state()==0){
              gpro_t.get_beijing_flag = 11;
           
          
           
            net_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
            gpro_t.wifi_rx_data_counter=0;
           
      
           
          }
          else{
             wifi_t.soft_ap_config_flag =1; //WE.EIDT 
             net_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
             gpro_t.get_beijing_flag = 0;

          }
        
       }

     
     if(wifi_link_net_state()==1){
          gpro_t.get_beijing_flag = 0;

       }
       
     break;


     case 11:
         if(wifi_link_net_state()==0 && gpro_t.wifi_led_fast_blink_flag==0){

           net_t.linking_tencent_cloud_doing =1;
        

            WIFI_IC_ENABLE();
       
    		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
            HAL_Delay(1000);
            wifi_t.gTimer_auto_link_net_time =0;
            auto_link_net_flag=1;

            gpro_t.get_beijing_flag = 12;

         }
         else {

             gpro_t.get_beijing_flag = 10;


         }
        
               

     break;


     case 12:

        if(wifi_t.gTimer_auto_link_net_time > 2 && auto_link_net_flag==1){


            wifi_t.gTimer_auto_link_net_time=0;
            gpro_t.wifi_rx_data_counter=0;
	        wifi_t.soft_ap_config_flag =1;
             auto_link_net_flag =2;
           // DISABLE_INT();
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
            HAL_Delay(1000);
           // HAL_Delay(1000);
           // HAL_Delay(1000);
           
            //HAL_Delay(1000);
           
		
	    }

        if(wifi_t.gTimer_auto_link_net_time > 4 && auto_link_net_flag==2){

            auto_link_net_flag=0 ;
            gpro_t.get_beijing_flag = 13;
           
        }


     break;


     case 13:
       if(wifi_link_net_state()==1){
       
         

        net_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
	    gpro_t.wifi_rx_data_counter=0; //clear USART2 counter is zero
		wifi_t.soft_ap_config_flag =0; 
	

          if(gpro_t.gpower_on == power_on){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                HAL_Delay(200);

            }
            else if(gpro_t.gpower_on == power_off){

               MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);


            }
            Subscriber_Data_FromCloud_Handler();
            HAL_Delay(200);

            SendWifiData_To_Data(0x1F,0x01); //0x1F: wifi link net is succes 

             gpro_t.get_beijing_flag = 0;
		
         }
         else{

             gpro_t.get_beijing_flag = 10;
         }

       break;
	

	
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
   

   if(gctl_t.gTimer_ptc_adc_times > 5 && gctl_t.interval_time_two_hours_stop_flag ==0){ //65s//3 minutes 120s
        gctl_t.gTimer_ptc_adc_times=0;
        
       Get_Ptc_ADC_Fun(1,10);
        
        

    }
    if(gctl_t.gTimer_fan_adc_times > 60 && gctl_t.interval_time_two_hours_stop_flag ==0){ //2 minute 180s
        gctl_t.gTimer_fan_adc_times =0;
        Get_Fan_ADC_Fun(ADC_CHANNEL_0,10);
        
    }

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
  
	WIFI_IC_ENABLE();
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



