/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "dma.h"
#include "gpio.h"
#include "SerLCD.h"
#include "i2c.h"
#include "spi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FALSE 0
#define TRUE 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint8_t pir_status = FALSE;	//FALSE->PM2008, TRUE->OTHER
uint8_t counter = 0;

uint8_t co2_buffer[13];
uint8_t co2_result_buffer[9];
uint8_t i_co2 = 0;

uint8_t mise_buffer[32];
uint8_t mise_send_buffer[7] = {0x42,0x4d,0};

uint8_t pm2_5 = 0;
uint8_t pm10 = 0;
char s_pm2_5[3] = "";
char s_pm10[3] = "";

uint8_t tx_data[6] = {0xA0, 0x00, 0x00, 0xA1, 0x00, 0x00};
uint8_t rx_data[6];

uint8_t led_flag = 0; // 0 -> green, 1 -> yellow, 2 -> red

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  xTaskCreate(vTask1,		/* Pointer to the function that implements the task. */
  				"Task 1",	/* text name */
  				128,		/* stack depth */
  				NULL,		/* task parameter. */
  				3,			/* task priority */
  				NULL );		/* task handle. */

  	/* Create the other task in exactly the same way. */
  	xTaskCreate( vTask2, "Task 2", 128, NULL, 3, NULL );
  	xTaskCreate( vTask3, "Task 3", 128, NULL, 3, NULL );
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void vTask1( void *pvParameters )
{
	if(HAL_UART_Receive_DMA(&huart3,mise_buffer,32)!=HAL_OK)
	{
		printf("fail\r\n");
	}
	write_MISE("Passive");
	/* infinite loop. */
	for( ;; )
	{

	}
}


void vTask2( void *pvParameters )
{
	/* infinite loop. */
	for( ;; )
	{
		HAL_UART_Receive_IT(&huart4, co2_buffer, 12);
		displayInit(&hi2c1);
		if(!pir_status) {
			displayWriteString("PM2.5 : ");
			displayWriteString(s_pm2_5);
			displaySetCursor(0, 1);
			displayWriteString("PM10  : ");
			displayWriteString(s_pm10);
		} else {
			displayWriteString("CO2 : ");
			displayWriteString(co2_result_buffer);
		}
		counter++;
		if(counter >= 5) {
			counter = 0;
			pir_status = ~pir_status;
		}

		i_co2 = atoi(co2_buffer);
		HAL_Delay(1000);
	}
}

void vTask3( void *pvParameters ) {
	for( ;; ) {
//		HAL_GPIO_TogglePin(Relay_GPIO_Port, Relay_Pin);
//		HAL_GPIO_TogglePin(Relay2_GPIO_Port, Relay2_Pin);
//		if(HAL_GPIO_ReadPin(PIR_GPIO_Port, PIR_Pin))
//			printf("DETECTED!!!\r\n");
//		else
//			printf("NO DETECTION...\r\n");
//		HAL_Delay(1000);

		MX_SPI1_Init();
		HAL_Delay(1000);
		HAL_GPIO_WritePin(SCE_GPIO_Port, SCE_Pin, RESET);
		HAL_Delay(1);
		HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 6, 100);
		HAL_Delay(1);
		HAL_GPIO_WritePin(SCE_GPIO_Port, SCE_Pin, SET);
		for(int i=0; i<6; i++)
			printf("%2X\r\n", rx_data[i]);
		HAL_Delay(1000);
	}
}


void print_MISE(void)
{
	uint16_t combine_value, check_byte_receive, check_byte_calculate=0;

	check_byte_receive=mise_buffer[30]<<8|mise_buffer[31];
	for(uint8_t i=0;i<30;i++)
	{
		check_byte_calculate+=mise_buffer[i];
	}

	if(check_byte_receive==check_byte_calculate)
	{
		printf("PM1.0 : %d	",(combine_value=(mise_buffer[10]<<8)|mise_buffer[11]));
		printf("PM2.5 : %d	",(combine_value=(mise_buffer[12]<<8)|mise_buffer[13]));
		printf("PM10 : %d	",(combine_value=(mise_buffer[14]<<8)|mise_buffer[15]));
		printf("0.3um : %d	",(combine_value=(mise_buffer[16]<<8)|mise_buffer[17]));
		printf("0.5um : %d	",(combine_value=(mise_buffer[18]<<8)|mise_buffer[19]));
		printf("1.0um : %d	",(combine_value=(mise_buffer[20]<<8)|mise_buffer[21]));
		printf("2.5um : %d	",(combine_value=(mise_buffer[22]<<8)|mise_buffer[23]));
		printf("5.0um : %d	",(combine_value=(mise_buffer[24]<<8)|mise_buffer[25]));
		printf("10.0um : %d\r\n",(combine_value=(mise_buffer[26]<<8)|mise_buffer[27]));
		pm2_5 = combine_value=((mise_buffer[12]<<8)|mise_buffer[13]);
		pm10 = combine_value=((mise_buffer[14]<<8)|mise_buffer[15]);
	}
	else
	{
	}
}

void write_MISE(char* cmd)
{
	uint16_t verify_byte=0;

	if(strcmp(cmd,"Read")==0)
	{
		mise_send_buffer[2]=0xe2;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
	}
	else if(strcmp(cmd,"Passive")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
	}
	else if(strcmp(cmd,"Active")==0)
	{
		mise_send_buffer[2]=0xe1;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;

	}
	else if(strcmp(cmd,"Sleep")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x00;
	}
	else if(strcmp(cmd,"WakeUp")==0)
	{
		mise_send_buffer[2]=0xe4;
		mise_send_buffer[3]=0x00;
		mise_send_buffer[4]=0x01;
	}
	for(uint8_t i=0;i<5;i++)
	{
		verify_byte+=mise_send_buffer[i];
	}
	mise_send_buffer[5]=verify_byte>>8;
	mise_send_buffer[6]=verify_byte;

	while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
	{
	}

	if(HAL_UART_Transmit_IT(&huart3,(uint8_t*)mise_send_buffer,7)!=HAL_OK)
	{

	}
	if(strcmp(cmd,"Read")==0)
	{
		while(HAL_UART_GetState(&huart3)!=HAL_UART_STATE_READY)
		{
		}
		if(HAL_UART_Receive_IT(&huart3,mise_send_buffer, 32)!=HAL_OK)
		{

		}
	}

	printf("%d\r\n", mise_send_buffer);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if(huart->Instance == USART3) {
		print_MISE();
		sprintf(s_pm2_5, "%d", pm2_5);
		sprintf(s_pm10, "%d", pm10);
	}
	if(huart->Instance == UART4) {
		for(int i=0; i<9; i++) {
			co2_result_buffer[i] = co2_buffer[i+1];
		}
		printf("%s\r\n", co2_result_buffer);

		for(int i=0; i<5; i++) {
			printf("%s", co2_result_buffer[i]);
		}
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USART3)
	{

	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == URBTN_Pin) {
		HAL_GPIO_TogglePin(Relay2_GPIO_Port, Relay2_Pin);
	}
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
