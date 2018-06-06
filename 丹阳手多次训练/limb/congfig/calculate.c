#include "calculate.h"


/////////////////////////emg.c-begin/////////////////////////////////////
//全局变量的初始化顺序
float feat_matrix[FeatMatLength*TrainTimesSet][N];

double DataTemp[EmgChannel];//临时数据缓冲区//jy
double DataBuffer[DataBufferSize][EmgChannel];//存储的EMG数据//jy

/////////////////////////emg.c-end///////////////////////////////////

void Push(double *p ,uint16_t index_temp)
{
	for (uint8_t j=0;j<EmgChannel;j++)
	{
	 DataBuffer[index_temp][j]=*p;
	 p++;		
	}
}

PreProcess_TypeDef PushToBuf=
{
	0,
	DataBuffer,
	Push			
};

void DataPreProcess(void)
{
		 for (uint8_t i=0;i<EmgChannel;i++)
	{
	 DataTemp[i] = Filter(AD_Converted[i]);
	}
  PushToBuf.Push(DataTemp,PushToBuf.DataIndex);	

	
	if(((TrainFlag==1)||(TestFlag==1))&&(FeatIndexReg==0))///斟酌一下，万一停在0这个地方
	{
	  FeatIndexReg = PushToBuf.DataIndex;
	}
	
	PushToBuf.DataIndex++;

	if(PushToBuf.DataIndex==DataBufferSize)
	  PushToBuf.DataIndex=0;//将后续的数据从头开始存储
}

/*
//data after filter
void DataAfterFilter(void)
{
	 for (uint8_t i=0;i<EmgChannel;i++)
	{
	 DataTemp[i] = Filter(AD_Converted[i]);
	}
}

void DataInBuffer(double *p)
{   
	if(Dur>0)
	{   
		uint16_t index_temp=(DataIndex+1);
		Dur--;
		DataIndex++;
		for (uint8_t j=0;j<EmgChannel;j++)
		{
		DataBuffer[index_temp][j]=*p;//LZX//<把data_buff[8]临时数据缓冲区的数据存到data[600][8]>
		p++;		
		} 
		
    /////////////////只跟train有关////////////////////		
		if(TrainFlag&&(Dur>0)&&(Dur<DataSum)&&(Dur%(ActionTrainTime*1000)==0))//LZX//<训练和休息，line99,100,>
		{
			//每个动作训练完提示
			if(Dur == 45000)
				{printf("Train action 1\r\n");}
			if((Dur == 40000)||(Dur == 35000))
				{printf("Train action 2\r\n");}
			if((Dur == 30000)||(Dur == 25000))
				{printf("Train action 3\r\n");}
			if((Dur == 20000)||(Dur == 15000))
				{printf("Train action 4\r\n");}
			if((Dur == 10000)||(Dur == 5000))
				{printf("Train action 5\r\n");}
		}
		////////////////只跟train有关//////////////////////
		///滑动窗
		if(FirstFlag==1)//first_flag使得增量窗执行在第一个TIME_INTERVAL后
		{  
			if((index_temp+1)%TimeIncrement==0)
			{  
			CountTemp++;     //保证每次执行特征提取函数时都是当前窗的数据
			DataFullFlag=1;//数据满个TIME_INTERVAL300个，即一个滑动窗，以便主程序开始特征提取
			if(index_temp==(DataBufferSize-1))
				DataIndex=-1;//将后续的数据从头开始存储
			}	   
		}
		///只运行一次
		if((index_temp>=TimeInterval-1)&((index_temp+1)%TimeIncrement==0)&(KillFlag==1))//kill_flag的作用是只执行一次if后的语句，第一个窗
		{ 
		 FirstFlag=1;KillFlag=0;
		 DataFullFlag=1;//数据满300个，即一个滑动窗，以便主程序开始特征提取
		}	
		//Dur减小到0后，重新赋值
		if(Dur==0&&TestFlag==1)
       Dur=DataSum;		 
	}
	if(Dur<0)//LZX//<只执行一次>
	{
	   Dur++;
		if(Dur==0&&ButtonState==Start )
			Dur=DataSum;	
	}	
}

void TaskDealData(void)
{
	DataAfterFilter();
	DataInBuffer(DataTemp);
}
*/

