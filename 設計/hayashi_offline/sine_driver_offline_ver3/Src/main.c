/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
volatile unsigned short* gpiob_idr = (unsigned short*)0x40020410;
volatile unsigned int *ccr1 = (unsigned int*)0x40010034;
volatile unsigned int *ccr2 = (unsigned int*)0x40010038;
volatile unsigned int *ccr3 = (unsigned int*)0x4001003C;
volatile unsigned int* tim2_cr1 = (unsigned int*)0x40000000;
volatile unsigned int* tim2_cnt = (unsigned int*)0x40000024;
volatile unsigned int* tim3_cr1 = (unsigned int*)0x40000400;
volatile unsigned int* tim3_cnt = (unsigned int*)0x40000424;
volatile unsigned int* tim3_psc = (unsigned int*)0x40000428;

int stateangle = 0;
int angleout = 0;
int freq = 0;
int accel = 0;
int current = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_ADC1_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
                                    
void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
                                

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  volatile unsigned int *ccer = (unsigned int*)0x40010020;
  *ccer |= (unsigned int)0x0555;           //ch1,1N,2,2N,3,3N enabled
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIM_Base_Start_IT(&htim2);

  uint16_t adc_data[2];
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_data, 2) != HAL_OK) {
		  HAL_Delay(500);
		  HAL_ADC_Stop_DMA(&hadc1);
	  }
	  accel = adc_data[0];
	  current = adc_data[1];

//	  if(accel < 40) {
//	      *ccer &= (unsigned int)0xFAAA;
//	  } else {
//		  *ccer |= (unsigned int)0x0555;
//	  }
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* ADC1 init function */
static void MX_ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/* TIM1 init function */
static void MX_TIM1_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = 4095;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 1;
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 100;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  HAL_TIM_MspPostInit(&htim1);

}

/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 8388608;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  if (HAL_TIM_OnePulse_Init(&htim2, TIM_OPMODE_SINGLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA2   ------> USART2_TX
     PA3   ------> USART2_RX
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : sensU_Pin sensV_Pin sensW_Pin */
  GPIO_InitStruct.Pin = sensU_Pin|sensV_Pin|sensW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI2_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	static int Uout[384] = {2048,2106,2164,2222,2280,2338,2396,2453,2511,2568,2626,2683,2740,2797,2853,2910,2966,3022,3078,3133,3188,3243,3298,3352,3405,3459,3512,3565,3617,3669,3720,3771,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096,4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096,4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3771,3720,3669,3617,3565,3512,3459,3405,3352,3298,3243,3188,3133,3078,3022,2966,2910,2853,2797,2740,2683,2626,2568,2511,2453,2396,2338,2280,2222,2164,2106,2048,1990,1932,1874,1816,1758,1700,1643,1585,1528,1470,1413,1356,1299,1243,1186,1130,1074,1018,963,908,853,798,744,691,637,584,531,479,427,376,325,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0,0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0,0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,325,376,427,479,531,584,637,691,744,798,853,908,963,1018,1074,1130,1186,1243,1299,1356,1413,1470,1528,1585,1643,1700,1758,1816,1874,1932,1990};
	static int Vout[384] = {4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3771,3720,3669,3617,3565,3512,3459,3405,3352,3298,3243,3188,3133,3078,3022,2966,2910,2853,2797,2740,2683,2626,2568,2511,2453,2396,2338,2280,2222,2164,2106,2048,1990,1932,1874,1816,1758,1700,1643,1585,1528,1470,1413,1356,1299,1243,1186,1130,1074,1018,963,908,853,798,744,691,637,584,531,479,427,376,325,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0,0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0,0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,325,376,427,479,531,584,637,691,744,798,853,908,963,1018,1074,1130,1186,1243,1299,1356,1413,1470,1528,1585,1643,1700,1758,1816,1874,1932,1990,2048,2106,2164,2222,2280,2338,2396,2453,2511,2568,2626,2683,2740,2797,2853,2910,2966,3022,3078,3133,3188,3243,3298,3352,3405,3459,3512,3565,3617,3669,3720,3771,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096,4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096};
	static int Wout[384] = {0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0,0,0,1,2,4,7,10,13,18,22,27,33,39,46,53,61,70,79,88,98,109,120,131,143,156,169,183,197,211,226,242,258,274,325,376,427,479,531,584,637,691,744,798,853,908,963,1018,1074,1130,1186,1243,1299,1356,1413,1470,1528,1585,1643,1700,1758,1816,1874,1932,1990,2048,2106,2164,2222,2280,2338,2396,2453,2511,2568,2626,2683,2740,2797,2853,2910,2966,3022,3078,3133,3188,3243,3298,3352,3405,3459,3512,3565,3617,3669,3720,3771,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096,4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3838,3854,3870,3885,3899,3913,3927,3940,3953,3965,3976,3987,3998,4008,4017,4026,4035,4043,4050,4057,4063,4069,4074,4078,4083,4086,4089,4092,4094,4095,4096,4096,4096,4095,4094,4092,4089,4086,4083,4078,4074,4069,4063,4057,4050,4043,4035,4026,4017,4008,3998,3987,3976,3965,3953,3940,3927,3913,3899,3885,3870,3854,3838,3822,3771,3720,3669,3617,3565,3512,3459,3405,3352,3298,3243,3188,3133,3078,3022,2966,2910,2853,2797,2740,2683,2626,2568,2511,2453,2396,2338,2280,2222,2164,2106,2048,1990,1932,1874,1816,1758,1700,1643,1585,1528,1470,1413,1356,1299,1243,1186,1130,1074,1018,963,908,853,798,744,691,637,584,531,479,427,376,325,274,258,242,226,211,197,183,169,156,143,131,120,109,98,88,79,70,61,53,46,39,33,27,22,18,13,10,7,4,2,1,0};

	static int startcnt = 0;
	int voltageout = 0;

	if(htim->Instance==TIM1) {
		/*tim1 interrupt*/
		voltageout = accel;
		*ccr1 = Uout[angleout] * voltageout >> 12;
		*ccr2 = Vout[angleout] * voltageout >> 12;
		*ccr3 = Wout[angleout] * voltageout >> 12;
		int hallsensor = *gpiob_idr & 0b111;
		if(hallsensor == 0) {
			angleout = startcnt >> 17;
			startcnt = (++startcnt % 0x3000000);
		} else {
			angleout = (stateangle + (freq * (*tim2_cnt + 4096) >> 17)) % 384;
		}

	}

	if(htim->Instance==TIM2) {
		/*tim2 interrupt*/
		freq = 0;
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  static int state[] = {0b100, 0b110, 0b010, 0b011, 0b001, 0b101};
//  static int state[] = {0b101, 0b001, 0b011, 0b010, 0b110, 0b100};	//reverse
  static int state_angle[] = {0, 64, 128, 192, 256, 320};
  static int state_num = 0;

  int hallsensor = *gpiob_idr & 0b111;
  if(hallsensor == state[state_num]) {
	  if((*tim2_cr1 & 1) == 1) {			//if tim2 enable
		  freq = 0x800000 / *tim2_cnt;
	  } else {
		  *tim2_cr1 |= 1;				//tim2 start
	  }
	  int temp = state_angle[state_num] + OFFSET;
	  stateangle = temp;
	  angleout = temp;
	  *tim2_cnt = 0;
	  state_num = (++state_num % 6);
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
