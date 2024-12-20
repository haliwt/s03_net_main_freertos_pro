#include "bsp.h"

void (*buzzer_sound)(void);




void buzzer_init(void)
{

    buzzer_sound_handler(buzzer_sound_fun);

}




void buzzer_sound_handler(void(*buzzer_handler)(void))
{
     buzzer_sound = buzzer_handler;

}





void buzzer_sound_fun(void)
{
   
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    osDelay(20);
    HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);



}


