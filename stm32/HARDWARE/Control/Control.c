#include "Control.h"

/*------------------------------------------
 				��������				
------------------------------------------*/
PIDStru motor;

float const pTable[7][7] = {0};
float const iTable[7][7] = {0};
float const dTable[7][7] = {0};

/*------------------------------------------
 ��������:��ʼ���ṹ�����			
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
* ģ������������ֵͨ�����Ǻ���ģ��������0 - 6
**/
u8 encode(float num)
{
	if(num > 300)
		num = 300;
	if(num < -300)
		num = -300;
	num += 350;		//������������
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

//λ��ʽ
void outValCal()
{
	
	motor.Err = motor.Set - motor.Cur;	//��ȡ���
	fuzzy();							//����������
	motor.SumErr += (motor.Err/1);		//���ֲ��޷�
	if(motor.SumErr > 20000)
		motor.SumErr = 20000;
	else if(motor.SumErr < -20000)
		motor.SumErr = -20000;
	motor.Out = motor.Err * motor.Kp + motor.SumErr * motor.Ki + (motor.Err - motor.LastErr) * motor.Kd;
	motor.LastErr = motor.Err;
}

///*------------------------------------------
// ��������:XPoint���PID����	
//------------------------------------------*/
//void XPoint(void)
//{
//	
//	/*--------λ���⻷-----------*/
//		/*----λ��ʽPID----*/
//	XPointPID.XPointErr=SetPoint.SetX-CurPoint.CurX;  			//�������
//	XPointPID.SumXPointErr+=(XPointPID.XPointErr*CurPoint.T);   //����ۻ�
//	if(XPointPID.SumXPointErr>20000)                          	//�������޷�
//		XPointPID.SumXPointErr=20000;
//	else if(XPointPID.SumXPointErr<-20000)
//		XPointPID.SumXPointErr=-20000;
//    XPout=XPointPID.XPointKp*XPointPID.XPointErr;
//	XIout=XPointPID.XPointKi*XPointPID.SumXPointErr/100;
//	XDout=XPointPID.XPointKd*1000*(XPointPID.XPointErr-XPointPID.LastXPointErr)/CurPoint.T;
//	XPointPID.XPointOut=XPout+XIout+XDout;
//	XPointPID.LastXPointErr=XPointPID.XPointErr;                //��¼���
//		/*----����ʽPID----*/
////	XPointPID.XPointErr=SetPoint.SetX-CurPoint.CurX;  //�������
////	XPointPID.XPointOut+=(XPointPID.XPointKp*(XPointPID.XPointErr-XPointPID.LastXPointErr)
////					+XPointPID.XPointKi*XPointPID.XPointErr
////					+XPointPID.XPointKd*((XPointPIDXPointErr-XPointPID.LastXPointErr)-(XPointPID.LastXPointErr-XPointPID.PrevrXPointErr)));
////	XPointPID.PrevrXPointErr=XPointPID.LastXPointErr;			//��¼�ϴ����
////	XPointPID.LastXPointErr=XPointPID.XPointErr;                //��¼�������
//	
//	/*--------�Ƕ��ڻ�--------*/
//		/*----λ��ʽPID----*/
//	XPointPID.RollErr=XPointPID.XPointOut-CurPoint.CurRoll;  		//���ٶ����
//	XPointPID.SumRollErr+=(XPointPID.RollErr*CurPoint.T);			//�ۻ�
//	if(XPointPID.SumRollErr>20000)
//		XPointPID.SumRollErr=20000;
//	else if(XPointPID.SumRollErr<-20000)
//		XPointPID.SumRollErr=-20000;
//	XPointPID.Out=XPointPID.RollKp*XPointPID.RollErr
//				+XPointPID.RollKi*XPointPID.SumRollErr
//				+XPointPID.RollKd*(XPointPID.RollErr-XPointPID.LastRollErr)/CurPoint.T;
//	XPointPID.LastRollErr=XPointPID.RollErr;					//��¼���
//		/*----����ʽPID----*/
////	XPointPID.RollErr=XPointPID.XPointOut-CurPoint.CurRoll;	    //�������
////	XPointPID.Out+=(XPointPID.RollKp*(XPointPID.RollErr-XPointPID.LastRollErr)
////				+XPointPID.RollKi*XPointPID.RollErr
////				+XPointPID.RollKd*((XPointPID.RollErr-XPointPID.LastRollErr)-(XPointPID.LastRollErr-XPointPID.PrevrRollErr)));
////	XPointPID.PrevrRollErr=XPointPID.LastRollErr;			//��¼�ϴ����
////	XPointPID.LastRollErr=XPointPID.RollErr;                //��¼�������
//}


/*------------------------------------------
 ��������:Roll���PID����	
 ����˵���������ĸ�PID���㺯�����ⲿ���ô˺�������
------------------------------------------*/
void PID(void)
{
	setValCal();	//Ŀ���趨ֵ�ļ���
	IOSetDir();		//����˶�����
	outValCal();	//���ֵ����
}

/*------------------------------------------
 ��������:���������㡢�޷�
 ����˵��:
------------------------------------------*/
void MotorOutLimit(void)
{
	if(motor.Out>9000)
		motor.Out=9000;
	else if(motor.Out<0)
		motor.Out=0;
}

/*------------------------------------------
 ��������:����ײ���������
 ����˵��:
------------------------------------------*/
void MotorMove(void)
{
	TIM_SetCompare1(TIM3,motor.Out);
	//TIM_SetCompare1(TIM3,2000);
}
