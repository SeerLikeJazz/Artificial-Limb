#include "button.h"
#include "DCMotor.h"


///////////////////////////////////////////////////////////////////////////
uint8_t key_fall_flag =0;
uint8_t keyUpFlag=0;
uint16_t key_holdon_ms=0;
uint8_t short_key_flag=0;
uint8_t key_long_down=0;
uint8_t long_key_flag=0;
uint16_t keyupCnt=0;


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if(Key_Value == 0)
	{
	  key_fall_flag = 1;//���ɰ������±�־
	}
	__HAL_GPIO_EXTI_CLEAR_IT(BUTTON_Pin);
 
}


void HAL_SYSTICK_Callback(void)
{
	if(key_fall_flag==1)//�������������¼�
	{
			if(Key_Value==0)//������������
			 {         
					if(key_holdon_ms <= 2000)  
					 {
							key_holdon_ms++; 
					 } 
					 else //�������µ�2000ms���жϳ���ʱ����������ɳ�����־
					 { 
						 key_holdon_ms = 0; 
						 short_key_flag=0;//��̰�����־
						 key_long_down = 1;//��������־��λ
						 key_fall_flag = 0;//�尴�����±�־
						} 
			 } 
		else //����̧��
		{ 
				 if(key_holdon_ms>50)//����ʱ�����50ms�����ɵ�����־
					{  
						 key_holdon_ms=0;
						 short_key_flag=1;
						 key_long_down =0;
						 key_fall_flag=0;
						
						 keyUpFlag = 1;//����̧�𰴼������ɰ���̧���־

					} 
				 else  //��������ʱ��С��50ms,����
						{    
								 key_holdon_ms=0; 
								 short_key_flag=0;
								 long_key_flag=0;
									key_fall_flag=0;
					 } 
		}

	}
	if(keyUpFlag)//����̧�������������������500ms
		 keyupCnt++;
  if(keyupCnt>500)
	{ 
		 keyupCnt = 0;
		 keyUpFlag = 0;
	}
}



void TaskKeyFunc(void)
{
 if(short_key_flag)//testing
 {
  short_key_flag = 0;
	 {
		TestFlag=1;
		TrainFlag=0;
		ButtonState=Start;
		FeatIndexReg=0;
	 }
 }
 if(key_long_down)//training
 {
  key_long_down = 0;
	 {
		TrainFlag =1; 
		TestFlag=0;
		ButtonState=Start;
		FeatIndexReg=0;
	 }
	WindowCount = 0;//��ʼ������Ȼtraining���������治����
	TrainBlank = 0;
	{
		printf("Train action 1\r\n");
		MotorA_Reverse();
		HAL_Delay(1500);
//		MotorA_Stop();
	}

 }

}

