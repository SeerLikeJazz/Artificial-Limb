#include "config.h"


uint16_t FeatIndexReg=0;

uint8_t ButtonState = Rest; //jy
uint16_t WindowCount=0;///把数据压到feat_matrix
uint16_t TrainBlank=0;///训练时每个动作间隔留白

uint16_t action_serial[(ActionNum-1)*2]={0,4,1,4,2,4,3,4};//只在LDA_train出现//jy..........
uint16_t UNILABEL[ActionNum]={0,1,2,3,4};//只在LDA_train出现//jy...........


//训练&测试标志
uint8_t TrainFlag=0; //jy
uint8_t TestFlag=0; //jy

//DCMotor parameter
uint8_t Channelone_Pulse_Set = 30;//范围100以下
uint16_t ADCMotorMax = 531;
uint8_t GraspTime = 1;

//设置训练次数
uint8_t TrainTimes = 0;

const uint8_t IsChannelOpen[8]={ON,ON,ON,ON,ON,ON,ON,ON};










