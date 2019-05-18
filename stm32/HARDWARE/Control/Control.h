#ifndef __Control_h
#define __Control_h

#include "sys.h"

/*------------------------------------------
 				电机PID结构体				
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
	int	Set;		//设定
	int Cur;		//测量值
	int Out;		//PID输出	
}PIDStru;
extern PIDStru motor;

extern void PID_init(void);		//初始化PID结构体、位置结构体、电机输出结构体的参数
extern void PID(void);      	//计算
extern void MotorOutLimit(void);//输出限幅
extern void MotorMove(void);	//输出

#endif
