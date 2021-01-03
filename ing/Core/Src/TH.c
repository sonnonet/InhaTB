/*
 * TH.c
 *
 *  Created on: Jan 3, 2021
 *      Author: S3
 */
#include "main.h"
#include "TH.h"
#include "stm32l4s5i_iot01.h"
#include "stm32l4s5i_iot01_tsensor.h"
#include "stm32l4s5i_iot01_hsensor.h"
/*
extern UART_HandleTypeDef huart1;

int __io_putchar(int ch)
{
	while(HAL_OK != HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 30000))
	{
	}
	return ch;

}
int __io_getchar(void)
{
	uint8_t ch =0;
	while(HAL_OK != HAL_UART_Receive(&huart1, (uint8_t *)&ch, 1, 30000))
	{
	}
	return ch;
}
*/
float get_temperature(void)
{
	float temp_value = 0.0;
	BSP_TSENSOR_Init();
	temp_value = BSP_TSENSOR_ReadTemp();

	return temp_value;
}
float get_humidity(void)
{
	float humi_value = 0.0;
	BSP_HSENSOR_Init();
	humi_value = BSP_HSENSOR_ReadHumidity();

	return humi_value;
}
