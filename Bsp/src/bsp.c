#include "bsp.h"

process_t gpro_t;

static uint8_t bcc_check(const unsigned char *data, int len); 


void bsp_init(void)
{
   buzzer_init();
   wifi_init();

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
void receive_data_fromm_display(uint8_t *pdata,uint8_t len)
{
    uint8_t check_code;
 
  check_code =  bcc_check(pdata, len) ;

  if(check_code == pdata[len-1]){

   if(pdata[0] == 0x01){

    switch(pdata[1]){

     case 0:


     break;

     case 0x01: //表示开机指令

        if(pdata[2] == 0x01){ //open
           buzzer_sound();

           gpro_t.gpower_on = power_on;

        }
        else if(pdata[2] == 0x0){ //close 
           buzzer_sound();
           gpro_t.gpower_on = power_off;


        }

     break;

     case 0x02: //PTC打开关闭指令

     if(pdata[2] == 0x01){
        
          gctl_t.gDry = 1;
       }
       else if(pdata[2] == 0x0){
        
         gctl_t.gDry =0;

       }

     break;

     case 0x03: //PLASMA 打开关闭指令

        if(pdata[2] == 0x01){
           

           gctl_t.gPlasma = 1;
        }
        else if(pdata[2] == 0x0){

          gctl_t.gPlasma = 0;

        }


     break;


      case 0x04: //ultrasonic  打开关闭指令

        if(pdata[2] == 0x01){  //open 
          
           gctl_t.gUlransonic =1;

        }
        else if(pdata[2] == 0x0){ //close 

           gctl_t.gUlransonic = 0;

        }


     break;

      case 0x05: // link wifi command

        if(pdata[2] == 0x01){  // link wifi 

              buzzer_sound();
		      esp8266data.esp8266_login_cloud_success=0;
	          gctl_t.wifi_config_net_lable=wifi_set_restor;
			  gctl_t.gTimer_linking_tencen_counter=0;
              gctl_t.wifi_run_set_restart_flag =1;
	          wifi_t.runCommand_order_lable= wifi_link_tencent_cloud;//2 

        }
        else if(pdata[2] == 0x0){ //don't link wifi 



        }


     break;

     case 0x06: //buzzer sound done

        if(pdata[2] == 0x01){  //buzzer sound 


        }
        else if(pdata[2] == 0x0){ // don't buzzer sound .



        }


     break;


      case 0x1A: //温度数据

        if(pdata[2] == 0x0F){ //数据

        

        }
      break;

      case 0x1B: //湿度数据

        if(pdata[2] == 0x0F){ //数据
            

        }
      break;

      case 0x1C: //表示时间：小时，分，秒

        if(pdata[2] == 0x0F){ //数据

           


        }
      break;

        case 0x1D: //表示日期： 年，月，日

        if(pdata[2] == 0x0F){ //数据

             
            

        }
      break;
     
     }

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

