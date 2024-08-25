#include "bsp.h"

void (*buzzer_sound)(void);

static void buzzer_fun(void)


void buzzer_init(void)
{

    buzzer_sound_handler(buzzer_fun);

}




void buzzer_sound_handler(void(*buzzer_hanlder)(void))
{
     buzzer_sound = buzzer_handler;

}





static void buzzer_fun(void)
{
   
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    osDelay(20);
    HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);



}


