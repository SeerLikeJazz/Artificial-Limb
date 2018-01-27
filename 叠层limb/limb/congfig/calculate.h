#ifndef __CALCULATE_H
#define __CALCULATE_H

#include "config.h"
#include "arm_math.h"//���ں���
#include "filter.h"
#include "FeatExtract.h"
#include "train.h"
#include "predict.h"

#define N 32

//ȫ�ֱ����ĳ�ʼ��˳��

///��FeatExtract.c����
extern float32_t feat[N];
extern float32_t feat_matrix[FeatMatLength][N];
///��train.c����
extern float32_t Bias_Mat[ActionNum];
extern float32_t ProjectionMat[N][ActionNum];
///��.c����
extern double DataBuffer[DataBufferSize][EmgChannel];//�洢��EMG����//jy


//void TaskDealData(void);
void DataPreProcess(void);
///��train.h����
void LDA_train(float32_t (*FeatMatIn)[N]);
void MatrixInv(float (*MatIn)[N],float (*MatOut)[N]);
///��predict.h����
uint8_t LDA_predict(float32_t FeatIn[N]);


typedef struct
{
	uint16_t DataIndex;
	double (*Buf)[EmgChannel];
	void (*Push)(double * ,uint16_t);
}PreProcess_TypeDef;

extern PreProcess_TypeDef PushToBuf;

#endif
