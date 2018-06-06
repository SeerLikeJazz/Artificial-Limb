#include "calculate.h"


/////////////////////////emg.c-begin/////////////////////////////////////
//ȫ�ֱ����ĳ�ʼ��˳��
float feat_matrix[FeatMatLength*TrainTimesSet][N];

double DataTemp[EmgChannel];//��ʱ���ݻ�����//jy
double DataBuffer[DataBufferSize][EmgChannel];//�洢��EMG����//jy

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

	
	if(((TrainFlag==1)||(TestFlag==1))&&(FeatIndexReg==0))///����һ�£���һͣ��0����ط�
	{
	  FeatIndexReg = PushToBuf.DataIndex;
	}
	
	PushToBuf.DataIndex++;

	if(PushToBuf.DataIndex==DataBufferSize)
	  PushToBuf.DataIndex=0;//�����������ݴ�ͷ��ʼ�洢
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
		DataBuffer[index_temp][j]=*p;//LZX//<��data_buff[8]��ʱ���ݻ����������ݴ浽data[600][8]>
		p++;		
		} 
		
    /////////////////ֻ��train�й�////////////////////		
		if(TrainFlag&&(Dur>0)&&(Dur<DataSum)&&(Dur%(ActionTrainTime*1000)==0))//LZX//<ѵ������Ϣ��line99,100,>
		{
			//ÿ������ѵ������ʾ
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
		////////////////ֻ��train�й�//////////////////////
		///������
		if(FirstFlag==1)//first_flagʹ��������ִ���ڵ�һ��TIME_INTERVAL��
		{  
			if((index_temp+1)%TimeIncrement==0)
			{  
			CountTemp++;     //��֤ÿ��ִ��������ȡ����ʱ���ǵ�ǰ��������
			DataFullFlag=1;//��������TIME_INTERVAL300������һ�����������Ա�������ʼ������ȡ
			if(index_temp==(DataBufferSize-1))
				DataIndex=-1;//�����������ݴ�ͷ��ʼ�洢
			}	   
		}
		///ֻ����һ��
		if((index_temp>=TimeInterval-1)&((index_temp+1)%TimeIncrement==0)&(KillFlag==1))//kill_flag��������ִֻ��һ��if�����䣬��һ����
		{ 
		 FirstFlag=1;KillFlag=0;
		 DataFullFlag=1;//������300������һ�����������Ա�������ʼ������ȡ
		}	
		//Dur��С��0�����¸�ֵ
		if(Dur==0&&TestFlag==1)
       Dur=DataSum;		 
	}
	if(Dur<0)//LZX//<ִֻ��һ��>
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

