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
  * COPYRIGHT(c) 2018 STMicroelectronics
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "config.h"
#include "button.h"
#include "motor.h"
#include "calculate.h"
#include "DCMotor.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t PredictResult[500]={0};
uint8_t motiondetect=0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

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
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  MX_TIM3_Init();

  /* USER CODE BEGIN 2 */
	HAL_TIM_Base_Start_IT(&htim6);//定时器中断开启
  HAL_ADC_Start_DMA(&hadc1,AD_Value,8);//ADC循环模式开启
	InitFilter();//滤波器初始化
	Motor_Long_Strong();//电机震动
	
  HAL_TIM_Base_Start_IT(&htim3);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
		
		
		
    #if 1
    TaskKeyFunc();
		if(TimeFlag)
		{
			TimeFlag = 0;
			DataPreProcess();
		}

    if(TrainFlag ==1)
	  { 
			 if((DataBufferSize-FeatIndexReg+PushToBuf.DataIndex)%100==0)
			 {   
					FeatExtract(DataBuffer);
				  if((TrainBlank+3)%50>=9)///6-46, 56-96.............
					{
						for(int i=0;i<N;i++)
						{
						 feat_matrix[WindowCount][i]=feat[i];					
						}
						WindowCount++;//WindowCount++
						///训练的时候显示
						if(WindowCount == 41)
							{printf("Train action 1\r\n");}
						if((WindowCount == 41*2)||(WindowCount == 41*3))
							{printf("Train action 2\r\n");}
						if((WindowCount == 41*4)||(WindowCount == 41*5))
							{printf("Train action 3\r\n");}
						if((WindowCount == 41*6)||(WindowCount == 41*7))
							{printf("Train action 4\r\n");}
//						if((WindowCount == 41*8)||(WindowCount == 41*9))
//							{printf("Train action 5\r\n");}	
						if(WindowCount==FeatMatLength)
							{printf("Train complete\r\n");}	
						///训练的时候显示		
          }
          TrainBlank++;	///397就结束				
			 }			
			 if(WindowCount==FeatMatLength)//训练数据收集完成
			 { 				 
				 LDA_train(feat_matrix);
				//数据float32_t ProjectionMat[32][6];float32_t Bias_Mat[6];存储在w25q128
//          Save_to_flash();
//					LED1=0;HAL_Delay(50);LED2=0;HAL_Delay(50);LED3=0;HAL_Delay(50);LED4=0;HAL_Delay(50);LED5=0;HAL_Delay(50);//提示存储
//					LED1=1;HAL_Delay(50);LED2=1;HAL_Delay(50);LED3=1;HAL_Delay(50);LED4=1;HAL_Delay(50);LED5=1;HAL_Delay(50);
				}
	  }
		
		if(TestFlag==1)
		{		
			if((DataBufferSize-FeatIndexReg+PushToBuf.DataIndex)%100==0)   
			{
				FeatExtract(DataBuffer);
				PredictResult[PredictIndex%500]=LDA_predict(feat);	      
				motiondetect=PredictResult[PredictIndex%500];				
				if(PredictIndex>1&&PredictResult[PredictIndex%500]==PredictResult[(PredictIndex-1)%500]&&
				  PredictResult[(PredictIndex-1)%500]==PredictResult[(PredictIndex-2)%500])
				{
					switch(motiondetect)
					{
					case 0:
						//USART_SendData(USART1, 'N');
						//模式1
						{printf("1\r\n");}
						break;
					case 1:
						//USART_SendData(USART1, 'P');
						//模式2
						{printf("2\r\n");}
						break;
					case 2:
						//USART_SendData(USART1, 'R');
						//模式3
						{printf("3\r\n");}
						MotorA_Reverse();
						break;
					case 3:
						//USART_SendData(USART1, 'T');
						//模式4
						{printf("4\r\n");}
						MotorA_Forward();
						break;
					case 4:
						//USART_SendData(USART1, '0');
						//模式5
					 {printf("r\r\n");}
					 MotorA_Stop();
						break;
					default:			
							break;
					 }
				}
				PredictIndex++;//PredictIndex++;
				if(PredictIndex ==500)
				{   	
					PredictIndex = 0;
				}			
			}	    	  	
		}
  #endif
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

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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

/* USER CODE BEGIN 4 */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	static uint8_t period_cnt = 0;
//	period_cnt++;
//	if (period_cnt >= 20)
//	{
//    htim3.Instance->CCR1=	25;
//	}
//	period_cnt=0;
//}
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
