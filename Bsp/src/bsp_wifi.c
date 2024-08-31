#include "bsp.h"


uint8_t (*wifi_link_net_state)(void);

static uint8_t wifi_link_net_fun(void);


void wifi_init(void)
{

    wifi_link_net_handler(wifi_link_net_fun);


}



void wifi_link_net_handler(uint8_t (wifi_link_net_state_handler)(void))
{

   wifi_link_net_state = wifi_link_net_state_handler;

}

static uint8_t wifi_link_net_fun(void)
{
     if(net_t.wifi_link_net_success ==1) return 1;
     else 
        return 0;


}


void Wifi_Link_Tencent_Net_State(void)
{

   HAL_UART_Transmit(&huart2, "AT+TCMQTTSTATE?\r\n", strlen("AT+TCMQTTSTATE?\r\n"), 5000);

   //AT+CWRECONNCFG?
 //  HAL_UART_Transmit(&huart2, "AT+CWRECONNCFG?\r\n", strlen("AT+CWRECONNCFG?\r\n"), 5000);
 
  
}




