#ifndef __BSP_BUZZER_H_
#define __BSP_BUZZER_H_
#include "main.h"

#define BUZZER_PIN      GPIO_PIN_1
#define BUZZER_GPIO     GPIOB

extern void (*buzzer_sound)(void);


void buzzer_init(void);

void buzzer_sound_handler(void(*buzzer_hanlder)(void));

void buzzer_sound_fun(void);



#endif 
