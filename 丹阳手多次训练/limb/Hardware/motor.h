#ifndef __MOTOR_H
#define __MOTOR_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define  Motor(a)    (a)? HAL_GPIO_WritePin(MOTOR_CTRL_GPIO_Port,MOTOR_CTRL_Pin,GPIO_PIN_RESET):HAL_GPIO_WritePin(MOTOR_CTRL_GPIO_Port,MOTOR_CTRL_Pin,GPIO_PIN_SET)

void Motor_Long_Strong(void);
void Motor_Short_Strong(void);
void Motor_Short_Soft(void);
void Motor_Attention(void);


#endif
