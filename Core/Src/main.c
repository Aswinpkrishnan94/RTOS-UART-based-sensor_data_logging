/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "i2c.h"
#include "iwdg.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Sensor_Task.h"
#include "Logging_Task.h"
#include "Watchdog_Task.h"
#include "dht11_driver.h"
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
osThreadId_t SensorTaskHandle, LoggingTaskHandle, WatchdogTaskHandle;
osMessageQueueId_t SensorQueueHandle, uartQueueHandle;
osMutexId_t uartMutexHandle;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
void StartSensorTask(void *argument);
void StartLoggingTask(void *argument);
void StartWatchdogTask(void *argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
const osThreadAttr_t SensorQueueAttributes = {.name = "Sensor Queue"};
const osMessageQueueAttr_t uartQueue_attributes = { .name = "uartQueue" };
const osThreadAttr_t uartMutexAttributes = {.name = "UART Mutex"};
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
  MX_IWDG_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Transmit(&huart3, (uint8_t *)"UART Test Before RTOS\r\n", 24, HAL_MAX_DELAY);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* USER CODE BEGIN RTOS_MUTEX */
     /* add mutexes, ... */
     uartMutexHandle = osMutexNew(&uartMutexAttributes);
     /* USER CODE END RTOS_MUTEX */

     /* USER CODE BEGIN RTOS_SEMAPHORES */
     /* add semaphores, ... */
     /* USER CODE END RTOS_SEMAPHORES */

     /* USER CODE BEGIN RTOS_TIMERS */
     /* start timers, add new ones, ... */
     /* USER CODE END RTOS_TIMERS */

     /* USER CODE BEGIN RTOS_QUEUES */
     /* add queues, ... */
     SensorQueueHandle = osMessageQueueNew(10, sizeof(float), &SensorQueueAttributes);
     if (SensorQueueHandle == NULL)
     {
         HAL_UART_Transmit(&huart3, (uint8_t *)"Queue Creation Failed!\r\n", 25, HAL_MAX_DELAY);
     }
     uartQueueHandle = osMessageQueueNew(10, sizeof(char) * 50, &uartQueue_attributes);
     /* USER CODE END RTOS_QUEUES */

     /* Create the thread(s) */

     /* USER CODE BEGIN RTOS_THREADS */
     /* add threads, ... */
     const osThreadAttr_t SensorTaskAttributes = {.name = "Sensor Task",
   		  	  	  	  	  	  	  	  	  	   .stack_size = 128, .priority = osPriorityAboveNormal};

     SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTaskAttributes);
     if (SensorTaskHandle == NULL)
     {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Sensor Task Creation Failed!\r\n", 30, HAL_MAX_DELAY);
     }

     const osThreadAttr_t LoggingTaskAttributes = {.name = "Logging Task",
     		  	  	  	  	  	  	  	  	  	    .stack_size = 1024, .priority = osPriorityNormal};

     LoggingTaskHandle = osThreadNew(StartLoggingTask, NULL, &LoggingTaskAttributes);
     if (LoggingTaskHandle == NULL)
     {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Logging Task Creation Failed!\r\n", 32, HAL_MAX_DELAY);
     }

     const osThreadAttr_t WatchdogTaskAttributes = {.name = "Watchdog Task",
       		  	  	  	  	  	  	  	  	  	 .stack_size = 128, .priority = osPriorityHigh};

     WatchdogTaskHandle = osThreadNew(StartWatchdogTask, NULL, &WatchdogTaskAttributes);
     if (WatchdogTaskHandle == NULL)
     {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Logging Task Creation Failed!\r\n", 32, HAL_MAX_DELAY);
     }
     /* USER CODE END RTOS_THREADS */

     /* USER CODE BEGIN RTOS_EVENTS */
     /* add events, ... */
     /* USER CODE END RTOS_EVENTS */

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
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
