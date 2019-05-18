#ifndef __Control_h
#define __Control_h

#include "sys.h"

/*------------------------------------------
 				���PID�ṹ��				
------------------------------------------*/
typedef struct
{	
	float  Kp;		
	float  Ki;
	float  Kd;
	float  Err;
	float  LastErr;         
	float  PrevrErr;       
	float  SumErr;
	int	Set;		//�趨
	int Cur;		//����ֵ
	int Out;		//PID���	
}PIDStru;
extern PIDStru motor;

extern void PID_init(void);		//��ʼ��PID�ṹ�塢λ�ýṹ�塢�������ṹ��Ĳ���
extern void PID(void);      	//����
extern void MotorOutLimit(void);//����޷�
extern void MotorMove(void);	//���

#endif
