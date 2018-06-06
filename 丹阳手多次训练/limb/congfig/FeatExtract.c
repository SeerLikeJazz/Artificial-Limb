#include "FeatExtract.h"
#include "calculate.h"

float feat[N];//feat存到feat_matrix

void FeatExtract(double (*DBuf)[EmgChannel])
{ 
  for(uint16_t i=0;i<EmgChannel;i++)
	{
		///初始化
	  feat[i*4]=0;feat[i*4+1]=0;feat[i*4+2]=0;feat[i*4+3]=0;  //4个特征
	  for(uint16_t j=0;j<TimeInterval;j++)
		{
			uint16_t  j0=(j+DataBufferSize+PushToBuf.DataIndex-TimeInterval)%DataBufferSize;//LZX//PushToBuf.DataIndex的前TimeInterval个数据
			int16_t  j1=(j-1+DataBufferSize+PushToBuf.DataIndex-TimeInterval)%DataBufferSize;
			int16_t  j2=(j-2+DataBufferSize+PushToBuf.DataIndex-TimeInterval)%DataBufferSize;			
			feat[i*4]+=fabs(DBuf[j0][i]); //LZX//<MAV,有点不一样>
			if (j>0)
			{
			  feat[i*4+1]+= (DBuf[j0][i]*(DBuf[j1][i])>0); //LZX//<ZC>
				feat[i*4+2]+= fabs(DBuf[j0][i]-DBuf[j1][i]); //LZX//<WV>
			}
			if (j>1)
			{
				feat[i*4+3]+= ((DBuf[j0][i]-DBuf[j1][i])*(DBuf[j1][i]-DBuf[j2][i])>0);//LZX//<SSC>			
			}
	  } 
	}
}
