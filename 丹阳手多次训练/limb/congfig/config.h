#ifndef __CONFIG_H
#define __CONFIG_H

#include "stm32f4xx_hal.h"

//�궨��

#define ActionNum 5           //��������Ϣ
#define EmgChannel 8     
#define TimeInterval 300              
#define TimeIncrement 100 
#define Start 1 
#define Rest 0   //ֻ��ʼ������һ��
#define DataBufferSize 800   //����������
#define FeatMatLength 328  ///41*8
//////����ѵ������////////////////////////////
#define TrainTimesSet 2   
//////////////////////////////
#define ON 1
#define OFF 0



//ȫ�ֱ���
extern uint8_t TimeFlag;
extern uint32_t AD_Value[9];
extern double   AD_Converted[EmgChannel+1];



extern uint16_t FeatIndexReg;

extern uint8_t ButtonState;
extern uint16_t WindowCount;
extern uint16_t TrainBlank;
extern uint16_t action_serial[(ActionNum-1)*2];
extern uint16_t UNILABEL[ActionNum];


//ѵ��&���Ա�־
extern uint8_t TrainFlag;
extern uint8_t TestFlag;

//DCMotor parameter
extern uint8_t Channelone_Pulse_Set;
extern uint16_t ADCMotorMax;
extern uint8_t GraspTime;

//����ѵ������
extern uint8_t TrainTimes;

extern const uint8_t IsChannelOpen[8];

#endif
