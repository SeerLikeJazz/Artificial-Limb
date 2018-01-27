#include "TimerTask.h"
#include "tim.h"


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==htim6.Instance)
	{
		GetAdcData();
		TimeFlag = 1;
	}
	if(htim->Instance==htim3.Instance)	
	{
		static uint8_t period_cnt = 0;
		period_cnt++;
		if (period_cnt >= 20)
		{
//			htim3.Instance->CCR1=	25;
			period_cnt=0;
		}
		
  }	
}
