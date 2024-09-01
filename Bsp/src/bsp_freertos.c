#include "bsp.h"

#define DECODER_BIT_0        (1<< 0)

/***********************************************************************************************************
											函数声明
***********************************************************************************************************/
//static void vTaskRunPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/* 创建任务通信机制 */
static void AppObjCreate(void);


/***********************************************************************************************************
											变量声明
***********************************************************************************************************/
//static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static QueueHandle_t xQueue1 = NULL;
static QueueHandle_t xQueue2 = NULL;
//static QueueHandle_t xQueue3 = NULL;



///#define QUEUE_LENGTH            1                   /* 队列支持的消息个数 */
///#define QUEUE_ITEM_SIZE         sizeof(uint32_t)    /* 队列中每条消息的大小 */
//#define SEMAPHORE_BINARY_LENGTH 1                   /* 二值信号量的有效长度 */
//#define QUEUESET_LENGTH         ((2 * QUEUE_LENGTH) + SEMAPHORE_BINARY_LENGTH)  /* 队列集支持的消息个数 */



typedef struct Msg
{
	uint8_t  ucMessageID;
	uint8_t usData[12];
	//uint8_t ulData[1];
}MSG_T;

MSG_T   gl_tMsg; /* 定义丢�个结构体用于消息队列 */

uint8_t rx_data_counter,rx_end_flag;

uint8_t  rx_end_counter,uid;

uint8_t check_code;

uint8_t bcc_check_code;


/**********************************************************************************************************
*
*	Function Name:void freeRTOS_Handler(void)
*   Function :
*	
*  
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
     /* 创建任务 */
	  AppTaskCreate();
	  
	  /* 创建任务通信机制 */
//	   AppObjCreate();
	  
	  /* 启动调度，开始执行任劄1�7 */
	   vTaskStartScheduler();


}

/**********************************************************************************************************
*	
*	Function Name:static void vTaskMsgPro(void *pvParameters)
*   Function :
*	
*   
**********************************************************************************************************/
static void vTaskMsgPro(void *pvParameters)
{
	
    static uint8_t power_on_sound_flag ;
    while(1)
    {
      
    if(power_on_sound_flag == 0){
        power_on_sound_flag ++;
        buzzer_sound();

    }
     
    if( gpro_t.gpower_on == power_on){

       if(gctl_t.buzzer_sound_flag == 1){
	 	gctl_t.buzzer_sound_flag = 0;
	    buzzer_sound();

	    }
        
        power_on_handler();
      
        main_function_detected_handler();
        if(gpro_t.wifi_led_fast_blink_flag==1){
            //RunWifi_Command_Handler();
            link_wifi_net_handler(v);

        }
        else{
           send_data_to_disp();
           adc_detected_hundler();

        }
       

      }
      else{

           power_off_handler();
      }

     if(gpro_t.wifi_led_fast_blink_flag==0){
         wifi_get_beijint_time_handler();
         wifi_auto_detected_link_state();
      }
     
    // clear_rx_copy_data();
   
     vTaskDelay(30);
     
    }

}
/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: vTaskStart
*	功能说明: 启动任务，也就是朢�高优先级任务，这里用作按键扫描��1�7
*	彄1�7    叄1�7: pvParameters 是在创建该任务时传��的形参
*	迄1�7 囄1�7 倄1�7: 旄1�7
*   伄1�7 兄1�7 纄1�7: 4  (数��越小优先级越低，这个跟uCOS相反)
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    MSG_T *ptMsg;
	BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(20); /* 1.测试设定的-设置最大等待时间为50ms */
    uint32_t ulValue;

	
    while(1)
    {

           
         xResult = xTaskNotifyWait(0x00000000,      
						           0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          portMAX_DELAY);  /* 最大允许延迟时间,等待时间 */

         if((ulValue & DECODER_BIT_0 ) != 0)
          {
            gpro_t.disp_rx_cmd_done_flag = 0;

           //  bcc_check_code =  gl_tMsg.usData[7];

            check_code =  bcc_check(gl_tMsg.usData,uid);

           

           if(check_code == bcc_check_code ){
           
              receive_data_fromm_display(gl_tMsg.usData);
            }
            

         }
       
      
		
         }
}
/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppTaskCreate
*	功能说明: 创建应用任务
*	彄1�7    参：旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
*********************************************************************************************************
*/
void AppTaskCreate (void)
{

//	xTaskCreate( vTaskRunPro,    		/* 任务函数  */
//                 "vTaskRunPro",  		/* 任务各1�7    */
//                 128,         		/* stack大小，单位word，也就是4字节 */
//                 NULL,        		/* 任务参数  */
//                 1,           		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
//                 &xHandleTaskRunPro); /* 任务句柄  */
	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务各1�7    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskStart );   /* 任务句柄  */
}



/*
*********************************************************************************************************
*	凄1�7 敄1�7 各1�7: AppObjCreate
*	功能说明: 创建任务通信机制
*	彄1�7    叄1�7: 旄1�7
*	迄1�7 囄1�7 倄1�7: 旄1�7
*********************************************************************************************************
*/
void AppObjCreate (void)
{
    #if 1

//   /* 创建10个uint8_t型消息队刄1�7 */
//	xQueue1 = xQueueCreate(4, sizeof(uint8_t));
//    if( xQueue1 == 0 )
//    {
//        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
//    }
	
	/* 创建10个存储指针变量的消息队列，由于CM3/CM4内核昄1�732位机，一个指针变量占甄1�74个字芄1�7 */
	xQueue2 = xQueueCreate(10, sizeof(struct Msg *));
    if( xQueue2 == 0 )
    {
        /* 没有创建成功，用户可以在这里加入创建失败的处理机刄1�7 */
    }

	

	#endif 

    #if 0

	 /* 创建队列雄1�7 */
    xQueueSet = xQueueCreateSet(QUEUESET_LENGTH);
    /* 创建队列*/
    xQueue1 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
    xQueue2 = xQueueCreate(QUEUE_LENGTH, QUEUE_ITEM_SIZE);
	
    /* 创建二��信号量 */
    xSemaphore = xSemaphoreCreateBinary();
	
    /* 将队列和二��信号量添加到队列集丄1�7 */
    xQueueAddToSet(xQueue1, xQueueSet);
    xQueueAddToSet(xQueue2, xQueueSet);
    xQueueAddToSet(xSemaphore, xQueueSet);
    #endif 
}


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
     static uint8_t state;
     BaseType_t xHigherPriorityTaskWoken = pdFALSE;
     MSG_T *ptMsg;

    if(huart->Instance==USART2)
    {
    // DISABLE_INT();
     if(net_t.linking_tencent_cloud_doing ==1){

			gpro_t.wifi_rx_data_array[gpro_t.wifi_rx_data_counter] =wifi_rx_inputBuf[0];
			gpro_t.wifi_rx_data_counter++;

			if(*wifi_rx_inputBuf==0X0A) // 0x0A = "\n"
			{
				
				Wifi_Rx_InputInfo_Handler();
				gpro_t.wifi_rx_data_counter=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					gpro_t.wifi_rx_data_array[gpro_t.wifi_rx_data_counter] = wifi_rx_inputBuf[0];
					gpro_t.wifi_rx_data_counter++;
					
				}
				else
				Subscribe_Rx_Interrupt_Handler();
	      }
      // ENABLE_INT();
	  __HAL_UART_CLEAR_OREFLAG(&huart2);
      HAL_UART_Receive_IT(&huart2,wifi_rx_inputBuf,1);
	}

	
   if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
      // DISABLE_INT();
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 0xA5){  // 0xA5 --didplay command head
               rx_data_counter=0;
               gl_tMsg.usData[rx_data_counter] = inputBuf[0];
			   state=1; //=1

             }
            else
                state=0;
		break;

       
		case 1: //#1

            if(gpro_t.disp_rx_cmd_done_flag ==0){
              /* 初始化结构体指针 */
               rx_data_counter++;
		     
	          gl_tMsg.usData[rx_data_counter] = inputBuf[0];
              

              if(rx_end_flag == 1){

                state = 0;
            
                uid = rx_data_counter;
                rx_end_flag=0;

                rx_data_counter =0;

                gpro_t.disp_rx_cmd_done_flag = 1 ;

                state=0;

                bcc_check_code=inputBuf[0];

                #if 0

                /* 向消息队列发数据 */
                xQueueSendFromISR(xQueue2,
                (void *)&gl_tMsg.usData,
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

                 #endif 

                xTaskNotifyFromISR(xHandleTaskStart,  /* 目标任务 */
                DECODER_BIT_0,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);

                /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
                  
              }

              }

              if(gl_tMsg.usData[rx_data_counter] ==0xFE && rx_end_flag == 0 &&  rx_data_counter > 4){
                     
                          rx_end_flag = 1 ;
                          
                        
               }

        break;


			
		}

      //  ENABLE_INT();
	    __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
	 }
    
    
  
 }











