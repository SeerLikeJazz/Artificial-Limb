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
		if(GraspFlag)//�ֺ��ϵ�ʱ����
		{
			if(AD_Converted[EmgChannel]>ADCMotorMax)///////////AD_Converted[EmgChannel]�����ADCͨ��
			{
				motor_flag++;
				if(motor_flag>=5)
				{	
					motor_flag=0;
					MotorA_Stop();
					///////�����û�//////////
					if(ADCMotorMax == 530)
						Motor_Attention();///��
					if(ADCMotorMax == 700)
						Motor_Short_Strong();///ǿ
					printf("nihao");
					GraspFlag = 0;//ץ�ı�־λ��0
				}
			}
		}//�ֺ��ϵ�ʱ����end
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
