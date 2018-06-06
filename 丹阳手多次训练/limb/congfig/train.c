#include "train.h"


float32_t Bias_Mat[ActionNum];//ģ�ʹ洢��
float32_t ProjectionMat[N][ActionNum];//ģ�ʹ洢��
////////////////////MatrixInv�������м����//////////////////////////////////////
float CovMat[N][N]; //���ļ�4������train,����MatrixInv
float MatrixInv_a[N][N],MatrixInv_b[N][N*2];//a,b��ΪMatrixInv�����ľֲ������ã���stack�Ļ����

/////////////////////ֻ�ڱ�ҳTrain����///////////////////////////////////////
uint16_t LABEL[FeatMatLength] ;
float ActionSameFeatNum[ActionNum]={41*TrainTimesSet,41*TrainTimesSet,41*TrainTimesSet,41*TrainTimesSet,41*4*TrainTimesSet};
float MeanMat[ActionNum*N] ;
float CovMat_Inverse[N][N];
float Temp_Mat[N];
//////////////////////////////////////////////////////


#if 1 ///LDA_train
void LDA_train(float32_t (*FeatMatIn)[N])
{  	
  /////////init data begin////////////// 
	for(uint16_t i=0;i<N;i++)
	{
		for(uint16_t j=0;j<ActionNum;j++)
		{
	       ProjectionMat[i][j]=0;
		}
	}
  for(uint16_t i=0;i<(N*ActionNum);i++)
	{
	  MeanMat[i]=0;	
	}
	for(uint16_t i=0;i<N;i++)
	{
	  for(uint16_t j=0;j<N;j++)
		{
		  CovMat[i][j]=0;	
	    CovMat_Inverse[i][j]=0;			
		}
	}	
	for(uint16_t i=0;i<N;i++)
	{
	  Temp_Mat[i]=0;
	}	
	 for(uint16_t i=0;i<ActionNum;i++)
	{
	  Bias_Mat[i]=0;	
	}
	 for(uint16_t i=0;i<((ActionNum-1)*2);i++)//������ǩ
  {
    for(uint16_t j=0;j<(FeatMatLength/((ActionNum-1)*2));j++)
       LABEL[j+i*(FeatMatLength/((ActionNum-1)*2))]=action_serial[i];//LZX//<u16 action_serial[10]={0,5,1,5,2,5,3,5,4,5};>
  }
  /////////init data end////////////// 
	
	/////////�����ֵ����begin///////////////
  for(uint16_t j=0;j<ActionNum;j++)
  {
   for(uint16_t i=0;i<FeatMatLength*TrainTimesSet;i++)
   {
		 if((LABEL[i%FeatMatLength]-UNILABEL[j])==0)  //LZX//<u16 UNILABEL[5]={0,1,2,3,4};>
		 {  
			 for(uint16_t k=0;k<N;k++)
			 { 			
				 MeanMat[j*N+k]=MeanMat[j*N+k]+feat_matrix[i][k]; 		 				 
			 } 
		 }   
   }	 
  }
  for(uint16_t i=0;i<ActionNum;i++)
  {
	 for(uint16_t k=0;k<N;k++)	 
		 MeanMat[i*N+k]=MeanMat[i*N+k]/ActionSameFeatNum[i];	 
  }
 	/////////�����ֵ����end///////////////
 
  /////////��������Э�������begin///////////////
  for(uint16_t j=0;j<ActionNum;j++)
  {  
   for(uint16_t i=0;i<FeatMatLength*TrainTimesSet;i++)
   {
		 if(LABEL[i%FeatMatLength]==UNILABEL[j])
		 {   			
			 for(uint16_t k=0;k<N;k++)
			 {
				 Temp_Mat[k]=feat_matrix[i][k]-MeanMat[j*N+k];             
			 }
			 for(uint16_t p=0;p<N;p++)
			 {
				 for(uint16_t q=0;q<N;q++)
				 {
					CovMat[p][q]=CovMat[p][q]+Temp_Mat[p]*Temp_Mat[q];
				 }		 
			 }		 
		 }	 
   }	  
  }
  for(uint16_t p=0;p<N;p++)
  {
	 for(uint16_t q=0;q<N;q++)
	 {
		CovMat[p][q]=CovMat[p][q]/(FeatMatLength*TrainTimesSet-ActionNum);//�㷨��N-C
	 }		 
  } 
  for(uint16_t i=0;i<N;i++)
  {
	 CovMat[i][i]+=1;//���ϵ�λ����֤��һ������
  }
	/////////��������Э�������end///////////////
		 
  //////////////////��������begin///////////////////////
	MatrixInv(CovMat,CovMat_Inverse);
  //////////////////��������end//////////////////////
		
  ////////////////////����������ͶӰ����32*32��Э�������������CovMat_Inverse���ֵ����MeanMatת�õĳ˻�/////
  ///////////////////ProjectionMat�����洢begin///////////////////////////////////////////////////////////////////////// 
	for(uint16_t i=0;i<N;i++)
	{
		for(uint16_t j=0;j<ActionNum;j++)
		{  
		   for(uint16_t p=0;p<N;p++)
		   {		     			   
			  ProjectionMat[i][j]+=CovMat_Inverse[i][p]*MeanMat[j*N+p];		      		 
		   } 		
		}
	}
  ////////////////////����������ͶӰ����32*32��Э�������������CovMat_Inverse���ֵ����MeanMatת�õĳ˻�/////
  ///////////////////ProjectionMat�����洢end/////////////////////////////////////////////////////////////////////////

  ////////////////////����ƫ��begin////////////////
	for(uint16_t i=0;i<ActionNum;i++)
	{
		for(uint16_t j=0;j<N;j++)
		{
			 Bias_Mat[i]+=MeanMat[i*N+j]*ProjectionMat[j][i];
		}
	}
	for(uint16_t i=0;i<ActionNum;i++)
	{
		Bias_Mat[i]=-0.5*Bias_Mat[i];
	}
  ////////////////////����ƫ��end////////////////
  // LED7=0;//DENOTE THE END OF TRAINING
  TrainFlag=0;
}
#endif //LDA_train


//MatIn[][N]:�������
//MatOut[][N]:��������ȶ���ȫ�ֱ���
//MatrixInv_a,MatrixInv_b
//�ȶ���4��ȫ�ֱ���
void MatrixInv(float (*MatIn)[N],float (*MatOut)[N])
{
  int16_t i,j,m,k;
	float t;	
	
	for(i=0;i<N;i++)//���Ƶ�MatrixInv_a[32,32]
   { 
		 for(k=0;k<N;k++)
		 {
			 MatrixInv_a[i][k] =MatIn[i][k];
		 }	   
    }
  for(i=0;i<N;i++)//����b�ұ߸�ֵ0
   {
		 for(j=N;j<2*N;j++)
		 {
			 MatrixInv_b[i][j]=0;
		 }
   }
  for(i=0;i<N;i++)//б���и�ֵ1
   {
		MatrixInv_b[i][N+i]=1;	
   }
  for(i=0;i<N;i++)//��a����ֵ��b���
   {
	   for(j=0;j<N;j++)
      {	
		     MatrixInv_b[i][j]=MatrixInv_a[i][j];
	    }
   }
	 
				for(m=0;m<N;m++)          //��ÿ�н��д��� line_335 
				{  
					 t=MatrixInv_b[m][m];                  //Ԥ��MatrixInv_b[m][m]��  
					 i=m;                         
					 while(MatrixInv_b[m][m]==0) 
					 {     
						 MatrixInv_b[m][m]=MatrixInv_b[i+1][m];   
						 i++;  
					 }  
					 if(i>m)  
					 { 
						 MatrixInv_b[i][m]=t;                  //ʵ�ֽ�����    														
						 //��������������Ӧλ�õ�Ԫ�� 
						 for(j=0;j<m;j++)                       
						 { 
							 t=MatrixInv_b[m][j];   
							 MatrixInv_b[m][j]=MatrixInv_b[i][j];   
							 MatrixInv_b[i][j]=t;  
						 } 
						 for(j=m+1;j<2*N;j++)                       
						 { 
							 t=MatrixInv_b[m][j];   
							 MatrixInv_b[m][j]=MatrixInv_b[i][j];   
							 MatrixInv_b[i][j]=t;  
						 }    
						}   
				    for(i=m+1;i<N;i++) 
						{
							for(j=2*N-1;j>=m;j--) 
							{
								MatrixInv_b[i][j]=MatrixInv_b[i][j]-MatrixInv_b[i][m]*MatrixInv_b[m][j]/MatrixInv_b[m][m]; //m=0ʱ������һ�е�-MatrixInv_b[i][0]/MatrixInv_b[0][0]���ӵ����¸��С���������ÿ�е�һ��Ԫ��MatrixInv_b[i][0]��Ϊ0��  
							}	
					  }		
					  for(j=2*N-1;j>=m;j--) 
						{
								MatrixInv_b[m][j]/=MatrixInv_b[m][m];   //�Ե�m�����б任��ͬ����MatrixInv_b[m][m]��ʹMatrixInv_b[m][m]Ϊ1��   	
						}
				 }
				
  for(j=2*N-1;j>=m-1;j--)
  {
     MatrixInv_b[m][j]=MatrixInv_b[m][j]/MatrixInv_b[m][m];
  } 
	
	m=N-1;  
	while(m>0)  
	{ 
		for(i=0;i<m;i++) 
		{
			 for(j=2*N-1;j>=m;j--)           //ǧ��ע�⣬�˴�j����ݼ�������MatrixInv_b[i][m]�ȱ�Ϊ0������ļ������Ч��     
			 MatrixInv_b[i][j]-=MatrixInv_b[i][m]*MatrixInv_b[m][j];        
		}
		m--;  
	}	
	
	for(i=0;i<N;i++)                         //�����������ά����c�С�   
	{
		 for(j=0;j<N;j++)    
			 MatOut[i][j]=MatrixInv_b[i][N+j];   
	}
}
