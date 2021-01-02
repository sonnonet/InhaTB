/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define URBTN_Pin GPIO_PIN_13
#define URBTN_GPIO_Port GPIOC
#define URBTN_EXTI_IRQn EXTI15_10_IRQn
#define CO2_TX_Pin GPIO_PIN_0
#define CO2_TX_GPIO_Port GPIOA
#define CO2_RX_Pin GPIO_PIN_1
#define CO2_RX_GPIO_Port GPIOA
#define SCE_Pin GPIO_PIN_4
#define SCE_GPIO_Port GPIOA
#define PIR_Pin GPIO_PIN_0
#define PIR_GPIO_Port GPIOB
#define GREEN_Pin GPIO_PIN_1
#define GREEN_GPIO_Port GPIOB
#define Relay_Pin GPIO_PIN_2
#define Relay_GPIO_Port GPIOB
#define Relay2_Pin GPIO_PIN_14
#define Relay2_GPIO_Port GPIOD
#define RED_Pin GPIO_PIN_15
#define RED_GPIO_Port GPIOA
#define YELLOW_Pin GPIO_PIN_4
#define YELLOW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
