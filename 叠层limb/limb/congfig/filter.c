#include "filter.h"

///filter params
uint8_t			  filter_n;
double				data_src[21][8];
double				data_filter[21][8];
double				filter_a[21];
double				filter_b[21];
int8_t        filter_index=-1;

//滤波器初始化
void InitFilter(void)
{  
   for(uint8_t i=0;i<21;i++)
   {
   	for(uint8_t j=0;j<8;j++)
   	{
   		data_src[i][j]=0;
   		data_filter[i][j]=0;
   	}
   }
   for(uint8_t i=0;i<21;i++)
   {
   	filter_a[i]=0;
   	filter_b[i]=0;
   }
   filter_n=20;        //1000Hz采样率下陷波器参数
   filter_a[0]=1.000000000000000;
   filter_a[20]=-0.809784033195007;
   filter_b[0]=0.904892016597504;
   filter_b[20]=-0.904892016597504;
}

//滤波
double Filter(double temp)
{
		filter_index=(filter_index+1)%8;    //LB// filter_index=-1;
		for(uint8_t i=0;i<filter_n;i++)         //LB// filter_n=20;
		{
			data_src[filter_n-i][filter_index]=data_src[filter_n-i-1][filter_index];
			data_filter[filter_n-i][filter_index]=data_filter[filter_n-i-1][filter_index];
		}
		data_src[0][filter_index]=temp;
		data_filter[0][filter_index]=0;
		for(uint8_t i=0;i<filter_n+1;i++)
			data_filter[0][filter_index]+=filter_b[i]*data_src[i][filter_index];
		for(uint8_t i=1;i<filter_n+1;i++)
			data_filter[0][filter_index]-=filter_a[i]*data_filter[i][filter_index];
		return data_filter[0][filter_index];	
} 
