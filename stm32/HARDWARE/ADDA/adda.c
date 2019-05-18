#include "adda.h"
#include "delay.h"

/*************************
* ��ʼ��ADC1,����ͨ��
*************************/															   
void ADC1_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);	  //ʹ��ADC1ͨ��ʱ��

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA2��3��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//ģ����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  //��λADC1,������ ADC1��ȫ���Ĵ�������Ϊȱʡֵ

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		//ADC����ģʽ:����ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;			//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);						//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  

	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����

	ADC_StartCalibration(ADC1);	 //����ADУ׼

	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����

//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������
}

/*************************
* ���ADCֵ
* ch��ͨ��ֵ 0~3
* ���ض�Ӧͨ��ADת����� 0 ~ 4095
*************************/
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);			//ʹ��ָ����ADC1�����ת����������	
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));	//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);			//�������һ��ADC1�������ת�����
}

/*************************
* ���ADCƽ��ֵ
* ch��ͨ��ֵ 0~3
* times����������
* ���ض�Ӧͨ��ADת����� 0 ~ 4095
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
* ƽ��N-X��ADC����
* ch������ͨ��ֵ Ĭ��Nȡ8����Xȡ4��
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
* DAC����
*************************/
void DAC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitStructure;

	DAC_DeInit();

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );         //ʹ��PORTAͨ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );           //ʹ��DACͨ��ʱ�� 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                      // �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;                  //ģ������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);                               //PA.4 �����

	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;        //��ʹ�ô������� TEN1=0
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//��ʹ�ò��η���
	DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;//���Ρ���ֵ����
	//DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;        //DAC1�������ر� BOFF1=1
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable ;        //DAC1�������򿪣���߶˿��������� BOFF1=0
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);           //��ʼ��DACͨ��1

	DAC_Cmd(DAC_Channel_1, ENABLE);                        //ʹ��DAC1	
	DAC_SetChannel1Data(DAC_Align_12b_R, 4095);            //12λ�Ҷ������ݸ�ʽ����DACֵ
}

/*************************
* �趨DACֵ
* vol��DA�趨ֵ 0 ~ 4095 mv
*************************/	
void DAC1_SetVol(u16 vol)
{
	DAC_SetChannel1Data(DAC_Align_12b_R, vol);      //12λ�Ҷ������ݸ�ʽ����DACֵ
	DAC_SoftwareTriggerCmd(DAC_Channel_1, ENABLE);
}
