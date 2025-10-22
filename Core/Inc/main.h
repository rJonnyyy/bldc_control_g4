/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_nucleo.h"
#include <stdio.h>

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
#define RCC_OSC32_IN_Pin GPIO_PIN_14
#define RCC_OSC32_IN_GPIO_Port GPIOC
#define RCC_OSC32_OUT_Pin GPIO_PIN_15
#define RCC_OSC32_OUT_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port GPIOF
#define RCC_OSC_OUT_Pin GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define AI_SPEED_POTI_Pin GPIO_PIN_1
#define AI_SPEED_POTI_GPIO_Port GPIOC
#define AI_I_SHUNT_INV_Pin GPIO_PIN_1
#define AI_I_SHUNT_INV_GPIO_Port GPIOA
#define DO_INH_C_Pin GPIO_PIN_15
#define DO_INH_C_GPIO_Port GPIOB
#define DI_HALL_A_Pin GPIO_PIN_6
#define DI_HALL_A_GPIO_Port GPIOC
#define DI_HALL_B_Pin GPIO_PIN_7
#define DI_HALL_B_GPIO_Port GPIOC
#define DI_HALL_C_Pin GPIO_PIN_8
#define DI_HALL_C_GPIO_Port GPIOC
#define DO_IN_A_Pin GPIO_PIN_8
#define DO_IN_A_GPIO_Port GPIOA
#define DO_IN_B_Pin GPIO_PIN_9
#define DO_IN_B_GPIO_Port GPIOA
#define DO_IN_C_Pin GPIO_PIN_10
#define DO_IN_C_GPIO_Port GPIOA
#define DO_INH_A_Pin GPIO_PIN_11
#define DO_INH_A_GPIO_Port GPIOA
#define DO_INH_B_Pin GPIO_PIN_12
#define DO_INH_B_GPIO_Port GPIOA
#define T_SWDIO_Pin GPIO_PIN_13
#define T_SWDIO_GPIO_Port GPIOA
#define T_SWCLK_Pin GPIO_PIN_14
#define T_SWCLK_GPIO_Port GPIOA
#define DI_DIR_SWITCH_Pin GPIO_PIN_10
#define DI_DIR_SWITCH_GPIO_Port GPIOC
#define DI_ON_SWITCH_Pin GPIO_PIN_11
#define DI_ON_SWITCH_GPIO_Port GPIOC
#define T_SWO_Pin GPIO_PIN_3
#define T_SWO_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
