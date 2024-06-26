/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "SHT3X.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t UART2_Code_GetkPa[4] = {0x55, 0x04, 0x0d, 0x88};
uint8_t UART1_ReceiveFinish = 0;
uint8_t UART2_ReceiveFinish = 0;
uint8_t UART1_ReceiveCount = 0;
uint8_t UART2_ReceiveCount = 0;
uint8_t UART1_ReceiveBuf[10];
uint8_t UART2_ReceiveBuf[10];
uint16_t Time6Count = 0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart == &huart1)
	{
		UART1_ReceiveCount++;
		if(UART1_ReceiveCount >=5)
		{
			UART1_ReceiveFinish = 1;
			UART1_ReceiveCount = 0;
		}
		
//		UART1_ReceiveCount = 0;
//		HAL_UART_Transmit(&huart2, &UART1_ReceiveBuf[UART1_ReceiveCount], 1, 0xFFFF);
		HAL_UART_Receive_IT(&huart1, &UART1_ReceiveBuf[UART1_ReceiveCount], 1);
	}
	else if (huart == &huart2)
	{
		UART2_ReceiveCount++;
		if(UART2_ReceiveCount >= 8)
		{
			UART2_ReceiveFinish = 1;
			UART2_ReceiveCount = 0;
		}
		
//		UART2_ReceiveCount = 0;
//		HAL_UART_Transmit(&huart1, &UART2_ReceiveBuf[UART2_ReceiveCount], 1, 0xFFFF);
		HAL_UART_Receive_IT(&huart2, &UART2_ReceiveBuf[UART2_ReceiveCount], 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM6)
	{
		Time6Count++;
	}
}

void SendFloatData(float TempValue)
{
	uint8_t DataBuf[6];
	if(TempValue >= 1000)
	{
		DataBuf[0] = (uint8_t)(((int)(TempValue) / 1000) + 0x30);
		DataBuf[1] = (uint8_t)(((int)(TempValue) / 100) + 0x30);
		DataBuf[2] = (uint8_t)((((int)(TempValue) % 100) / 10) + 0x30);
		DataBuf[3] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[4] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[5] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		
		HAL_UART_Transmit(&huart1, DataBuf, 6, 0xFFFF);
	}
	else if(TempValue >= 100)
	{
		DataBuf[0] = (uint8_t)(((int)(TempValue) / 100) + 0x30);
		DataBuf[1] = (uint8_t)((((int)(TempValue) % 100) / 10) + 0x30);
		DataBuf[2] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[3] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[4] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		
		HAL_UART_Transmit(&huart1, DataBuf, 5, 0xFFFF);
	}
	else if(TempValue >= 10)
	{
		DataBuf[0] = (uint8_t)(((int)(TempValue) / 10) + 0x30);
		DataBuf[1] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[2] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[3] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		
		HAL_UART_Transmit(&huart1, DataBuf, 4, 0xFFFF);
	}
	else if(TempValue >= 0)
	{
		DataBuf[0] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[1] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[2] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		
		HAL_UART_Transmit(&huart1, DataBuf, 3, 0xFFFF);
	}
	else if(TempValue > -10)
	{
		DataBuf[0] = (uint8_t)'-';
		TempValue = -TempValue;
		DataBuf[1] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[2] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[3] = (uint8_t)((((int)(TempValue) % 10) + 0x30));
		
		HAL_UART_Transmit(&huart1, DataBuf, 4, 0xFFFF);
	}
	else if(TempValue > -100)
	{
		DataBuf[0] = (uint8_t)'-';
		TempValue = -TempValue;
		DataBuf[1] = (uint8_t)(((int)(TempValue) / 10) + 0x30);
		DataBuf[2] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		DataBuf[3] = 0x2e;
		TempValue = TempValue - (int)TempValue;
		TempValue = TempValue * 10;
		DataBuf[4] = (uint8_t)(((int)(TempValue) % 10) + 0x30);
		
		HAL_UART_Transmit(&huart1, DataBuf, 5, 0xFFFF);
	}
}


void SPIDelay_500us()
{
	uint8_t a,b,c;
     for(c=11;c>0;c--)
        for(b=4;b>0;b--)
            for(a=89;a>0;a--);
}

void SPIDelay_50us()
{
	uint8_t a,b,c;
    for(c=1;c>0;c--)
        for(b=2;b>0;b--)
            for(a=197;a>0;a--);
}

void SendSPI(uint16_t Dat1, uint16_t Dat2, uint16_t Dat3, uint8_t * ReceiveBuf)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	uint8_t i = 0;
	for(i = 0; i < 16; i++)
	{
		if((Dat1 & 0x8000) != 0)
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_RESET);
		}
		Dat1 = Dat1 << 1;
		
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	}
	
	SPIDelay_50us();
	
	for(i = 0; i < 16; i++)
	{
		if((Dat1 & 0x8000) != 0)
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_RESET);
		}
		Dat1 = Dat1 << 1;
		
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	}
	
	SPIDelay_50us();
	
	for(i = 0; i < 16; i++)
	{
		if((Dat1 & 0x8000) != 0)
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_RESET);
		}
		Dat1 = Dat1 << 1;
		
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	}
	
	SPIDelay_50us();
	
	for(i = 0; i < 16; i++)
	{
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		
		ReceiveBuf[i] = ReceiveBuf[i] << 1;
		GPIO_PinState SDOState = HAL_GPIO_ReadPin(EM4325_MISO_GPIO_Port, EM4325_MISO_Pin);
		if(SDOState == GPIO_PIN_RESET)
		{
			ReceiveBuf[i] = ReceiveBuf[i] & 0xfe;
		}
		else
		{
			ReceiveBuf[i] = ReceiveBuf[i] | 0x01;
		}
		SPIDelay_50us();
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	}
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
}


void SendOneByte(uint8_t SendData)
{
	uint8_t i = 0;
	for(i = 0; i < 8; i++)
	{
		if((SendData & 0x80) != 0)
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(EM4325_MOSI_GPIO_Port, EM4325_MOSI_Pin, GPIO_PIN_RESET);
		}
		SendData = SendData << 1;
		
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		SPIDelay_50us();
		
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
		SPIDelay_50us();
	}
}

uint8_t GetOneByte()
{
	uint8_t i = 0;
	uint8_t ReceiveData = 0x00;
	for(i = 0; i < 8; i++)
	{
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_SET);
		SPIDelay_50us();
		
		ReceiveData = ReceiveData << 1;
		GPIO_PinState SDOState = HAL_GPIO_ReadPin(EM4325_MISO_GPIO_Port, EM4325_MISO_Pin);
		if(SDOState == GPIO_PIN_RESET)
		{
			ReceiveData = ReceiveData & 0xfe;
		}
		else
		{
			ReceiveData = ReceiveData | 0x01;
		}
		
		HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
		SPIDelay_50us();
	}
	
	return ReceiveData;
}

uint8_t GetSPIRequestStatus()
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xe0);
	uint8_t RequestStatus = GetOneByte();
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	
	return RequestStatus;
}

uint8_t GetSPIBoot()
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xe1);
	uint8_t RequestStatus = GetOneByte();
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	return RequestStatus;
}

void GetSensorData(uint8_t * ReceiveBuf)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xe4);
	
	HAL_Delay(30);
	
	uint8_t i = 0;
	for(i = 0; i < 9; i++)
	{
		ReceiveBuf[i] = GetOneByte();
	}
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
}

void GetReqRN(uint8_t * ReceiveBuf)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xF0);
	uint8_t RequestStatus1 = GetOneByte();
	uint8_t RequestStatus2 = GetOneByte();
	uint8_t RequestStatus3 = GetOneByte();
	ReceiveBuf[0] = RequestStatus1;
	ReceiveBuf[1] = RequestStatus2;
	ReceiveBuf[2] = RequestStatus3;
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
}

void GetReadWord(uint8_t Addr, uint8_t * ReceiveBuf)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xE7);
	
	SendOneByte(Addr);
	SPIDelay_500us();
	SPIDelay_500us();
	
	uint8_t RequestStatus1 = GetOneByte();
	SPIDelay_500us();
	
	uint8_t RequestStatus2 = GetOneByte();
	SPIDelay_500us();
	
	uint8_t RequestStatus3 = GetOneByte();
	SPIDelay_500us();
	
	ReceiveBuf[0] = RequestStatus1;
	ReceiveBuf[1] = RequestStatus2;
	ReceiveBuf[2] = RequestStatus3;
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
}

uint8_t SetWriteWord(uint8_t Addr, uint16_t WordData)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xE8);
	
	SendOneByte(Addr);
	SendOneByte(WordData >> 8);
	SendOneByte(WordData);
	
	uint8_t RequestStatus1 = GetOneByte();
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	
	return RequestStatus1;
}

void GetReadPage(uint8_t PageNum, uint8_t * ReceiveBuf)
{
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_RESET);
	SPIDelay_500us();
	
	SendOneByte(0xe9);
	SendOneByte(PageNum | 0x80);
	
	SPIDelay_500us();
	SPIDelay_500us();
	HAL_Delay(1);
	HAL_Delay(1);
	
	uint8_t i = 0;
	for(i = 0; i < 9; i++)
	{
		ReceiveBuf[i] = GetOneByte();
	}
	
	HAL_GPIO_WritePin(EM4325_CS_GPIO_Port, EM4325_CS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(EM4325_SCK_GPIO_Port, EM4325_SCK_Pin, GPIO_PIN_RESET);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	uint8_t ReceiveBuf[16];
	uint8_t RequestStatus;
	uint16_t RequestStatus16;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_IT(&huart1, &UART1_ReceiveBuf[UART1_ReceiveCount], 1);
	HAL_UART_Receive_IT(&huart2, &UART2_ReceiveBuf[UART2_ReceiveCount], 1);
	
	Init_SHT3X();
	HAL_TIM_Base_Start_IT(&htim6);
  //HAL_TIM_Base_Start(&htim6);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(Time6Count >= 25)
		{
			Time6Count = 0;
			HAL_GPIO_TogglePin(LED_D2_GPIO_Port, LED_D2_Pin);


			float pTemperature = 0; 
			float pHumidity = 0;
			SHT30_read_result(&pTemperature, &pHumidity);
			
			
			HAL_UART_Transmit(&huart1, (uint8_t*)"Temp:", 5, 0xFFFF);
			SendFloatData(pTemperature);
			HAL_UART_Transmit(&huart1, (uint8_t*)", Humi:", 7, 0xFFFF);
			SendFloatData(pHumidity);
			
			if(UART1_ReceiveFinish == 1)
			{
				UART1_ReceiveFinish = 0;
				HAL_UART_Transmit(&huart1, (uint8_t*)"ReceiveFinish1", 14, 0xFFFF);
			}
			
			if(UART2_ReceiveFinish == 1)
			{
				UART2_ReceiveFinish = 0;
				float kPa = UART2_ReceiveBuf[5];
				kPa = kPa * 256 * 256;
				kPa = kPa + UART2_ReceiveBuf[4] * 256;
				kPa = kPa + UART2_ReceiveBuf[3];
				kPa = kPa / 1000;
				
				HAL_UART_Transmit(&huart1, (uint8_t*)", kPa:", 6, 0xFFFF);
				SendFloatData(kPa);
			}
			
			UART2_ReceiveCount = 0;
			HAL_UART_Transmit(&huart2, UART2_Code_GetkPa, 4, 0xFFFF);
			
			//EM4325
//			RequestStatus = GetSPIRequestStatus();
//			HAL_UART_Transmit(&huart1, &RequestStatus, 1, 0xFFFF);
//			HAL_Delay(1);
			
//			RequestStatus = GetSPIBoot();
//			HAL_UART_Transmit(&huart1, &RequestStatus, 1, 0xFFFF);
//			HAL_Delay(1);
//			
//			GetReqRN(ReceiveBuf);
//			HAL_UART_Transmit(&huart1, ReceiveBuf, 3, 0xFFFF);
//			HAL_Delay(1);
		}
		HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
