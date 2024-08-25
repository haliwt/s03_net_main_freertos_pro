#include "bsp.h"



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
	uint8_t usData[1];
	uint8_t ulData[1];
}MSG_T;

MSG_T   g_tMsg; /* 定义丢�个结构体用于消息队列 */

uint8_t decoder_flag;

uint8_t ulid,uldata,usdata;

//uint8_t test_counter;

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
	   AppObjCreate();
	  
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

       //Buzzer_KeySound(void);
        run_t.power_off_flag = 0xff;
        buzzer_sound();

    }
     


     if(run_t.RunCommand_Label== POWER_ON){
        
             mainboard_run_handler();

             Read_TempSensor_Data();

             works_two_hours_detected_handler();

            // test_counter ++;

      }
      else{

             PowerOff_Run_Pro();

      }

    
   
     vTaskDelay(100);
     
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

	
    while(1)
    {
		
       xResult = xQueueReceive(xQueue2,                   /* 消息队列句柄 */
		                        (void *)&ptMsg,  		   /* 这里获取的是结构体的地址 */
		                        (TickType_t)xMaxBlockTime);/* 设置阻塞时间 */
		
		if(xResult == pdPASS){
            

          usdata = ptMsg->usData[0];

          uldata = ptMsg->ulData[0];
          

          if(usdata ==0x90 || usdata ==0x91){
            
             if(uldata == 0x43){
                 // buzzer_sound();//Buzzer_KeySound();
              }
           }
           else{
              buzzer_sound();//Buzzer_KeySound();


           }
          Decode_RunCmd(uldata,usdata);
       
         /* 成功接收，  并通过串口将数据打印出来 */
			
		    //printf("ptMsg->ucMessageID = %d\r\n",ptMsg->ucMessageID);
            ///printf("ptMsg->usData[0] = %d\r\n",ptMsg->usData[0]);
            //printf("ptMsg->ulData[0] = %d\r\n",ptMsg->ulData[0]);
      
          #if 0
          
            HAL_UART_Transmit(&huart2, &uldata, 1, 0xffff);
            
            HAL_UART_Transmit(&huart2, &space_key, 1, 0xffff);
            HAL_UART_Transmit(&huart2, &usdata, 1, 0xffff);
          #endif 
		
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
                 2,               		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务各1�7    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,              		/* 任务优先纄1�7 数��越小优先级越低，这个跟uCOS相反 */
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
           
         
        //  USART2->ISR = 0xf5; 
	
	      if(esp8266data.linking_tencent_cloud_doing ==1){

			wifi_usart_data.UART_Data[wifi_usart_data.UART_Cnt] = wifi_usart_data.wifi_inputBuf[0];
			wifi_usart_data.UART_Cnt++;

			if(*wifi_usart_data.wifi_inputBuf==0X0A) // 0x0A = "\n"
			{
				wifi_usart_data.UART_Flag = 1;
				Wifi_Rx_InputInfo_Handler();
				wifi_usart_data.UART_Cnt=0;
			}

	      } 
		  else{

		         if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_usart_data.UART_Data[wifi_usart_data.UART_Cnt] = wifi_usart_data.wifi_inputBuf[0];
					wifi_usart_data.UART_Cnt++;
					//Subscribe_Rx_Interrupt_Handler();
				}
				else
				Subscribe_Rx_Interrupt_Handler();
	      }
	  __HAL_UART_CLEAR_OREFLAG(&huart2);
      HAL_UART_Receive_IT(&huart2,wifi_usart_data.wifi_inputBuf,1);
	}

	
   if(huart->Instance==USART1)//if(huart==&huart1) // Motor Board receive data (filter)
	{
       DISABLE_INT();
		switch(state)
		{
		case 0:  //#0
			if(inputBuf[0] == 'T')  //hex :54 - "T" -fixed
				state=1; //=1
            else
                state=0;
		break;
		case 1: //#1
             if(inputBuf[0] == 'K'){//hex :4B - "K" -fixed
               
				state=2; //=1

             }
			else{
			   state =0;
			}
			break;
            
        case 2:
			 if(inputBuf[0] == 'P' || inputBuf[0] == 'C' ||inputBuf[0] == 'Z' || inputBuf[0]=='T'){
			    inputCmd[0]= inputBuf[0];
               
                // ptMsg->ulData[0] = inputBuf[0];
				 state =3;

			 }
			 else
                 state =0;
        
        break;
        
        case 3:

              /* 初始化结构体指针 */
              ptMsg = &g_tMsg;
		  
	        inputCmd[1]= inputBuf[0];

            ptMsg->ulData[0] = inputCmd[0];
            ptMsg ->usData[0] =  inputCmd[1];
           
           

             /* 向消息队列发数据 */
        	xQueueSendFromISR(xQueue2,
        				      (void *)&ptMsg,
        				      &xHigherPriorityTaskWoken);

        	/* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	       
		 
			 
	         state = 0;
        
        break;

	
			
		}

        ENABLE_INT();
	    __HAL_UART_CLEAR_OREFLAG(&huart1);
		HAL_UART_Receive_IT(&huart1,inputBuf,1);//UART receive data interrupt 1 byte
		
	 }
    
    
  
 }











