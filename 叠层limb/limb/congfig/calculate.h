#ifndef __CALCULATE_H
#define __CALCULATE_H

#include "config.h"
#include "arm_math.h"//放在后面
#include "filter.h"
#include "FeatExtract.h"
#include "train.h"
#include "predict.h"

#define N 32

//全局变量的初始化顺序

///在FeatExtract.c定义
extern float32_t feat[N];
extern float32_t feat_matrix[FeatMatLength][N];
///在train.c定义
extern float32_t Bias_Mat[ActionNum];
extern float32_t ProjectionMat[N][ActionNum];
///在.c定义
extern double DataBuffer[DataBufferSize][EmgChannel];//存储的EMG数据//jy


//void TaskDealData(void);
void DataPreProcess(void);
///在train.h定义
void LDA_train(float32_t (*FeatMatIn)[N]);
void MatrixInv(float (*MatIn)[N],float (*MatOut)[N]);
///在predict.h定义
uint8_t LDA_predict(float32_t FeatIn[N]);


typedef struct
{
	uint16_t DataIndex;
	double (*Buf)[EmgChannel];
	void (*Push)(double * ,uint16_t);
}PreProcess_TypeDef;

extern PreProcess_TypeDef PushToBuf;

#endif
