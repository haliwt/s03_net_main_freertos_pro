#include "bsp.h"

#define MAX_BUFFER_SIZE  12

uint8_t  inputBuf[4];
uint8_t  inputCmd[30];
uint8_t  wifiInputBuf[1];
//uint8_t test_counter;
//uint8_t test_counter_usat1;

uint8_t rx_wifi_data[7];

//uint8_t wifi_rx_temp_data[25];


static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

volatile uint8_t transOngoingFlag;
volatile uint8_t usart2_transOngoingFlag;

void USART1_ERROR_Callback(void);
void (*EUSART_TxDefaultInterruptHandler)(void);
void (*EUSART_RxDefaultInterruptHandler)(void);

//void (*EUSART_FramingErrorHandler)(void);
//void (*EUSART_OverrunErrorHandler)(void);
//void (*EUSART_ErrorHandler)(void);




    
/********************************************
	*
	*Function Name:
    *Function: receive dsipay panel of order
    *Input Ref:NO
    *Return Ref:NO

*********************************************/ 
void USART1_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{

     uint32_t temp;

   if(huart==&huart1){


      if(gctl_t.gTimer_usart_error >27){
	  	gctl_t.gTimer_usart_error=0;
	     __HAL_UART_CLEAR_OREFLAG(&huart1);
		 temp = USART1->RDR;

		  UART_Start_Receive_IT(&huart1,inputBuf,1);
		
		
         }
		  
          
        }
  	  
 }


void USART2_Cmd_Error_Handler(UART_HandleTypeDef *huart)
{
       uint32_t temp;

	if(huart==&huart2){


      if(gctl_t.gTimer_usart2_error >11){
	  	gctl_t.gTimer_usart2_error=0;

           __HAL_UART_CLEAR_OREFLAG(&huart2);
        

          temp = USART2->RDR;

		   UART_Start_Receive_IT(&huart2,(uint8_t *)wifi_rx_inputBuf,1);
		
		
         	}
		  
        } 
       
  	  
   
}
/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
{

	//crc=0x55;
	outputBuf[0]=0x5A; //head : displayBoard = 0xA5
	outputBuf[1]=0x10; //main board device No: 0x10
	outputBuf[2]=0x1A; //command : temperature of value 
	outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
	outputBuf[4]= 0x02; //data of length: 0x01 - 2 byte.
	outputBuf[5] =hum;
    outputBuf[6] =temp;

    outputBuf[7] = 0xFE;
    outputBuf[8] = bcc_check(outputBuf,8);
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=9;
	if(transferSize)
	{
		while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}
/********************************************************************************
    **
    *Function Name:void SendWifiData_To_PanelTime(uint8_t hours,uint8_t minutes,uint8_t seconds)
    *Function :
    *Input Ref: hours,minutes,seconds of beijing time 
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_PanelTime(uint8_t hours,uint8_t minutes,uint8_t seconds)
{
    outputBuf[0]=0x5A; //mainboard head : displayBoard = 0xA5
	outputBuf[1]=0x10; //mainboard device No: 01
	outputBuf[2]=0x1C; //command : is data of hours and minutes and seconds.
	outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
	outputBuf[4]= 0x03; //data of length: 0x01 - 3 byte.
	outputBuf[5]= hours; //	
	outputBuf[6]= minutes; //	
	outputBuf[7]= seconds; //	

    outputBuf[8] = 0xFE;
    outputBuf[9] = bcc_check(outputBuf,9);

	transferSize=10;
	if(transferSize)
	{
	while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
	transOngoingFlag=1;
	HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_WifiSetTemp(uint8_t dat1)
    *Function :
    *Input Ref: dat1- fan of grade value 
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_WifiSetTemp(uint8_t dat1)
{
        outputBuf[0]=0x5A; //head : displayBoard = 0xA5
        outputBuf[1]=0x10; //main board device No: 01
        outputBuf[2]=0x1A; //command type: temperature value
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
        outputBuf[4]= 0x01; //data of length: 0x01 - 1 byte.
        outputBuf[5] =dat1;
    
    
         outputBuf[6] = 0xFE;
         outputBuf[7] = bcc_check(outputBuf,7);
        
        transferSize=8;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
       

}


/********************************************************************************
    **
    *Function Name:void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
    *Function :
    *Input Ref: dat1- fan of grade value 
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_PanelWindSpeed(uint8_t dat1)
{
   
	    outputBuf[0]=0x5A; //head : displayBoard = 0xA5
        outputBuf[1]=0x10; //device No: 01
        outputBuf[2]=0x1E; //command type: fan speed of value 
        outputBuf[3]=0x0F; // 0x0F : is data ,don't command data.
        outputBuf[4]= 0x01; //data of length: 0x01 - 2 byte.
        outputBuf[5] =dat1;
    
    
        outputBuf[6] = 0xFE;
        outputBuf[7] = bcc_check(outputBuf,7);
        
        transferSize=8;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }

}

/********************************************************************************
    **
    *Function Name:void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
    *Function : commad order , data -command type
    *Input Ref: commad order , data -command type
    *Return Ref:NO
    *
*******************************************************************************/
void SendWifiData_To_Cmd(uint8_t cmd,uint8_t data)
{
        outputBuf[0]=0x5A; //head : displayBoard = 0xA5
        outputBuf[1]=0x10; //device No: 01
        outputBuf[2]=cmd; //command type: fan speed of value 
        outputBuf[3]=data; // 0x0F : is data ,don't command data.
        outputBuf[4]= 0x0; // don't data 
        
        outputBuf[5] = 0xFE;
        outputBuf[6] = bcc_check(outputBuf,6);
        
        transferSize=7;
        if(transferSize)
        {
            while(transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
            transOngoingFlag=1;
            HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
        }
	
}



void EUSART_SetTxInterruptHandler(void (* interruptHandler)(void))
{
    EUSART_TxDefaultInterruptHandler = interruptHandler;
}


/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1)
	{
		transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

	if(huart== &huart2){

       usart2_transOngoingFlag =0;

	}

}





