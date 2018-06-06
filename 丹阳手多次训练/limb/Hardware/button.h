#ifndef __BUTTON_H
#define __BUTTON_H

#include "main.h"
#include "config.h"

#define Key_Value  HAL_GPIO_ReadPin(BUTTON_GPIO_Port,BUTTON_Pin)

void TaskKeyFunc(void);

#endif
