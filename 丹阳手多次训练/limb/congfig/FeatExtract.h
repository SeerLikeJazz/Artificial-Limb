#ifndef __FEATEXTRACT_H
#define __FEATEXTRACT_H

#include "config.h"
#include "arm_math.h"//放在后面,fabs函数
#include "calculate.h"  //N在这里定义



void FeatExtract(double (*DBuf)[EmgChannel]);


#endif
