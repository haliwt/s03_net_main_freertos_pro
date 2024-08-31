#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bsp.h"



ESP8266DATATypedef net_t;

 uint8_t *sub_buf;

char *CloudInfo="+TCMQTTCONN:OK";
char *usart2_tx;

uint8_t usart2_flag;

/**
 *pdata: pointer of data for send
 *len:  len of data to be sent
 *return: the len of data send success
 * @brief hal api for at data send
 */
uint8_t at_send_data(uint8_t* pdata, uint16_t len)
{
	if(HAL_OK == HAL_UART_Transmit(&huart2, pdata, len, 10000))
	{
		return len;
	}
	else
	{
		return 0;
	}	
}


/****************************************************************************************************
**
*Function Name:static void initBtleModule(void)
*Function: 
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void InitWifiModule(void)
{
	
	if(gctl_t.wifi_config_net_lable==0){
		 gctl_t.wifi_config_net_lable++;
			
			WIFI_IC_ENABLE();
	
	
			at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
			HAL_Delay(100);
	}
		
}

void InitWifiModule_Hardware(void)
{
	//WIFI_IC_DISABLE();
	//HAL_Delay(1000);
	WIFI_IC_ENABLE();
	at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
	HAL_Delay(1000);
			//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		
}

void ReConnect_Wifi_Net_ATReset_Hardware(void)
{
	    WIFI_IC_DISABLE();
        //osDelay(1000);
        //osDelay(1000);
		HAL_Delay(1000);
		//HAL_Delay(1000);
		//HAL_Delay(1000);
		WIFI_IC_ENABLE();
		//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		//HAL_Delay(1000);
        HAL_Delay(1000);
		//osDelay(1000);
        //osDelay(1000);



}



/****************************************************************************************************
	**
	*Function Name:void Wifi_SoftAP_Config_Handler(void)
	*Function: tensent cloud configuration softAP to connector WIFI
	*Input Ref: 
	*Return Ref:NO
	*
****************************************************************************************************/
void Wifi_SoftAP_Config_Handler(void)
{
     uint8_t *device_massage;
    

    device_massage = (uint8_t *)malloc(128);


   switch (gctl_t.wifi_config_net_lable)
  {

    case wifi_set_restor:
           //InitWifiModule();
           ReConnect_Wifi_Net_ATReset_Hardware();//InitWifiModule_Hardware();
		   //HAL_Delay(1000);
		  // osDelay(1000);
           gctl_t.wifi_config_net_lable =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
        	HAL_Delay(1000);
           
			//HAL_Delay(1000);
           // osDelay(1000);
           /// osDelay(1000);
			//HAL_UART_Transmit(&huart2, "AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 5000);
			gctl_t.wifi_config_net_lable =wifi_set_softap;
			gctl_t.randomName[0]=HAL_GetUIDw0();
		

	 break;

	  case wifi_set_softap:
            WIFI_IC_ENABLE();
			
            sprintf((char *)device_massage, "AT+TCPRDINFOSET=1,\"%s\",\"%s\",\"UYIJIA01-%d\"\r\n", PRODUCT_ID, DEVICE_SECRET,gctl_t.randomName[0]);
			usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
	  		HAL_Delay(1000);
        
           // HAL_Delay(1000);
     
			//HAL_Delay(1000);
			//osDelay(1000);
            //osDelay(1000);
         
	        
			gctl_t.wifi_config_net_lable=wifi_set_tcdevreg;
		
	


	 case wifi_set_tcdevreg://dynamic register
		 HAL_UART_Transmit(&huart2, "AT+TCDEVREG\r\n", strlen("AT+TCDEVREG\r\n"), 0xffff); //动态注册 
	      HAL_Delay(1000);
      
		// HAL_Delay(1000);
     
	     //HAL_Delay(1000);
   
		//HAL_Delay(1000);
		//osDelay(1000);
        //osDelay(1000);
osDelay(1000);
	  
	     gctl_t.wifi_config_net_lable=wifi_set_tcsap;

	 break;


	 case wifi_set_tcsap:
	 
           // HAL_Delay(1000);
          
		   /// HAL_Delay(1000);
    //
			//HAL_Delay(1000);
      
		    ///HAL_Delay(1000);
		    osDelay(1000);
            osDelay(1000);
            osDelay(1000);
            osDelay(1000);
         
            net_t.linking_tencent_cloud_doing =1;
            wifi_t.soft_ap_config_flag =1; //WE.EIDT 
	        sprintf((char *)device_massage, "AT+TCSAP=\"UYIJIA01-%d\"\r\n",gctl_t.randomName[0]);
            usart2_flag = at_send_data(device_massage, strlen((const char *)device_massage));
			 HAL_Delay(1000);
            
            // osDelay(1000);
             ///osDelay(1000);
         
			
		     gctl_t.wifi_config_net_lable=wifi_inquire_register_codes;
			
	case wifi_inquire_register_codes: //0x06//WT.EDIT 2024.07.22
       wifi_t.gTimer_get_beijing_time=0;

         osDelay(1000);
   
       
	   if(net_t.soft_ap_config_success==0){
         
		// net_t.wifi_uart_counter=0;
        HAL_UART_Transmit(&huart2, "AT+TCPRDINFOSET?\r\n", strlen("AT+TCPRDINFOSET?\r\n"), 0xffff); //动
		HAL_Delay(1000);
        gctl_t.wifi_config_net_lable=0xff;//

        }

	 }

  free(device_massage);
}

/****************************************************************************************************
**
*Function Name:void Wifi_Link_SmartPhone_Fun(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
****************************************************************************************************/
void SmartPhone_LinkTencent_Cloud(void)
{
   
    if(net_t.soft_ap_config_success==1){

       net_t.soft_ap_config_success=0;
	   HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
       HAL_Delay(1000);
       HAL_Delay(1000);
       SendWifiData_To_Cmd(0x20,0x01);//To tell display panel wifi be connetor to tencent cloud is success
	 
     }
	
 	// free(device_submassage);

}



void wifi_Disconnect_Fun(void)
{

  HAL_UART_Transmit(&huart2, "AT+TCMQTTDISCONN\r\n", strlen("AT+TCMQTTDISCONN\r\n"), 5000);//开始连接

}

void Get_BeiJing_Time_Cmd(void)
{

  HAL_UART_Transmit(&huart2, "AT+CIPSNTPCFG=1,8,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n", strlen("AT+CIPSNTPCFG=1,800,\"cn.ntp.org.cn\",\"ntp.sjtu.edu.cn\"\r\n"), 5000);//开始连接

}

void Get_Beijing_Time(void)
{

   HAL_UART_Transmit(&huart2, "AT+CIPSNTPTIME?\r\n", strlen("AT+CIPSNTPTIME?\r\n"), 3000);//开始连接


}

void PowerOn_Self_Auto_Link_Tencent_Cloud(void)
{

	 uint8_t *device_massage,auto_link_cloud_flag=0;
    

    device_massage = (uint8_t *)malloc(128);


   switch (auto_link_cloud_flag)
  {

    case 0:
           InitWifiModule_Hardware();//InitWifiModule();
           
		   HAL_Delay(1000);
		   
           auto_link_cloud_flag =wifi_set_cwmode;
	break;


	 case wifi_set_cwmode:
    	    WIFI_IC_ENABLE();
         	HAL_UART_Transmit(&huart2, "AT+CWMODE=3\r\n", strlen("AT+CWMODE=3\r\n"), 5000);
     
            HAL_Delay(1000);
		  //HAL_Delay(1000);
			//HAL_UART_Transmit(&huart2, "AT+CIPMUX=1\r\n", strlen("AT+CIPMUX=1\r\n"), 5000);
		//	auto_link_cloud_flag =wifi_set_softap;
			gctl_t.randomName[0]=HAL_GetUIDw0();
		

	 break;
	 default:
        
	 break;

}
  free(device_massage);

}

void SmartPhone_TryToLink_TencentCloud(void)
{
    net_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
    gpro_t.wifi_counter=0;
	wifi_t.soft_ap_config_flag =0;
    HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接

	HAL_Delay(1000);
    HAL_Delay(1000);
	HAL_Delay(1000);
	
	   
}


void Reconnection_Wifi_Order(void)
{
	HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 0xffff);
    HAL_Delay(200);
  
}


void AutoRepeate_Link_Tencent_Cloud(void)
{
    
	 static uint8_t wifi_en,wifi_det;
	 if(net_t.esp8266_login_cloud_success==0){
	     if( wifi_t.gTimer_reconnect_wifi_order  > 15  && wifi_en ==0){
		   	   wifi_en++;
			 
			 wifi_t.gTimer_reconnect_wifi_order=0;

		  // InitWifiModule_Hardware();//InitWifiModule();
		   ReConnect_Wifi_Net_ATReset_Hardware();
		   HAL_Delay(1000);
		   HAL_Delay(1000);
		    wifi_det=1;
	  
	     }

		if(wifi_t.gTimer_reconnect_wifi_order > 5 && wifi_det==1){
			wifi_det ++;
			wifi_en=0;

			wifi_t.gTimer_reconnect_wifi_order=0;

	        net_t.linking_tencent_cloud_doing =1; //enable usart2 receive wifi  data
	        gpro_t.wifi_counter=0;
		    wifi_t.soft_ap_config_flag =0;
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
		    HAL_Delay(1000);
			HAL_Delay(1000);
			HAL_Delay(1000);
		
		
		 }
	 }
	
   

}








