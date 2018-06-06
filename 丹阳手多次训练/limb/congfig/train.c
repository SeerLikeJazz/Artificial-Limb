#include "train.h"


float32_t Bias_Mat[ActionNum];//模型存储用
float32_t ProjectionMat[N][ActionNum];//模型存储用
////////////////////MatrixInv函数的中间变量//////////////////////////////////////
float CovMat[N][N]; //本文件4处，在train,还有MatrixInv
float MatrixInv_a[N][N],MatrixInv_b[N][N*2];//a,b作为MatrixInv函数的局部变量用，放stack的话溢出

/////////////////////只在本页Train出现///////////////////////////////////////
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
	 for(uint16_t i=0;i<((ActionNum-1)*2);i++)//制作标签
  {
    for(uint16_t j=0;j<(FeatMatLength/((ActionNum-1)*2));j++)
       LABEL[j+i*(FeatMatLength/((ActionNum-1)*2))]=action_serial[i];//LZX//<u16 action_serial[10]={0,5,1,5,2,5,3,5,4,5};>
  }
  /////////init data end////////////// 
	
	/////////计算均值矩阵begin///////////////
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
 	/////////计算均值矩阵end///////////////
 
  /////////计算总体协方差矩阵begin///////////////
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
		CovMat[p][q]=CovMat[p][q]/(FeatMatLength*TrainTimesSet-ActionNum);//算法中N-C
	 }		 
  } 
  for(uint16_t i=0;i<N;i++)
  {
	 CovMat[i][i]+=1;//加上单位矩阵保证逆一定存在
  }
	/////////计算总体协方差矩阵end///////////////
		 
  //////////////////矩阵求逆begin///////////////////////
	MatrixInv(CovMat,CovMat_Inverse);
  //////////////////矩阵求逆end//////////////////////
		
  ////////////////////接下来计算投影矩阵32*32的协方差矩阵的逆矩阵CovMat_Inverse与均值矩阵MeanMat转置的乘积/////
  ///////////////////ProjectionMat用来存储begin///////////////////////////////////////////////////////////////////////// 
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
  ////////////////////接下来计算投影矩阵32*32的协方差矩阵的逆矩阵CovMat_Inverse与均值矩阵MeanMat转置的乘积/////
  ///////////////////ProjectionMat用来存储end/////////////////////////////////////////////////////////////////////////

  ////////////////////计算偏置begin////////////////
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
  ////////////////////计算偏置end////////////////
  // LED7=0;//DENOTE THE END OF TRAINING
  TrainFlag=0;
}
#endif //LDA_train


//MatIn[][N]:输入矩阵
//MatOut[][N]:输出矩阵，先定义全局变量
//MatrixInv_a,MatrixInv_b
//先定义4个全局变量
void MatrixInv(float (*MatIn)[N],float (*MatOut)[N])
{
  int16_t i,j,m,k;
	float t;	
	
	for(i=0;i<N;i++)//复制到MatrixInv_a[32,32]
   { 
		 for(k=0;k<N;k++)
		 {
			 MatrixInv_a[i][k] =MatIn[i][k];
		 }	   
    }
  for(i=0;i<N;i++)//矩阵b右边赋值0
   {
		 for(j=N;j<2*N;j++)
		 {
			 MatrixInv_b[i][j]=0;
		 }
   }
  for(i=0;i<N;i++)//斜对行赋值1
   {
		MatrixInv_b[i][N+i]=1;	
   }
  for(i=0;i<N;i++)//把a方阵赋值给b左边
   {
	   for(j=0;j<N;j++)
      {	
		     MatrixInv_b[i][j]=MatrixInv_a[i][j];
	    }
   }
	 
				for(m=0;m<N;m++)          //对每行进行处理。 line_335 
				{  
					 t=MatrixInv_b[m][m];                  //预存MatrixInv_b[m][m]。  
					 i=m;                         
					 while(MatrixInv_b[m][m]==0) 
					 {     
						 MatrixInv_b[m][m]=MatrixInv_b[i+1][m];   
						 i++;  
					 }  
					 if(i>m)  
					 { 
						 MatrixInv_b[i][m]=t;                  //实现交换。    														
						 //交换其它各列相应位置的元素 
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
								MatrixInv_b[i][j]=MatrixInv_b[i][j]-MatrixInv_b[i][m]*MatrixInv_b[m][j]/MatrixInv_b[m][m]; //m=0时，将第一行的-MatrixInv_b[i][0]/MatrixInv_b[0][0]倍加到以下各行。这样以下每行第一个元素MatrixInv_b[i][0]就为0。  
							}	
					  }		
					  for(j=2*N-1;j>=m;j--) 
						{
								MatrixInv_b[m][j]/=MatrixInv_b[m][m];   //对第m行作行变换，同除以MatrixInv_b[m][m]，使MatrixInv_b[m][m]为1。   	
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
			 for(j=2*N-1;j>=m;j--)           //千万注意，此处j必须递减，否则MatrixInv_b[i][m]先变为0，后面的计算就无效！     
			 MatrixInv_b[i][j]-=MatrixInv_b[i][m]*MatrixInv_b[m][j];        
		}
		m--;  
	}	
	
	for(i=0;i<N;i++)                         //将逆矩阵存入二维数组c中。   
	{
		 for(j=0;j<N;j++)    
			 MatOut[i][j]=MatrixInv_b[i][N+j];   
	}
}
