#include "AdcTask.h"
#include "DCMotor.h"


uint32_t AD_Value[9]={0};
double  AD_Converted[EmgChannel+1]={0};

void GetAdcData(void)
{
	uint8_t j=0;
	
	for(uint8_t i=0;i<8;i++)
	{
		if(IsChannelOpen[i])
			AD_Converted[j++] = (AD_Value[i]&0xFFF);
	}
	AD_Converted[j] = (AD_Value[8]&0xFFF);	/////ADC of motor 传给	
}

//void GetAdcData(void)
//{
//		for(uint8_t i=0;i<(EmgChannel+1);i++)
//		{
//			AD_Converted[i] = (AD_Value[i]&0xFFF);
//		}
//		
//}

/*********************************************

V1   PA1
V2   PA2
V3   PA3
V4   PA4
V5   PA5
V6   PA6
V7   PA7
V8   PB0

ADC通道按顺序

********************************************/
