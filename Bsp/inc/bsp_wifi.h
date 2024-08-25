#ifndef __BSP_WIFI_H
#define __BSP_WIFI_H
#include "main.h"

#define WIFI_RX_NUMBERS         1

extern uint8_t (*wifi_link_net_state)(void);

extern uint8_t wifi_rx_inputBuf[WIFI_RX_NUMBERS];



void wifi_init(void);


void wifi_link_net_handler(uint8_t (wifi_link_net_state_handler)(void));




#endif 

