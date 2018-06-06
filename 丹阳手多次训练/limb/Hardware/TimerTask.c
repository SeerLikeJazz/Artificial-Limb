#include "TimerTask.h"
#include "tim.h"
#include "DCMotor.h"
#include "motor.h"



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint8_t motor_flag=0;
	if(htim->Instance==htim6.Instance)
	{
		GetAdcData();
		TimeFlag = 1;
		if(GraspFlag)//手合上的时候检测
		{
			if(AD_Converted[EmgChannel]>ADCMotorMax)///////////AD_Converted[EmgChannel]电机的ADC通道
			{
				motor_flag++;
				if(motor_flag>=5)
				{	
					motor_flag=0;
					MotorA_Stop();
					///////提醒用户//////////
					if(ADCMotorMax == 530)
						Motor_Attention();///弱
					if(ADCMotorMax == 700)
						Motor_Short_Strong();///强
					printf("nihao");
					GraspFlag = 0;//抓的标志位置0
				}
			}
		}//手合上的时候检测end
	}
	if(htim->Instance==htim3.Instance)	
	{
//		static uint8_t period_cnt = 0;
//		period_cnt++;
//		if (period_cnt >= 50)
//		{
//			htim3.Instance->CCR1=	5;
//			period_cnt=0;
//		}
  }	


}
