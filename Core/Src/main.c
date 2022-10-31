/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "sensorIO.h"
#include "stdio.h"
#include "string.h"
#include "helper.h"
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
I2C_HandleTypeDef hi2c2;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
osThreadId BlinkLedHandle;
osThreadId RnT_SensorHandle;
osThreadId Int_ButHandle;
osThreadId Fire_FluxerHandle;
osThreadId Mode_InitHandle;
/* USER CODE BEGIN PV */
int mode = 0; // 0 corresponds to exploration and 1 to battle
int state = 0; // 0 corresponds to normal and 1 to warning
int button_press = 0; //1 means button has been pressed
int battery_level = 10;

TaskHandle_t Int_ButHandle = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartBlinkLed(void const * argument);
void Start_RnT_Sensor(void const * argument);
void StartInt_But(void const * argument);
void StartFire_Fluxer(void const * argument);
void StartMode_Init(void const * argument);

/* USER CODE BEGIN PFP */
//#ifdef __GNUC__
//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif
//
//PUTCHAR_PROTOTYPE
//{
//  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
//  return ch;
//}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//extern void initialise_monitor_handles(void);
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	//	initialise_monitor_handles();
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
	MX_I2C2_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	BSP_ACCELERO_Init();
	BSP_TSENSOR_Init();
	BSP_GYRO_Init();
	BSP_PSENSOR_Init();
	BSP_MAGNETO_Init();
	BSP_HSENSOR_Init();
	/* USER CODE END 2 */

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

	/* definition and creation of BlinkLed */
	osThreadDef(BlinkLed, StartBlinkLed, osPriorityNormal, 0, 128);
	BlinkLedHandle = osThreadCreate(osThread(BlinkLed), NULL);

	/* definition and creation of RnT_Sensor */
	osThreadDef(RnT_Sensor, Start_RnT_Sensor, osPriorityBelowNormal, 0, 512);
	RnT_SensorHandle = osThreadCreate(osThread(RnT_Sensor), NULL);

	/* definition and creation of Int_But */
	osThreadDef(Int_But, StartInt_But, osPriorityAboveNormal, 0, 128);
	Int_ButHandle = osThreadCreate(osThread(Int_But), NULL);

	/* definition and creation of Fire_Fluxer */
	osThreadDef(Fire_Fluxer, StartFire_Fluxer, osPriorityNormal, 0, 128);
	Fire_FluxerHandle = osThreadCreate(osThread(Fire_Fluxer), NULL);

	/* definition and creation of Mode_Init */
	osThreadDef(Mode_Init, StartMode_Init, osPriorityNormal, 0, 128);
	Mode_InitHandle = osThreadCreate(osThread(Mode_Init), NULL);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
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

	/** Configure LSE Drive Capability
	 */
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.MSIState = RCC_MSI_ON;
	RCC_OscInitStruct.MSICalibrationValue = 0;
	RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}

	/** Enable MSI Auto calibration
	 */
	HAL_RCCEx_EnableMSIPLLMode();
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void)
{

	/* USER CODE BEGIN I2C2_Init 0 */

	/* USER CODE END I2C2_Init 0 */

	/* USER CODE BEGIN I2C2_Init 1 */

	/* USER CODE END I2C2_Init 1 */
	hi2c2.Instance = I2C2;
	hi2c2.Init.Timing = 0x00000E14;
	hi2c2.Init.OwnAddress1 = 0;
	hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c2.Init.OwnAddress2 = 0;
	hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
	hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c2) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Analogue filter
	 */
	if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Digital filter
	 */
	if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C2_Init 2 */

	/* USER CODE END I2C2_Init 2 */

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void)
{

	/* USER CODE BEGIN USART1_Init 0 */
	//    __HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
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
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
	/* USER CODE BEGIN 5 */
	const portTickType freq_button_sleep = 700;
	/* Infinite loop */
	for(;;)
	{
		if(button_press > 0)
		{
			vTaskDelay(freq_button_sleep);
			vTaskResume(Int_ButHandle);
		}
		vTaskSuspend(NULL);
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartBlinkLed */
/**
 * @brief Function implementing the BlinkLed thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartBlinkLed */
void StartBlinkLed(void const * argument)
{
	/* USER CODE BEGIN StartBlinkLed */
	portTickType lastRun;
	const portTickType frequency_battle = 500;
	const portTickType frequency_warning = 166;
	lastRun = xTaskGetTickCount();
	/* Infinite loop */
	for(;;)
	{
		if(mode == 1 && state == 0)
		{
			HAL_GPIO_TogglePin(GPIOB, LED_Pin);
			vTaskDelayUntil(&lastRun,frequency_battle);
		}
		else if(state == 1)
		{
			HAL_GPIO_TogglePin(GPIOB, LED_Pin);
			vTaskDelayUntil(&lastRun,frequency_warning);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, LED_Pin, GPIO_PIN_SET);
			vTaskDelayUntil(&lastRun,frequency_warning);
		}

	}
	/* USER CODE END StartBlinkLed */
}

/* USER CODE BEGIN Header_Start_RnT_Sensor */
/**
 * @brief Function implementing the RnT_Sensor thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Start_RnT_Sensor */
void Start_RnT_Sensor(void const * argument)
{
	/* USER CODE BEGIN Start_RnT_Sensor */
	portTickType lastRun;
	const portTickType frequency_transmit = 1000;
	lastRun = xTaskGetTickCount();

	char message_exploration[90];
	char message_battle1[64];
	char message_battle2[64];
	char message_warning[32];
	char message_battery[32];

	float accel_data[3];
	float mag_data[3];
	float gyro_data[3];
	float temp_data[1];
	float pres_data[1];
	float hum_data[1];

	int threshold_status[6] = {0};
	int nb_exceeded[1] = {0};
	float threshold_values[6] = {20, 500, 99, 15, 1010, 35};

	/* Infinite loop */
	for(;;)
	{
		if(mode == 0 && state == 0)
		{
			Read_Gyro(gyro_data);
			Read_Hum(hum_data);
			Read_Mag(mag_data);
			Read_Pres(pres_data);

			sprintf(message_exploration, "G: %2.2f(deg/s), M: %2.2f (g),  P: %2.2f (hPa), H: %2.2f (%%rH)\r\n", gyro_data[2], mag_data[2], pres_data[0], hum_data[0]);
			HAL_UART_Transmit(&huart1, (uint8_t*)message_exploration, strlen(message_exploration), 0xFF);

			float sensor_reading[6] = {0, gyro_data[2], hum_data[0], mag_data[2], pres_data[0], 0};
			Check_Threshold(nb_exceeded, sensor_reading, threshold_values, threshold_status);

			if(nb_exceeded[0] >= 2)
			{
				state = 1;
				nb_exceeded[0]=0;
				memset(threshold_status, 0, sizeof(threshold_status));
			}

			vTaskDelayUntil(&lastRun,frequency_transmit);

		}
		else if(mode == 1 && state == 0)
		{
			Read_Acc(accel_data);
			Read_Gyro(gyro_data);
			Read_Hum(hum_data);
			Read_Mag(mag_data);
			Read_Pres(pres_data);
			Read_Temp(temp_data);

			sprintf(message_battle1, "T:%2.2f (deg C), P: %2.2f (hPa), H: %2.2f (%%rH), ", temp_data[0],  pres_data[0],  hum_data[0]);
			HAL_UART_Transmit(&huart1, (uint8_t*)message_battle1, strlen(message_battle1), 0xFFFF);
			sprintf(message_battle2, "A: %2.2f (m/s^2), G: %2.2f(deg/s), M: %2.2f (g)\r", accel_data[2], gyro_data[2], mag_data[2]);
			HAL_UART_Transmit(&huart1, (uint8_t*)message_battle2, strlen(message_battle2), 0xFFFF);
			sprintf(message_battery, "Battery level: %d/10\r\n", battery_level);
			HAL_UART_Transmit(&huart1, (uint8_t*)message_battery, strlen(message_battery), 0xFFFF);

			float sensor_reading[6] = {accel_data[2], gyro_data[2], hum_data[0], mag_data[2], pres_data[0], temp_data[0]};
			Check_Threshold(nb_exceeded, sensor_reading, threshold_values, threshold_status);

			if(nb_exceeded[0] >= 1)
			{
				state = 1;
				nb_exceeded[0]=0;
				memset(threshold_status, 0, sizeof(threshold_status));
			}

			vTaskDelayUntil(&lastRun,frequency_transmit);
		}
		else
		{
			if(mode == 0)
			{
				sprintf(message_warning, "WARNING mode: SOS\r\n");
			}
			else
			{
				sprintf(message_warning, "BATTLE mode: SOS\r\n");
			}

			HAL_UART_Transmit(&huart1, (uint8_t*)message_warning, strlen(message_warning), 0xFFFF);
			vTaskDelayUntil(&lastRun,frequency_transmit);
		}
	}
	/* USER CODE END Start_RnT_Sensor */
}

/* USER CODE BEGIN Header_StartInt_But */
/**
 * @brief Function implementing the Int_But thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartInt_But */
void StartInt_But(void const * argument)
{
	/* USER CODE BEGIN StartInt_But */
	portTickType lastRun;
	lastRun = 0;
	/* Infinite loop */
	for(;;)
	{
		vTaskSuspend(NULL);
		if((xTaskGetTickCount() - lastRun) > 500)
		{
			if(button_press == 0)
			{
				button_press = 1;
				lastRun = xTaskGetTickCount();
				vTaskResume(defaultTaskHandle);
			}
			else if(button_press == 1 && state == 1)
			{
				state = 0;
				button_press = 0;
			}
			else if(button_press == 2 && state == 0)
			{
				mode = !mode;
				button_press = 0;
				vTaskResume(Mode_InitHandle);
			}
			else if(button_press == 1 && state == 0 && battery_level < 10)
			{
				battery_level += 1;
			}
			else
			{
				button_press = 0;
			}
		}
		else
		{
			button_press += 1;
		}
	}
	/* USER CODE END StartInt_But */
}

/* USER CODE BEGIN Header_StartFire_Fluxer */
/**
 * @brief Function implementing the Fire_Fluxer thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartFire_Fluxer */
void StartFire_Fluxer(void const * argument)
{
	/* USER CODE BEGIN StartFire_Fluxer */
	portTickType lastRun;
	const portTickType frequency_fire = 5000;
	lastRun = xTaskGetTickCount();

	char message_fluxer[40];
	sprintf(message_fluxer, "Fluxer is fired! Death to the Zurks!\r\n");

	/* Infinite loop */
	for(;;)
	{
		if(battery_level > 1 && mode == 1 && state == 0)
		{
			HAL_UART_Transmit(&huart1, (uint8_t*)message_fluxer, strlen(message_fluxer), 0xFFFF);
			battery_level -= 2;
		}
		vTaskDelayUntil(&lastRun,frequency_fire);
	}
	/* USER CODE END StartFire_Fluxer */
}

/* USER CODE BEGIN Header_StartMode_Init */
/**
 * @brief Function implementing the Mode_Init thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartMode_Init */
void StartMode_Init(void const * argument)
{
	/* USER CODE BEGIN StartMode_Init */
	char message_start[32];
	/* Infinite loop */
	for(;;)
	{
		if(mode == 0)
		{
			sprintf(message_start, "Entering exploration mode\r\n");
		}
		else if(mode == 1)
		{
			sprintf(message_start, "Entering battle mode\r\n");
		}
		HAL_UART_Transmit(&huart1, (uint8_t*)message_start, strlen(message_start), 0xFFFF);
		vTaskSuspend(NULL);
	}
	/* USER CODE END StartMode_Init */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

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
