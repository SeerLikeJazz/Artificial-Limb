#include "motor.h"

void Motor_Long_Strong(void)
{
	Motor(1);
	HAL_Delay(2000);
	Motor(0);
}

void Motor_Short_Strong(void)
{	
	Motor(1);
	HAL_Delay(500);
	Motor(0);
}

void Motor_Short_Soft(void)
{
	for (int i = 0; i < 20; i++)
	{
	  Motor(1);
	  HAL_Delay(10);
	  Motor(0);
	  HAL_Delay(25);
	}
	HAL_Delay(500);
}
