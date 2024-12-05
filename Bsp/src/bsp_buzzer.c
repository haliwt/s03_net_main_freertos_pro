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
    #if 0
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);
    osDelay(20);
    HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
    #endif 
     int8_t  m=80;//80

   // buzzer_gpio_output_init();
	while(m--){

    HAL_GPIO_TogglePin(BEEP_GPIO_Port,BEEP_Pin);//BEEP=!BEEP;
    //delay_us(600);
    delay_us(500);//460//__delay_us(300);;//__delay_us(800);//delayUS(300);

    }

}

void buzzer_gpio_output_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
   
      __HAL_RCC_GPIOB_CLK_ENABLE();
    
    
      /*Configure GPIO pin Output Level */
      HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_RESET);//|TEMP_SENSOR_Pin
    
      /*Configure GPIO pins : PAPin PAPin PAPin */
      GPIO_InitStruct.Pin = BEEP_Pin;
      GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStruct.Pull = GPIO_PULLDOWN;//NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

void buzzer_gpio_input_init(void)
{
GPIO_InitTypeDef GPIO_InitStruct = {0};
    /* GPIO Ports Clock Enable */
      
         __HAL_RCC_GPIOB_CLK_ENABLE();
       
       
         /*Configure GPIO pin Output Level */
        // HAL_GPIO_WritePin(GPIOB, BEEP_Pin, GPIO_PIN_RESET);//|TEMP_SENSOR_Pin
       
         /*Configure GPIO pins : PAPin PAPin PAPin */
         GPIO_InitStruct.Pin = BEEP_Pin;
         GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
         GPIO_InitStruct.Pull = GPIO_PULLDOWN;//NOPULL;
        // GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
         HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



}


