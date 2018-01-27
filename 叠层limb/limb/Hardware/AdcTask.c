#include "AdcTask.h"


uint32_t AD_Value[EmgChannel]={0};
double  AD_Converted[EmgChannel]={0};


void GetAdcData(void)
{
		for(uint8_t i=0;i<EmgChannel;i++)
		{
			AD_Converted[i] = (AD_Value[i]&0xFFF);
		}

}
