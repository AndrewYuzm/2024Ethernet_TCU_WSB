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
#include "cmsis_os.h"
#include "lwip.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tcpclient.h"
#include "math.h"
#include "stdio.h"
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
UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
struct tcp_pcb *client_pcb = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    //具体哪个串口可以更改huart1为其它串�???????
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1 , 0xffff);
    return ch;
}
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
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 1024);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
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

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

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
  /* init code for LWIP */
  MX_LWIP_Init();
  /* USER CODE BEGIN 5 */
  //  TCP_Client_Init();
    int soctemp=create_tcp_client();
  //  double t=0,y=0;
    int t=0,y=0;
    uint8_t send_buf[1550];
  //  uint8_t send_buf[30];
    void init_dataItems();
    /* Infinite loop */
    for(;;)
    {
  //	sprintf(send_buf,"{\"t\": %f, \"y\": %f}\n{\"t\": %f, \"y\": %f}\n{\"t\": %f, \"y\": %f}\n{\"t\": %f, \"y\": %f}\n",t,y,t,y,t,y,t,y);
  //	sprintf(send_buf,"{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n",t,y,t,y,t,y,t,y);
//  	sprintf(send_buf,
//  			",0x000000010000000000000001,0x000000020000000000000002,0x000000030000000000000003,0x000000040000000000000004\n"
//  			",0x000000010000000000000002,0x000000020000000000000003,0x000000030000000000000006,0x000000040000000000000004\n"
//  			",0x000000010000000000000003,0x000000020000000000000004,0x000000030000000000000009,0x000000040000000000000004\n"
//  			",0x000000010000000000000004,0x000000020000000000000005,0x0000000300000000000000012,0x000000040000000000000004\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
  //			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
  //			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
  //			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
  //			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			"12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n12345678901234567890123456789012345678901234567890\n"
//  			);


  //	sprintf(send_buf,"{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n{\"t\": %d, \"y\": %d}\n",t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y,t,y);
  //	sprintf(send_buf,"{\"t\": %d, \"y\": %d}\n",(int)t,(int)y);
  //	t+=0.05;
  //	if(t>4)t=0;
  //	y=sin(t*7);
  	t+=1;
  	if(t>100)t=0;
  	y=t*2;//(int)sin(t*7);
  	//发�?�数据到服务�???????
  //	send_json_data(client_pcb, send_buf);
  	add_or_update_data(1,1);add_or_update_data(2,2);add_or_update_data(3,3);add_or_update_data(4,4);
//  	tcp_client_send(soctemp, send_buf);
  	tcp_client_send(soctemp, get_all_data_str());
      osDelay(200);
      HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

//      sprintf(send_buf,
////        			",0x000000010000000000000001,0x000000020000000000000002,0x000000030000000000000003,0x000000040000000000000004\n"
//        			",0x000000010000000000000002,0x000000020000000000000003,0x000000030000000000000006,0x000000040000000000000004\n"
////        			",0x000000010000000000000003,0x000000020000000000000004,0x000000030000000000000009,0x000000040000000000000004\n"
////        			",0x000000010000000000000004,0x000000020000000000000005,0x0000000300000000000000012,0x000000040000000000000004\n"
//     			);

//        	tcp_client_send(soctemp, send_buf);
      add_or_update_data(1,2);add_or_update_data(2,4);add_or_update_data(3,6);add_or_update_data(4,4);
      tcp_client_send(soctemp, get_all_data_str());
            osDelay(200);
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

//            sprintf(send_buf,
//      //        			",0x000000010000000000000001,0x000000020000000000000002,0x000000030000000000000003,0x000000040000000000000004\n"
////              			",0x000000010000000000000002,0x000000020000000000000003,0x000000030000000000000006,0x000000040000000000000004\n"
//              			",0x000000010000000000000003,0x000000020000000000000004,0x000000030000000000000009,0x000000040000000000000004\n"
//      //        			",0x000000010000000000000004,0x000000020000000000000005,0x0000000300000000000000012,0x000000040000000000000004\n"
//           			);

//              	tcp_client_send(soctemp, send_buf);
            add_or_update_data(1,3);add_or_update_data(2,6);add_or_update_data(3,9);add_or_update_data(4,4);
            tcp_client_send(soctemp, get_all_data_str());
                  osDelay(200);
                  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);

//                  sprintf(send_buf,
//            //        			",0x000000010000000000000001,0x000000020000000000000002,0x000000030000000000000003,0x000000040000000000000004\n"
////                    			",0x000000010000000000000002,0x000000020000000000000003,0x000000030000000000000006,0x000000040000000000000004\n"
//            //        			",0x000000010000000000000003,0x000000020000000000000004,0x000000030000000000000009,0x000000040000000000000004\n"
//                    			",0x000000010000000000000004,0x000000020000000000000005,0x0000000300000000000000012,0x000000040000000000000004\n"
//                 			);

//                    	tcp_client_send(soctemp, send_buf);
                  add_or_update_data(1,4);add_or_update_data(2,8);add_or_update_data(3,12);add_or_update_data(4,4);
                  tcp_client_send(soctemp, get_all_data_str());
                        osDelay(200);
                        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);


    }
  /* USER CODE END 5 */
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
