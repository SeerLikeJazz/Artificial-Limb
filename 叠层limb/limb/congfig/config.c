#include "config.h"


uint16_t FeatIndexReg=0;

uint8_t ButtonState = Rest; //jy
uint16_t WindowCount=0;///������ѹ��feat_matrix
uint16_t TrainBlank=0;///ѵ��ʱÿ�������������

uint16_t action_serial[(ActionNum-1)*2]={0,4,1,4,2,4,3,4};//ֻ��LDA_train����//jy..........
uint16_t UNILABEL[ActionNum]={0,1,2,3,4};//ֻ��LDA_train����//jy...........


//ѵ��&���Ա�־
uint8_t TrainFlag=0; //jy
uint8_t TestFlag=0; //jy






