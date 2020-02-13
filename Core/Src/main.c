/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define _countof(a) (sizeof(a)/sizeof(*(a)))
#define DEFAULT_GPIO_DELAY (1000)

#define DEFAULT_GPIO_TOGGLES 5
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
uint8_t GetCommand();

uint8_t GetBlinksCount();

void BlinkNTimesWithDelay(
	uint8_t  uiBlinks,
	uint32_t uiDelay   /* mseconds */
);

uint32_t atoi(uint8_t* pszNumber);

_Bool IsDigit(uint8_t uiByte);

_Bool IsNumber(uint8_t* pszString);

void PrintNewLine();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
int delay_800 = 800;
HAL_Delay(5000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  uint8_t uiCommand = GetCommand();
	  switch(uiCommand)
	  	  {
	  		  case '1':
	  		  {
	  			  int n;
				  n = HAL_GetTick() % 12 + 1;	/* random is here!!1! */
				  BlinkNTimesWithDelay(
				  	  			      n,
									  delay_800
				  	  			  );
	  			  uint8_t uiBlinks = GetBlinksCount();
	  			  if(uiBlinks == n){
			uint8_t pszMessage[] = "\r\nright number";

			HAL_UART_Transmit(
				&huart1,
				pszMessage,
				_countof(pszMessage),
				HAL_MAX_DELAY
			);
	  			  } else{
	  				uint8_t pszMessage[] = "\r\nthat is wrong number";

	  				HAL_UART_Transmit(
	  					&huart1,
	  					pszMessage,
	  					_countof(pszMessage),
	  					HAL_MAX_DELAY
	  				);
	  		  			  }



	  			  break;
	  		  }
	  		case '2':
	  				  {
	  					  goto SILENCE;
	  				  }
	  				  default:
	  				  {
	  					  uint8_t pszErrorMessage[] = \
	  							  "\r\nYou've entered wrong command. Try again\r\n";

	  					  HAL_UART_Transmit(
	  					      &huart1,
	  						  pszErrorMessage,
	  						  _countof(pszErrorMessage),
	  						  HAL_MAX_DELAY
	  					  );

	  					  continue;
	  				  }
	  			  }
	  		  }

	  		SILENCE:	  		  ;

	  		  uint8_t pszFinalMessage[] = \
	  		  	  	  "\r\n\r\nService stopped! Reconnect to run again\r\n";

	  		  HAL_UART_Transmit(
	  		      &huart1,
	  			  pszFinalMessage,
	  			  _countof(pszFinalMessage),
	  			  HAL_MAX_DELAY
	  		  );

	  		  while(1)	  			  ;

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
uint8_t GetCommand()
{
	uint8_t pszMenu[] = \
			"\r\n1) start new game" \
			"\r\n2) Stop listening for commands" \
			"\r\nInput > ";

	HAL_UART_Transmit(
		&huart1,
		pszMenu,
		_countof(pszMenu),
		HAL_MAX_DELAY
	);

	uint8_t cmd[1] = { 0 };

	HAL_UART_Receive(
		&huart1,
		cmd,
		_countof(cmd),
		HAL_MAX_DELAY
	);

	HAL_UART_Transmit(
		&huart1,
		cmd,
		_countof(cmd),
		HAL_MAX_DELAY
	);

	PrintNewLine();

	return cmd[0];
}

uint8_t GetBlinksCount()
{
	uint8_t pszMessage[] = "\r\nEnter the number:";
	HAL_UART_Transmit(
		&huart1,
		pszMessage,
		_countof(pszMessage),
		HAL_MAX_DELAY
	);

	uint8_t pszCount[3] = { 0, 0, 0 };

	uint8_t pszPs[] = "\r\nInput > ";
	do
	{
		HAL_UART_Transmit(
			&huart1,
			pszPs,
			_countof(pszPs),
			HAL_MAX_DELAY
		);

		HAL_UART_Receive(
			&huart1,
			pszCount,
			_countof(pszCount) - 1,
			HAL_MAX_DELAY
		);

		HAL_UART_Transmit(
			&huart1,
			pszCount,
			_countof(pszCount) - 1,
			HAL_MAX_DELAY
		);

		PrintNewLine();

	} while(!IsNumber(pszCount));

	return (uint8_t)atoi(pszCount);
}

void BlinkNTimesWithDelay(
	uint8_t  uiBlinks,
	uint32_t uiDelay /* mseconds */
)
{
	for (uint8_t i = 0; i < uiBlinks; i++)
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
		HAL_Delay(uiDelay / 2);
	}
}

uint32_t atoi(uint8_t* pszNumber)
{
    uint32_t uiResult = 0;

    for (int i = 0; pszNumber[i]; i++) {
    	uiResult = uiResult * 10 + (pszNumber[i] - '0');
    }

    return uiResult;
}


_Bool IsDigit(uint8_t uiByte)
{
	return (_Bool)('0' <= uiByte && uiByte <= '9');
}


_Bool IsNumber(uint8_t* pszString)
{
	for(uint8_t i = 0; pszString[i]; i++)
	{
		if (!IsDigit(pszString[i])) {
			return (_Bool)0;
		}
	}
	return (_Bool)1;
}


void PrintNewLine()
{
	uint8_t pszNewLine[] = { '\r', '\n' };

	HAL_UART_Transmit(
		&huart1,
		pszNewLine,
		_countof(pszNewLine),
		HAL_MAX_DELAY
	);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
void assert_failed(char *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
