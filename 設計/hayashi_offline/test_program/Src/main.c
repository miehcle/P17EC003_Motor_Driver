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
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
int angle = 0;
int accel = 0;
int current = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_ADC1_Init(void);
                                    
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
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_ADC1_Init();

  /* USER CODE BEGIN 2 */
  volatile unsigned int *ccer = (unsigned int*)0x40010020;
  *ccer |= (unsigned int)0x0555;           //ch1,1N,2,2N,3,3N enabled
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_3);
  HAL_TIM_Base_Start_IT(&htim2);
  HAL_TIM_Base_Start_IT(&htim3);
  HAL_TIM_Base_Stop(&htim3);

  static uint16_t adc_data[2];
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
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

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_CENTERALIGNED1;
  htim1.Init.Period = 4095;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 1;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

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
  htim2.Init.Prescaler = 127;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
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

/* TIM3 init function */
static void MX_TIM3_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
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
*/
static void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : sensU_Pin sensV_Pin sensW_Pin */
  GPIO_InitStruct.Pin = sensU_Pin|sensV_Pin|sensW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
	static int Uout[768] = {2048,2077,2106,2135,2164,2193,2222,2251,2280,2309,2338,2367,2396,2425,2453,2482,2511,2540,2568,2597,2626,2654,2683,2712,2740,2768,2797,2825,2853,2882,2910,2938,2966,2994,3022,3050,3078,3105,3133,3161,3188,3216,3243,3270,3298,3325,3352,3379,3405,3432,3459,3485,3512,3538,3565,3591,3617,3643,3669,3695,3720,3746,3771,3796,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096,4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096,4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3796,3771,3746,3720,3695,3669,3643,3617,3591,3565,3538,3512,3485,3459,3432,3405,3379,3352,3325,3298,3270,3243,3216,3188,3161,3133,3105,3078,3050,3022,2994,2966,2938,2910,2882,2853,2825,2797,2768,2740,2712,2683,2654,2626,2597,2568,2540,2511,2482,2453,2425,2396,2367,2338,2309,2280,2251,2222,2193,2164,2135,2106,2077,2048,2019,1990,1961,1932,1903,1874,1845,1816,1787,1758,1729,1700,1671,1643,1614,1585,1556,1528,1499,1470,1442,1413,1384,1356,1328,1299,1271,1243,1214,1186,1158,1130,1102,1074,1046,1018,991,963,935,908,880,853,826,798,771,744,717,691,664,637,611,584,558,531,505,479,453,427,401,376,350,325,300,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,300,325,350,376,401,427,453,479,505,531,558,584,611,637,664,691,717,744,771,798,826,853,880,908,935,963,991,1018,1046,1074,1102,1130,1158,1186,1214,1243,1271,1299,1328,1356,1384,1413,1442,1470,1499,1528,1556,1585,1614,1643,1671,1700,1729,1758,1787,1816,1845,1874,1903,1932,1961,1990,2019};
	static int Vout[768] = {4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3796,3771,3746,3720,3695,3669,3643,3617,3591,3565,3538,3512,3485,3459,3432,3405,3379,3352,3325,3298,3270,3243,3216,3188,3161,3133,3105,3078,3050,3022,2994,2966,2938,2910,2882,2853,2825,2797,2768,2740,2712,2683,2654,2626,2597,2568,2540,2511,2482,2453,2425,2396,2367,2338,2309,2280,2251,2222,2193,2164,2135,2106,2077,2048,2019,1990,1961,1932,1903,1874,1845,1816,1787,1758,1729,1700,1671,1643,1614,1585,1556,1528,1499,1470,1442,1413,1384,1356,1328,1299,1271,1243,1214,1186,1158,1130,1102,1074,1046,1018,991,963,935,908,880,853,826,798,771,744,717,691,664,637,611,584,558,531,505,479,453,427,401,376,350,325,300,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,300,325,350,376,401,427,453,479,505,531,558,584,611,637,664,691,717,744,771,798,826,853,880,908,935,963,991,1018,1046,1074,1102,1130,1158,1186,1214,1243,1271,1299,1328,1356,1384,1413,1442,1470,1499,1528,1556,1585,1614,1643,1671,1700,1729,1758,1787,1816,1845,1874,1903,1932,1961,1990,2019,2048,2077,2106,2135,2164,2193,2222,2251,2280,2309,2338,2367,2396,2425,2453,2482,2511,2540,2568,2597,2626,2654,2683,2712,2740,2768,2797,2825,2853,2882,2910,2938,2966,2994,3022,3050,3078,3105,3133,3161,3188,3216,3243,3270,3298,3325,3352,3379,3405,3432,3459,3485,3512,3538,3565,3591,3617,3643,3669,3695,3720,3746,3771,3796,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096,4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096};
	static int Wout[768] = {0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0,0,0,0,1,1,2,2,3,4,6,7,8,10,12,13,15,18,20,22,25,27,30,33,36,39,43,46,50,53,57,61,66,70,74,79,83,88,93,98,103,109,114,120,125,131,137,143,150,156,162,169,176,183,190,197,204,211,219,226,234,242,250,258,266,274,300,325,350,376,401,427,453,479,505,531,558,584,611,637,664,691,717,744,771,798,826,853,880,908,935,963,991,1018,1046,1074,1102,1130,1158,1186,1214,1243,1271,1299,1328,1356,1384,1413,1442,1470,1499,1528,1556,1585,1614,1643,1671,1700,1729,1758,1787,1816,1845,1874,1903,1932,1961,1990,2019,2048,2077,2106,2135,2164,2193,2222,2251,2280,2309,2338,2367,2396,2425,2453,2482,2511,2540,2568,2597,2626,2654,2683,2712,2740,2768,2797,2825,2853,2882,2910,2938,2966,2994,3022,3050,3078,3105,3133,3161,3188,3216,3243,3270,3298,3325,3352,3379,3405,3432,3459,3485,3512,3538,3565,3591,3617,3643,3669,3695,3720,3746,3771,3796,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096,4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3830,3838,3846,3854,3862,3870,3877,3885,3892,3899,3906,3913,3920,3927,3934,3940,3946,3953,3959,3965,3971,3976,3982,3987,3993,3998,4003,4008,4013,4017,4022,4026,4030,4035,4039,4043,4046,4050,4053,4057,4060,4063,4066,4069,4071,4074,4076,4078,4081,4083,4084,4086,4088,4089,4090,4092,4093,4094,4094,4095,4095,4096,4096,4096,4096,4096,4095,4095,4094,4094,4093,4092,4090,4089,4088,4086,4084,4083,4081,4078,4076,4074,4071,4069,4066,4063,4060,4057,4053,4050,4046,4043,4039,4035,4030,4026,4022,4017,4013,4008,4003,3998,3993,3987,3982,3976,3971,3965,3959,3953,3946,3940,3934,3927,3920,3913,3906,3899,3892,3885,3877,3870,3862,3854,3846,3838,3830,3822,3796,3771,3746,3720,3695,3669,3643,3617,3591,3565,3538,3512,3485,3459,3432,3405,3379,3352,3325,3298,3270,3243,3216,3188,3161,3133,3105,3078,3050,3022,2994,2966,2938,2910,2882,2853,2825,2797,2768,2740,2712,2683,2654,2626,2597,2568,2540,2511,2482,2453,2425,2396,2367,2338,2309,2280,2251,2222,2193,2164,2135,2106,2077,2048,2019,1990,1961,1932,1903,1874,1845,1816,1787,1758,1729,1700,1671,1643,1614,1585,1556,1528,1499,1470,1442,1413,1384,1356,1328,1299,1271,1243,1214,1186,1158,1130,1102,1074,1046,1018,991,963,935,908,880,853,826,798,771,744,717,691,664,637,611,584,558,531,505,479,453,427,401,376,350,325,300,274,266,258,250,242,234,226,219,211,204,197,190,183,176,169,162,156,150,143,137,131,125,120,114,109,103,98,93,88,83,79,74,70,66,61,57,53,50,46,43,39,36,33,30,27,25,22,20,18,15,13,12,10,8,7,6,4,3,2,2,1,1,0,0};

	static volatile unsigned short* gpiob_idr = (unsigned short*)0x40020410;
	static volatile unsigned int* tim3_cr1 = (unsigned int*)0x40000400;
	static volatile unsigned int *ccr1 = (unsigned int*)0x40010034;
	static volatile unsigned int *ccr2 = (unsigned int*)0x40010038;
	static volatile unsigned int *ccr3 = (unsigned int*)0x4001003C;

	static int startcnt = 0;
	int angleout = 0;
	int voltageout = 0;

	if(htim->Instance==TIM1) {
		/*tim1 interrupt*/

		if((*tim3_cr1 & 1) != 1) {
			int hallsensor = (*gpiob_idr >> 3) & 0b111;
			switch(hallsensor) {
				case 0b100:
					angle = 0;
					break;
				case 0b110:
					angle = 128;
					break;
				case 0b010:
					angle = 256;
					break;
				case 0b011:
					angle = 384;
					break;
				case 0b001:
					angle = 512;
					break;
				case 0b101:
					angle = 640;
					break;
				default:
					angle = startcnt >> 16;
					startcnt = (++startcnt % 0x3000000);
					break;
			}
		}
		voltageout = accel;
		angleout = (angle + OFFSET) % 768;
		*ccr1 = Uout[angleout] * voltageout >> 12;
		*ccr2 = Vout[angleout] * voltageout >> 12;
		*ccr3 = Wout[angleout] * voltageout >> 12;
	}

	if(htim->Instance==TIM2) {
		/*tim2 interrupt*/
		*tim3_cr1 &= 0xFFFE;			//tim3 stop
	}

	if(htim->Instance==TIM3) {
		/*tim3 interrupt*/
		angle = (++angle % 768);
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  static int state[] = {0b100, 0b110, 0b010, 0b011, 0b001, 0b101};
//  static int state[] = {0b101, 0b001, 0b011, 0b010, 0b110, 0b100};	//reverse
  static int state_angle[] = {0, 128, 256, 384, 512, 640};
  static int state_num = 0;

  static volatile unsigned short* gpiob_idr = (unsigned short*)0x40020410;
  static volatile unsigned int* tim2_cr1 = (unsigned int*)0x40000000;
  static volatile unsigned int* tim2_cnt = (unsigned int*)0x40000024;
  static volatile unsigned int* tim3_cr1 = (unsigned int*)0x40000400;
  static volatile unsigned int* tim3_cnt = (unsigned int*)0x40000424;
  static volatile unsigned int* tim3_arr = (unsigned int*)0x4000042C;

  int hallsensor = (*gpiob_idr >> 3) & 0b111;
  if(hallsensor == state[state_num]) {
	  if((*tim2_cr1 & 1) == 1) {			//if tim2 enable
		  *tim3_cr1 &= 0xFFFE;			//tim3 stop
		  *tim3_cnt = 0;
		  *tim3_arr = *tim2_cnt;
		  *tim2_cnt = 0;
		  *tim3_cr1 |= 1;				//tim3 start
	  } else {
		  *tim2_cr1 |= 1;				//tim2 start
	  }
	  angle = state_angle[state_num];
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
