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
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "usart.h"
#include "dma.h"
#include "gpio.h"
#include "SerLCD.h"
#include "i2c.h"
#include "spi.h"
#include "TH.h"

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
uint8_t co2_result_buffer2[5];
int i_co2 = 0;

uint8_t mise_buffer[32];


uint8_t pm2_5 = 0;
uint8_t pm10 = 0;
char s_pm2_5[3] = "";
char s_pm10[3] = "";

uint8_t tx_data[6] = {0xA0, 0x00, 0x00, 0xA1, 0x00, 0x00};
uint8_t rx_data[6];

bool pm_flag = FALSE;
bool co2_flag = FALSE;
bool temp_flag = FALSE;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);
void print_MISE(void);
void write_MISE(char* cmd);
void print_co2(void);
void turnOnRed(void);
void turnOnGreen(void);
void turnOnYellow(void);
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

  // pm sensor init
  if(HAL_UART_Receive_DMA(&huart3,mise_buffer,32) != HAL_OK)
  {
	printf("fail\r\n");
  }

  uint8_t mise_send_buffer[7] = {0x42,0x4d,0};
  uint16_t verify_byte = 0;

  mise_send_buffer[2] = 0xe1;
  mise_send_buffer[3] = 0x00;
  mise_send_buffer[4] = 0x00;

  for(uint8_t i = 0; i < 5; i++)
  {
	verify_byte += mise_send_buffer[i];
  }
  mise_send_buffer[5] = verify_byte >> 8;
  mise_send_buffer[6] = verify_byte;

  HAL_UART_Transmit(&huart3, (uint8_t*)mise_send_buffer, 7, 100);

  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
int ipow(int base, int exp)
{
    int result = 1;
    for (;;)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (!exp)
            break;
        base *= base;
    }

    return result;
}

#define SENSOR_NUM 3
void vTask1(void *pvParameters)
{
  /* infinite loop. */
  uint8_t counter = 0;
  uint8_t seq = 0;
  uint16_t co2 = 0;
  float temp = 0.0;
  float humi = 0.0;

  for( ;; )
  {

	// particulate matter parsing
	if (seq % SENSOR_NUM == 0) {
	  if(pm_flag) {
		uint16_t combine_value, check_byte_receive, check_byte_calculate=0;

		check_byte_receive = mise_buffer[30] << 8 | mise_buffer[31];
		for(uint8_t i = 0; i < 30; i++)
		{
			check_byte_calculate += mise_buffer[i];
		}

		if(check_byte_receive == check_byte_calculate)
		{
			pm2_5 = combine_value = ((mise_buffer[12] << 8) | mise_buffer[13]);
			pm10 = combine_value = ((mise_buffer[14] << 8) | mise_buffer[15]);

			printf("p%d\r\n", pm2_5);
			HAL_Delay(100);
			printf("m%d\r\n", pm10);
			HAL_Delay(100);
		}

	    sprintf(s_pm2_5, "%d", pm2_5);
	    sprintf(s_pm10, "%d", pm10);


	    pm_flag = FALSE;
	  }

	// co2 parsing
	} else if (seq % SENSOR_NUM == 1) {
      if (co2_flag) {
        co2 = 0;

      	if (co2_buffer[6] == ' ' || co2_buffer[7] == 'p'  || co2_buffer[8] == 'p'
    	  || co2_buffer[9] == 'm'  || co2_buffer[10] != 0x0D
		  || co2_buffer[11] == 0x0A) {

    	  for(int i = 0; i < 6; i++) {
    		if(co2_buffer[i] != ' ') {
    		  //printf("%d, %d, %d \n", co2_buffer[i] - 48, i, ipow(10, 5 - i)); // for debug
    		  co2 += (co2_buffer[i] - 48) * ipow(10, 5 - i);
    		}
    	  }
    	}

      	// for debug
      	if (co2 > 1300) co2 = 1300;

    	if(i_co2 < 1000) {
    	  turnOnGreen();
    	} else if(i_co2 < 2000) {
    	  turnOnYellow();
    	} else {
    	  turnOnRed();
    	}

    	printf("c%d\r\n", co2);
    	HAL_Delay(100);

    	co2_flag = FALSE;
	  }

    // temperature sensing
    } else if (seq % SENSOR_NUM == 2) {
		HAL_GPIO_WritePin(SCE_GPIO_Port, SCE_Pin, RESET);
		HAL_SPI_TransmitReceive(&hspi1, tx_data, rx_data, 6, 100);
		HAL_GPIO_WritePin(SCE_GPIO_Port, SCE_Pin, SET);

        //printf("obj_temp:%d,sen_temp:%d",
    	//  (rx_data[2] <<8 ) | rx_data[1], (rx_data[4] <<8 ) | rx_data[5]);
		printf("t%d\r\n", (rx_data[2] <<8 ) | rx_data[1]);
		HAL_Delay(100);
        // ===================== HanGyeol modify =======================
		temp = get_temperature();
		humi = get_humidity();
		printf("T%d\r\n", temp);
		printf("H%d\r\n", humi);
    }

	printf("\n");
	HAL_Delay(100);
	// display to lcd
	// lcd init
	displayInit(&hi2c1);
	//displayClear();

	if(!pir_status) {
		displayWriteString("PM2.5 : ");
		displayWriteString(s_pm2_5);
		displaySetCursor(0, 1);
		displayWriteString("PM10  : ");
		displayWriteString(s_pm10);
	} else {
		char tmp_val[4];
		displayWriteString("CO2 : ");
		displayWriteString(itoa(co2, tmp_val, 10));
		displayWriteString("PPM");
	}


	seq++;
	counter++;

	if(counter >= 5) {
		counter = 0;
		pir_status = ~pir_status;
	}

    //printf("seq %d, counter : %d\n", seq, counter); // for debug
    HAL_Delay(500);

  }
}


void vTask2(void *pvParameters)
{
  /* infinite loop. */
  for( ;; )
  {
	if(!co2_flag) {
	  HAL_UART_Receive_IT(&huart4, co2_buffer, 12);
	  //HAL_UART_Receive_IT(&huart4, co2_buffer, 1);
	}
	HAL_Delay(1000);
  }
}

void vTask3(void *pvParameters)
{
  for( ;; ) {

  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if(huart->Instance == USART3) {
	pm_flag = TRUE;
  }
  if(huart->Instance == UART4) {
	co2_flag = TRUE;
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
	if(GPIO_Pin == URBTN_Pin) {
		HAL_GPIO_TogglePin(Relay2_GPIO_Port, Relay2_Pin);
	}
}

void turnOnRed(void) {
	HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, SET);
	HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, RESET);
	HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, RESET);
}

void turnOnYellow(void) {
	HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, SET);
	HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, RESET);
}

void turnOnGreen(void) {
	HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, RESET);
	HAL_GPIO_WritePin(YELLOW_GPIO_Port, YELLOW_Pin, RESET);
	HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, SET);
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
