#include "Control.h"

/*------------------------------------------
 				声明变量				
------------------------------------------*/
PIDStru motor;

float const pTable[7][7] = {0};
float const iTable[7][7] = {0};
float const dTable[7][7] = {0};

/*------------------------------------------
 函数功能:初始化结构体参数			
------------------------------------------*/
void PID_init()
{
	motor.Kp=6.5;
	motor.Ki=0.8;
	motor.Kd=0;
	motor.Err=0;
	motor.LastErr=0;
	motor.PrevrErr=0;
	motor.SumErr=0;
	motor.Set=0;
	motor.Cur=0;
	motor.Out=0;
}

/**
* 模糊化，将输入值通过三角函数模糊，返回0 - 6
**/
u8 encode(float num)
{
	if(num > 300)
		num = 300;
	if(num < -300)
		num = -300;
	num += 350;		//考虑四舍五入
	return num / 100;
}

void fuzzy()
{
	static u8 e, ec;
	static float dkp, dki, dkd;
	e = encode(motor.Err);
	ec = encode(motor.Err - motor.LastErr);
	dkp = pTable[e][ec];
	dki = iTable[e][ec];
	dkd = dTable[e][ec];
	motor.Kp += dkp;
	motor.Ki += dki;
	motor.Kd += dkd;
}

void setValCal()
{
	
}

void IOSetDir()
{
	
}

//位置式
void outValCal()
{
	
	motor.Err = motor.Set - motor.Cur;	//获取误差
	fuzzy();							//参数自整定
	motor.SumErr += (motor.Err/1);		//积分并限幅
	if(motor.SumErr > 20000)
		motor.SumErr = 20000;
	else if(motor.SumErr < -20000)
		motor.SumErr = -20000;
	motor.Out = motor.Err * motor.Kp + motor.SumErr * motor.Ki + (motor.Err - motor.LastErr) * motor.Kd;
	motor.LastErr = motor.Err;
}

///*------------------------------------------
// 函数功能:XPoint输出PID计算	
//------------------------------------------*/
//void XPoint(void)
//{
//	
//	/*--------位置外环-----------*/
//		/*----位置式PID----*/
//	XPointPID.XPointErr=SetPoint.SetX-CurPoint.CurX;  			//计算误差
//	XPointPID.SumXPointErr+=(XPointPID.XPointErr*CurPoint.T);   //误差累积
//	if(XPointPID.SumXPointErr>20000)                          	//误差积分限幅
//		XPointPID.SumXPointErr=20000;
//	else if(XPointPID.SumXPointErr<-20000)
//		XPointPID.SumXPointErr=-20000;
//    XPout=XPointPID.XPointKp*XPointPID.XPointErr;
//	XIout=XPointPID.XPointKi*XPointPID.SumXPointErr/100;
//	XDout=XPointPID.XPointKd*1000*(XPointPID.XPointErr-XPointPID.LastXPointErr)/CurPoint.T;
//	XPointPID.XPointOut=XPout+XIout+XDout;
//	XPointPID.LastXPointErr=XPointPID.XPointErr;                //记录误差
//		/*----增量式PID----*/
////	XPointPID.XPointErr=SetPoint.SetX-CurPoint.CurX;  //计算误差
////	XPointPID.XPointOut+=(XPointPID.XPointKp*(XPointPID.XPointErr-XPointPID.LastXPointErr)
////					+XPointPID.XPointKi*XPointPID.XPointErr
////					+XPointPID.XPointKd*((XPointPIDXPointErr-XPointPID.LastXPointErr)-(XPointPID.LastXPointErr-XPointPID.PrevrXPointErr)));
////	XPointPID.PrevrXPointErr=XPointPID.LastXPointErr;			//记录上次误差
////	XPointPID.LastXPointErr=XPointPID.XPointErr;                //记录本次误差
//	
//	/*--------角度内环--------*/
//		/*----位置式PID----*/
//	XPointPID.RollErr=XPointPID.XPointOut-CurPoint.CurRoll;  		//角速度误差
//	XPointPID.SumRollErr+=(XPointPID.RollErr*CurPoint.T);			//累积
//	if(XPointPID.SumRollErr>20000)
//		XPointPID.SumRollErr=20000;
//	else if(XPointPID.SumRollErr<-20000)
//		XPointPID.SumRollErr=-20000;
//	XPointPID.Out=XPointPID.RollKp*XPointPID.RollErr
//				+XPointPID.RollKi*XPointPID.SumRollErr
//				+XPointPID.RollKd*(XPointPID.RollErr-XPointPID.LastRollErr)/CurPoint.T;
//	XPointPID.LastRollErr=XPointPID.RollErr;					//记录误差
//		/*----增量式PID----*/
////	XPointPID.RollErr=XPointPID.XPointOut-CurPoint.CurRoll;	    //计算误差
////	XPointPID.Out+=(XPointPID.RollKp*(XPointPID.RollErr-XPointPID.LastRollErr)
////				+XPointPID.RollKi*XPointPID.RollErr
////				+XPointPID.RollKd*((XPointPID.RollErr-XPointPID.LastRollErr)-(XPointPID.LastRollErr-XPointPID.PrevrRollErr)));
////	XPointPID.PrevrRollErr=XPointPID.LastRollErr;			//记录上次误差
////	XPointPID.LastRollErr=XPointPID.RollErr;                //记录本次误差
//}


/*------------------------------------------
 函数功能:Roll输出PID计算	
 功能说明：调用四个PID计算函数，外部调用此函数即可
------------------------------------------*/
void PID(void)
{
	setValCal();	//目标设定值的计算
	IOSetDir();		//电机运动方向
	outValCal();	//输出值计算
}

/*------------------------------------------
 函数功能:电机输出计算、限幅
 函数说明:
------------------------------------------*/
void MotorOutLimit(void)
{
	if(motor.Out>9000)
		motor.Out=9000;
	else if(motor.Out<0)
		motor.Out=0;
}

/*------------------------------------------
 函数功能:电机底层驱动函数
 函数说明:
------------------------------------------*/
void MotorMove(void)
{
	TIM_SetCompare1(TIM3,motor.Out);
	//TIM_SetCompare1(TIM3,2000);
}
