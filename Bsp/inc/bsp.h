#ifndef __BSP_H
#define  __BSP_H
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bsp_fan.h"

#include "bsp_ctl.h"
#include "bsp_delay.h"

#include "bsp_cmd_link.h"

#include "interrupt_manager.h"
#include "bsp_subscription.h"
#include "bsp_wifi_fun.h"
#include "bsp_wifi.h"
#include "bsp_buzzer.h"
#include "bsp_adc.h"
#include "bsp_freertos.h"


#include "bsp_wifi_fun.h"
#include "bsp_dht11.h"
#include "bsp_fan.h"
#include "bsp_ultrasonic.h"

#include "bsp_special_power.h"

#include "bsp_esp8266.h"
#include "bsp_mqtt_iot.h"
#include "bsp_publish.h"
#include "bsp_flash.h"


#include "tim.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include "dma.h"





#define WIFI_RX_NUMBERS         1


#define  USE_FreeRTOS      1


#if USE_FreeRTOS == 1
	#include "FreeRTOS.h"
    #include "task.h"
    #include "cmsis_os.h"
	#define DISABLE_INT()    taskENTER_CRITICAL()
	#define ENABLE_INT()     taskEXIT_CRITICAL()
#else
	/* ����ȫ���жϵĺ� */
	#define ENABLE_INT()	__set_PRIMASK(0)	/* ʹ��ȫ���ж� */
	#define DISABLE_INT()	__set_PRIMASK(1)	/* ��ֹȫ���ж� */
#endif


extern uint8_t wifi_rx_inputBuf[WIFI_RX_NUMBERS];



typedef enum _power_state{

  power_off,
  power_on,


}power_state;


typedef struct PROCESS_T{


   uint8_t gpower_on;
   uint8_t disp_rx_cmd_done_flag;
   uint8_t wifi_rx_data_array[150];
   uint8_t wifi_rx_data_counter;
  
   uint8_t wifi_led_fast_blink_flag;
   uint8_t get_beijing_time_success;
   
   uint8_t disp_works_hours ;    
   uint8_t disp_works_minutes ;
   uint8_t disp_works_time_seconds ;

   uint8_t gTimer_power_on_first_link_tencent;
   uint8_t gTimer_get_data_from_tencent_data;



}process_t;

extern process_t gpro_t;


void bsp_init(void);



void receive_data_fromm_display(uint8_t *pdata);



uint8_t bcc_check(const unsigned char *data, int len);

void send_data_to_disp(void);

void wifi_get_beijint_time_handler(void);


void adc_detected_hundler(void);


void wifi_auto_detected_link_state(void);


#endif 

