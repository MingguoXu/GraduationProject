#include "adda.h"
#include "delay.h"

/*************************
* 初始化ADC1,规则通道
*************************/															   
void ADC1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);	  //使能ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PA2、3作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //复位ADC1,将外设 ADC1的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ADC工作模式:独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);						//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  

	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束

	ADC_StartCalibration(ADC1);	 //开启AD校准

	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束

//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能
}

/*************************
* 获得ADC值
* ch：通道值 0~3
* 返回对应通道AD转换结果 0 ~ 4095
*************************/
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//使能指定的ADC1的软件转换启动功能	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//等待转换结束

	return ADC_GetConversionValue(ADC1);			//返回最近一次ADC1规则组的转换结果
}

/*************************
* 获得ADC平均值
* ch：通道值 0~3
* times：采样次数
* 返回对应通道AD转换结果 0 ~ 4095
*************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
} 

/*************************
* 平均N-X个ADC采样
* ch：规则通道值 默认N取8个，X取4个
*************************/
u16 Get_Adc_AvgNX(u8 ch)
{
	u8 i, flag=1, N=8;
	u16 temp, adc_sample[8]={0};
	for(i=0; i<N; i++)
	{
		adc_sample[i]=Get_Adc(ch);
		delay_ms(1);
	}
	
	while(flag)
	{
		flag = 0;
		for(i=0; i<N-1; i++)
		{
			if(adc_sample[i] > adc_sample[i+1])
			{
				temp = adc_sample[i];
				adc_sample[i] = adc_sample[i+1];
				adc_sample[i+1] = temp;
				flag = 1;
			}
		}
	}
	
	temp = 0;
	for(i=2; i<6; i++)
		temp += adc_sample[i];
	return temp>>2;
}


/*************************
* DAC配置
*************************/
void DAC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	DAC_DeInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );         //使能PORTA通道时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );           //使能DAC通道时钟 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                      // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                  //模拟输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                               //PA.4 输出高

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;        //不使用触发功能 TEN1=0
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不使用波形发生
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	//DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;        //DAC1输出缓存关闭 BOFF1=1
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable ;        //DAC1输出缓存打开，提高端口驱动能力 BOFF1=0
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);           //初始化DAC通道1

	DAC_Cmd(DAC_Channel_1, ENABLE);                        //使能DAC1	
	DAC_SetChannel1Data(DAC_Align_12b_R, 4095);            //12位右对齐数据格式设置DAC值
}

/*************************
* 设定DAC值
* vol：DA设定值 0 ~ 4095 mv
*************************/	
void DAC1_SetVol(u16 vol)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, vol);      //12位右对齐数据格式设置DAC值
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}
