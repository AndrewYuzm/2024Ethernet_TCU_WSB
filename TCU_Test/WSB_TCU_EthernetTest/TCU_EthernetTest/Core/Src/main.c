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
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
/* USER CODE BEGIN PV */
struct tcp_pcb *client_pcb = NULL;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_CAN1_Init(void);
void StartDefaultTask(void const * argument);

/* USER CODE BEGIN PFP */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    //具体哪个串口可以更改huart1为其它串�?????????
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1 , 0xffff);
    return ch;
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void configCANFilters2(CAN_HandleTypeDef* canHandle){
	CAN_FilterTypeDef  sFilterConfig;

	    // Filter msgs to this nodes Id to fifo 0
	    uint32_t filterID = 1<<8;//CAN_NODE_ADDRESS
	    filterID = filterID << 3; // Filter ID is left aligned to 32 bits
	    uint32_t filterMask = 0xFF00;
	    filterMask = filterMask << 3; // Filter masks are also left aligned to 32 bits
	    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	    sFilterConfig.FilterIdHigh = (filterID>>16) & 0xFFFF;
	    sFilterConfig.FilterIdLow = (filterID & 0xFFFF);
	    sFilterConfig.FilterMaskIdHigh = (filterMask>>16) & 0xFFFF;
	    sFilterConfig.FilterMaskIdLow = (filterMask & 0xFFFF);
	    sFilterConfig.FilterFIFOAssignment = 0;
	    sFilterConfig.FilterActivation = ENABLE;
	    sFilterConfig.FilterBank = 0;

	    //From the reference manual, it seems that setting SlaveStartFilterBank to 0 means all filters are used for the enabled CAN peripheral
	    //TODO: Verify this is the correct config
	    sFilterConfig.SlaveStartFilterBank = 0;
	    if(HAL_CAN_ConfigFilter(canHandle, &sFilterConfig) != HAL_OK)
	    {
	        Error_Handler();
	    }

	    // Filter msgs to the broadcast Id to fifo 0
	    filterID = 0xFF<<8;
	    filterID = filterID << 3; // Filter ID is left aligned to 32 bits
	    filterMask = 0xFF00;
	    filterMask = filterMask << 3; // Filter masks are also left aligned to 32 bits
	    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	    sFilterConfig.FilterIdHigh = (filterID>>16) & 0xFFFF;
	    sFilterConfig.FilterIdLow = (filterID & 0xFFFF);
	    sFilterConfig.FilterMaskIdHigh = (filterMask>>16) & 0xFFFF;
	    sFilterConfig.FilterMaskIdLow = (filterMask & 0xFFFF);


	    sFilterConfig.FilterFIFOAssignment = 0;
	    sFilterConfig.FilterActivation = ENABLE;
	    sFilterConfig.FilterBank = 1;

	    //From the reference manual, it seems that setting SlaveStartFilterBank to 0 means all filters are used for the enabled CAN peripheral
	    //TODO: Verify this is the correct config
	    sFilterConfig.SlaveStartFilterBank = 0;
	    if(HAL_CAN_ConfigFilter(canHandle, &sFilterConfig) != HAL_OK)
	    {
	        Error_Handler();
	    }

	    // Filter msgs to the broadcast Id to fifo 0
	    filterID = 1<<12;
	    filterID = filterID << 3; // Filter ID is left aligned to 32 bits
	    filterMask = 0xFF00;
	    filterMask = filterMask << 3; // Filter masks are also left aligned to 32 bits
	    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	    sFilterConfig.FilterIdHigh = (filterID>>16) & 0xFFFF;
	    sFilterConfig.FilterIdLow = (filterID & 0xFFFF);
	    sFilterConfig.FilterMaskIdHigh = (filterMask>>16) & 0xFFFF;
	    sFilterConfig.FilterMaskIdLow = (filterMask & 0xFFFF);
	    sFilterConfig.FilterFIFOAssignment = 0;
	    sFilterConfig.FilterActivation = ENABLE;
	    sFilterConfig.FilterBank = 3;

	    sFilterConfig.SlaveStartFilterBank = 0;

	    //From the reference manual, it seems that setting SlaveStartFilterBank to 0 means all filters are used for the enabled CAN peripheral
	    //TODO: Verify this is the correct config
	    if(HAL_CAN_ConfigFilter(canHandle, &sFilterConfig) != HAL_OK)
	    {
	        Error_Handler();
	    }
}
void configCANFilters(CAN_HandleTypeDef* canHandle){	//Without filtered any CAN msgs

    CAN_FilterTypeDef sFilterConfig;

    // Configure the filter to accept all messages
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = 0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.FilterBank = 0;  // Use filter bank 0

    // From the reference manual, it seems that setting SlaveStartFilterBank to 0 means all filters are used for the enabled CAN peripheral
    // TODO: Verify this is the correct config
    sFilterConfig.SlaveStartFilterBank = 0;

    if (HAL_CAN_ConfigFilter(canHandle, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }
}

uint16_t fifo0=0,fifo1=0;
char globalmsg[128];	//transfer msgs to string
int msg_len;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	fifo0++;
    CAN_RxHeaderTypeDef   RxHeader;
    uint8_t               RxData[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
//    	msg_len = snprintf(globalmsg, sizeof(globalmsg),
//			   "CAN Rx - ID: 0x%03X, DLC: %d, Data: %02X %02X %02X %02X %02X %02X %02X %02X\n",
//			   RxHeader.ExtId, RxHeader.DLC,
//			   RxData[0], RxData[1], RxData[2], RxData[3],
//			   RxData[4], RxData[5], RxData[6], RxData[7]);	//full

//    	msg_len = snprintf(globalmsg, sizeof(globalmsg),
//    				   ",%08X%02X%02X%02X%02X%02X%02X%02X%02X\n",
//    				   RxHeader.ExtId,
//    				   RxData[0], RxData[1], RxData[2], RxData[3],
//    				   RxData[4], RxData[5], RxData[6], RxData[7]);	//real
//    	HAL_UART_Transmit(&huart1, (uint8_t *)globalmsg, msg_len, HAL_MAX_DELAY);

    	msg_len = snprintf(globalmsg, sizeof(globalmsg),
    	    				   "%02X%02X%02X%02X%02X%02X%02X%02X",
    	    				   RxData[0], RxData[1], RxData[2], RxData[3],
    	    				   RxData[4], RxData[5], RxData[6], RxData[7]);

    	add_or_update_data(RxHeader.ExtId, globalmsg);
    }
    else {
        uint8_t msg[] = "Failed to receive CAN message from FIFO0\n";
        HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
        // handleError();
    }

    /*
        This check is essential as it was causing issues with our brake light flashing and our button presses were getting random values.
        The cause is the motor controllers who send messages with standard ID lengths. So since we are passing in RxHeader.ExtId
        the data that was being received changed but the id did not so it would call the callback of the last extended message that was processed.
        This is why we would get brake light values of 255 and button presses with multiple bits high even though that is impossible from our code.
        Props to Joseph Borromeo for squashing this 5 year old bug
    */
    // if (RxHeader.IDE == CAN_ID_EXT){  // Only parse data if it is an extended CAN frame
    //         // if (parseCANData(RxHeader.ExtId, RxData) != HAL_OK) {
    //         //     /*ERROR_PRINT_ISR("Failed to parse CAN message id 0x%lX", RxHeader.ExtId);*/
    //         // }
    // }
}

//Currently not used (we use FIFO0)
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	fifo1++;
    CAN_RxHeaderTypeDef   RxHeader;
    uint8_t               RxData[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &RxHeader, RxData) != HAL_OK)
    {
        uint8_t msg[] = "Failed to receive CAN message from FIFO1\n";
        HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
        // handleError();
    }

    // if (RxHeader.IDE == CAN_ID_EXT){  // Only parse data if it is an extended CAN frame
    //         if (parseCANData(RxHeader.ExtId, RxData) != HAL_OK) {
    //             /*ERROR_PRINT_ISR("Failed to parse CAN message id 0x%lX", RxHeader.ExtId);*/
    //         }
    // }
}
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
  MX_CAN1_Init();
  /* USER CODE BEGIN 2 */
  configCANFilters(&hcan1);
  if (HAL_CAN_Start(&hcan1) != HAL_OK) {
         uint8_t msg[] = "Failed to start CAN!\n";
         HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
     return HAL_ERROR;
   }

   if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
   {
         uint8_t msg[] = "Error starting to listen for CAN msgs from FIFO0\n";
         HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
       return HAL_ERROR;
   }

   if (HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
   {
         uint8_t msg[] = "Error starting to listen for CAN msgs from FIFO0\n";
         HAL_UART_Transmit(&huart1, msg, sizeof(msg), 100);
       return HAL_ERROR;
   }
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 80;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 4;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

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
    int soctemp=create_tcp_client(); //$
  //  double t=0,y=0;
    int t=0,y=0;
    uint8_t send_buf[1550];
  //  uint8_t send_buf[30];
    void init_dataItems();
    /* Infinite loop */

    for(;;){
    	printf("CAN: 0:%u,1:%u\r\n",fifo0,fifo1);
    	tcp_client_send(soctemp, get_all_data_str());
    	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    	osDelay(500);
    }




    for(;;)
    {

    	printf("CAN: 0:%u,1:%u\r\n",fifo0,fifo1);
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
  	//发�?�数据到服务�?????????
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
