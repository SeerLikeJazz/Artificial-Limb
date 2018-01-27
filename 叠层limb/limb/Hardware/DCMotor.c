#include "DCMotor.h"

///////////Motor A /////////////
void MotorA_Forward(void)
{
HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,GPIO_PIN_RESET);//0
HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,GPIO_PIN_SET);//1
}
void MotorA_Reverse(void)
{
HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,GPIO_PIN_SET);//1
HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,GPIO_PIN_RESET);//0
}
void MotorA_Stop(void)
{
HAL_GPIO_WritePin(IN1_GPIO_Port,IN1_Pin,GPIO_PIN_SET);//0
HAL_GPIO_WritePin(IN2_GPIO_Port,IN2_Pin,GPIO_PIN_SET);//0
}
///////////Motor B///////////////////
void MotorB_Forward(void)
{
HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_RESET);//0
HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_SET);//1
HAL_Delay(1100);
HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_RESET);//0
}
void MotorB_Reverse(void)
{
HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_SET);//1
HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_RESET);//0
HAL_Delay(1300);
HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_RESET);//0
}
void MotorB_Stop(void)
{
HAL_GPIO_WritePin(IN3_GPIO_Port,IN3_Pin,GPIO_PIN_RESET);//0
HAL_GPIO_WritePin(IN4_GPIO_Port,IN4_Pin,GPIO_PIN_RESET);//0
}
