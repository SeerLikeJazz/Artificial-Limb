#include "predict.h"

float JudgeFunc[ActionNum];//在LDA_predict
uint16_t PredictIndex=0;

uint8_t LDA_predict(float32_t FeatIn[N])	
{ 
  float32_t gx; 
  float32_t MAXgx;
  uint8_t predict_temp; 
  for(uint8_t i=0;i<ActionNum;i++)
	{
		JudgeFunc[i]=0;
	}
  for(uint8_t i=0;i<ActionNum;i++)
  {
	 for(uint8_t j=0;j<N;j++)
	  { 
		 JudgeFunc[i]+=ProjectionMat[j][i]*feat[j];
	  }
  } 
  for(uint8_t i=0;i<ActionNum;i++)
  {
		 JudgeFunc[i]+=Bias_Mat[i];  
  }
	
	///////////////////选出极大值begin//////////////////////////
  for(uint8_t i=0;i<ActionNum;i++)
  {
    gx=JudgeFunc[i];
		if(i==0)
		{
			MAXgx =gx;
			predict_temp=i;
		}
		else
		{
			if(gx>MAXgx)
			{
				MAXgx=gx;
			  predict_temp=i;
			}	
		}
  }
	///////////////////选出极大值end//////////////////////////
  return predict_temp;
}

