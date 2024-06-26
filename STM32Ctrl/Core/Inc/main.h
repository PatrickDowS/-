/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#define SHT30_SDA_Pin GPIO_PIN_14
#define SHT30_SDA_GPIO_Port GPIOC
#define SHT30_SCK_Pin GPIO_PIN_15
#define SHT30_SCK_GPIO_Port GPIOC
#define EM4325_CS_Pin GPIO_PIN_4
#define EM4325_CS_GPIO_Port GPIOA
#define EM4325_SCK_Pin GPIO_PIN_5
#define EM4325_SCK_GPIO_Port GPIOA
#define EM4325_MISO_Pin GPIO_PIN_6
#define EM4325_MISO_GPIO_Port GPIOA
#define EM4325_MOSI_Pin GPIO_PIN_7
#define EM4325_MOSI_GPIO_Port GPIOA
#define SP_SDO_Pin GPIO_PIN_5
#define SP_SDO_GPIO_Port GPIOB
#define LED_D2_Pin GPIO_PIN_6
#define LED_D2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
