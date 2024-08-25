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
     if(esp8266data.esp8266_login_cloud_success ==1) return 1;
     else 
        return 0;


}



