/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#ifndef HIDE_CUBEMX_FREERTOS
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticTimer_t osStaticTimerDef_t;
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for ctrlTask */
osThreadId_t ctrlTaskHandle;
uint32_t ctrlTaskBuffer[ 512 ];
osStaticThreadDef_t ctrlTaskControlBlock;
const osThreadAttr_t ctrlTask_attributes = {
  .name = "ctrlTask",
  .cb_mem = &ctrlTaskControlBlock,
  .cb_size = sizeof(ctrlTaskControlBlock),
  .stack_mem = &ctrlTaskBuffer[0],
  .stack_size = sizeof(ctrlTaskBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for idleTask */
osThreadId_t idleTaskHandle;
uint32_t idleTaskBuffer[ 128 ];
osStaticThreadDef_t idleTaskControlBlock;
const osThreadAttr_t idleTask_attributes = {
  .name = "idleTask",
  .cb_mem = &idleTaskControlBlock,
  .cb_size = sizeof(idleTaskControlBlock),
  .stack_mem = &idleTaskBuffer[0],
  .stack_size = sizeof(idleTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ctrlTaskQueue */
osMessageQueueId_t ctrlTaskQueueHandle;
uint8_t ctrlTaskQueueBuffer[ 16 * sizeof( uint16_t ) ];
osStaticMessageQDef_t ctrlTaskQueueControlBlock;
const osMessageQueueAttr_t ctrlTaskQueue_attributes = {
  .name = "ctrlTaskQueue",
  .cb_mem = &ctrlTaskQueueControlBlock,
  .cb_size = sizeof(ctrlTaskQueueControlBlock),
  .mq_mem = &ctrlTaskQueueBuffer,
  .mq_size = sizeof(ctrlTaskQueueBuffer)
};
/* Definitions for testTimer */
osTimerId_t testTimerHandle;
osStaticTimerDef_t testTimerControlBlock;
const osTimerAttr_t testTimer_attributes = {
  .name = "testTimer",
  .cb_mem = &testTimerControlBlock,
  .cb_size = sizeof(testTimerControlBlock),
};
/* Definitions for oneShotTimer */
osTimerId_t oneShotTimerHandle;
osStaticTimerDef_t oneShotTimerControlBlock;
const osTimerAttr_t oneShotTimer_attributes = {
  .name = "oneShotTimer",
  .cb_mem = &oneShotTimerControlBlock,
  .cb_size = sizeof(oneShotTimerControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartCtrlTask(void *argument);
void StartIdleTask(void *argument);
void testTimerCallback(void *argument);
void oneShotTimerCallback(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
}
/* USER CODE END 2 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
{
   /* Run time stack overflow checking is performed if
   configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
   called if a stack overflow is detected. */
}
/* USER CODE END 4 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of testTimer */
  testTimerHandle = osTimerNew(testTimerCallback, osTimerPeriodic, NULL, &testTimer_attributes);

  /* creation of oneShotTimer */
  oneShotTimerHandle = osTimerNew(oneShotTimerCallback, osTimerOnce, NULL, &oneShotTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of ctrlTaskQueue */
  ctrlTaskQueueHandle = osMessageQueueNew (16, sizeof(uint16_t), &ctrlTaskQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ctrlTask */
  ctrlTaskHandle = osThreadNew(StartCtrlTask, NULL, &ctrlTask_attributes);

  /* creation of idleTask */
  idleTaskHandle = osThreadNew(StartIdleTask, NULL, &idleTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartCtrlTask */
/**
  * @brief  Function implementing the ctrlTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartCtrlTask */
void StartCtrlTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartCtrlTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartCtrlTask */
}

/* USER CODE BEGIN Header_StartIdleTask */
/**
* @brief Function implementing the idleTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartIdleTask */
void StartIdleTask(void *argument)
{
  /* USER CODE BEGIN StartIdleTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartIdleTask */
}

/* testTimerCallback function */
void testTimerCallback(void *argument)
{
  /* USER CODE BEGIN testTimerCallback */

  /* USER CODE END testTimerCallback */
}

/* oneShotTimerCallback function */
void oneShotTimerCallback(void *argument)
{
  /* USER CODE BEGIN oneShotTimerCallback */

  /* USER CODE END oneShotTimerCallback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
#endif
/* USER CODE END Application */

