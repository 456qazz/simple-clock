/***************************************************************************
                         DS1302��ʾʱ��
****************************************************************************/
#ifndef __STCREG_H__
#define __STCREG_H__

#include"reg52.h"
#include"main.h"

void Ds1302ShowTime();		  //ʱ����ʾ������
void DStime_up();			  //����ʱ�����Ӻ���
void DStime_down();			  //����ʱ����ٺ���
void Key4_TimeScan();		  //key4ɨ�躯��
void Key4_TimeHandler();	  //key4����������

sbit SCLK = P3^6;
sbit DSIO  = P3^4;
sbit RST = P3^5;

extern bit long_press;
extern uchar flag;
extern bit readtime;	  	  //ʱ����ʾ����

						   	  //DS1302д��Ͷ�ȡʱ����ĵ�ַ����
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
							  //���ʱ�������� ���λ��дλ 
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
							  //DS1302ʱ�ӳ�ʼ��0��00��00�룬�洢˳�������ʱ���洢��ʽ����BCD��
uchar TIME[3] = {0x00, 0x00, 0x00};


/*******************************************************************************
* �� �� ��         : Ds1302Write
* ��������		   : ��DS1302�����ַ+���ݣ�
* ��    ��         : addr,dat
* ��    ��         : ��
*******************************************************************************/

void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();

	SCLK = 0;				//�Ƚ�SCLK�õ͵�ƽ��
	_nop_();
	RST = 1; 				//Ȼ��RST(CE)�øߵ�ƽ��
	_nop_();

	for (n=0; n<8; n++)		//��ʼ���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01; //���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;			//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)		//д��8λ����
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;			//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;				//�������ݽ���
	_nop_();
}

/*******************************************************************************
* �� �� ��         : Ds1302Read
* ��������		   : ��ȡһ����ַ������
* ��    ��         : addr
* ��    ��         : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;				//SCLK�õ͵�ƽ��
	_nop_();
	RST = 1;				//RST(CE)�øߵ�ƽ��
	_nop_();

	for(n=0; n<8; n++)		//���Ͱ�λ��ַ����
	{
		DSIO = addr & 0x01; //���ݴӵ�λ��ʼ����
		addr >>= 1;
		SCLK = 1;			//������������ʱ��DS1302��ȡ����
		_nop_();
		SCLK = 0;			//DS1302�½���ʱ����������
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)		//��ȡ8λ����
	{
		dat1 = DSIO;		//�����λ��ʼ����
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;			//DS1302�½���ʱ����������
		_nop_();
	}

	RST = 0;
	_nop_();				//DS1302��λ���ȶ�ʱ��
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************************
* �� �� ��         : Ds1302Init
* ��������		   : ��ʼ��DS1302.
*******************************************************************************/

void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0x00);		//�ر�д��������
	for (n=0; n<3; n++)			//д��7���ֽڵ�ʱ���źţ�����ʱ
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		//��д��������
}

/*******************************************************************************
* �� �� ��         : Ds1302ReadTime
* ��������		   : ��ȡʱ����Ϣ
*******************************************************************************/

void Ds1302ReadTime()
{
	uchar n;
	for (n=0; n<3; n++)//��ȡ7���ֽڵ�ʱ���źţ�����ʱ��������
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}

/***************************************************************
		  			        ����ɨ��
***************************************************************/

uchar time_keytemp = 0, i_t = 0;		 //time_keytempΪ����ֵ��i_tΪ�л�������λ

void time_scan()						 //key2��key3����ɨ��
{
	static char time_state = 0; 
	
	if (key2 == 0)
		time_keytemp = 2;
	else if (key3 == 0)
		time_keytemp = 3;
	else
		time_keytemp = 0;

 	switch(time_state)
	{
		case 0: if(time_keytemp == 0)
				{
					time_state = 1;
				}
				else
					time_state = 0;
				break;		
		case 1: if(time_keytemp == 0)
				{
					time_state = 2;
				}
				else
					time_state = 0;
				break;
		case 2: if(time_keytemp != 0) 
				{
					if(time_keytemp == 2)
					{
					 	DStime_up();	
					}
					else if(time_keytemp == 3)
					{
					 	DStime_down();
					}
					time_state = 0;
				}
				else
					time_state = 2;
				break;
	}
}

void Key4_TimeScan()						//key4ɨ��
{
	static uchar Key4_Timestate = 0; 		//key4��״̬

	switch (Key4_Timestate)
	{
	case 0: if (key4 == 0)
			{
				Key4_Timestate = 1;
			}
			else
				Key4_Timestate = 0;
			break;

	case 1: if (key4 == 0)
			{
				Key4_Timestate = 2;
			}
			else
				Key4_Timestate = 0;
			break;

	case 2: if (key4 != 0)
			{
				
				Key4_TimeHandler(); 		//�԰������д���
				Key4_Timestate = 0;
			}
			else
				Key4_Timestate = 3;
			break;

	case 3: if (key4 == 0)
			{
				last_time++;
				if (last_time >= 200)		//ȷ���ǳ��������볤���Ĳ���
				{
					last_time = 0;
					long_press = 1;
				}
			}
			else
				Key4_Timestate = 2;				 
			break;
	   		}
}

void Key4_TimeHandler()					//key4��������
{
	if(long_press == 1)
	{
		long_press = 0;
		led_long = 0;				    //����ָʾ��
		readtime = 1;				   	//��ʱ����ʾ
	}	
	else if(long_press == 0)
	{
	 	led_short = ~led_short;		    //�̰�ָʾ��
		i_t++;							//�л�������λ
		if(i_t >= 3)
			i_t = 0;
	}
}
/****************************************************************
						�������ʾ�����ֵ���
*****************************************************************/
void DStime_up()							 //ʱ������
{
	if(i_t == 2)							 //����ʱ
	{
		if(TIME[i_t] >=0x23)
			TIME[i_t] = 0x00;
		else 
		{	
			TIME[i_t]++;
			if((TIME[i_t]&0x0f) >= 0x0a)
				TIME[i_t] += 6; 		
		}
	}
	else if(i_t == 1||i_t == 0)				 //�����֡���
	{
		 if(TIME[i_t] >=0x59)
			TIME[i_t] = 0x00;
		 else 
		 {	
			TIME[i_t]++;
			if((TIME[i_t]&0x0f) >= 0x0a)
				TIME[i_t] += 6; 		
		 } 
	}
}

void DStime_down()							  //ʱ���С
{
	if(i_t == 2)						   	  //����ʱ
	{
		if(TIME[i_t] == 0)
			TIME[i_t] = 23;
		else 
		{	
			TIME[i_t]--;
			if((TIME[i_t]&0x0f) == 0x00)
				TIME[i_t] -= 7; 		
		}
	}
	else if(i_t == 1||i_t == 0)				  //�����֡���
	{
		 if(TIME[i_t] == 0)
			TIME[i_t] = 59;
		else 
		{	
			TIME[i_t]--;
			if((TIME[i_t]&0x0f) == 0x00)
				TIME[i_t] -= 7; 		
		}
	}
}

/*******************************************************************************
* �� �� ��         : Ds1302ShowTime
* ��������		   : ��ʾʱ����Ϣ
*******************************************************************************/

void Ds1302ShowTime()
{
	led_alarm2 = 1 ;				 //��������Ӧ�ر�
	led_alarm1 = 1 ;

	while(flag == 1)
	{	
		display(1,TIME[0]&0x0f);	 //ͨ���������ʾʱ��
		display(2,TIME[0]/16);
		display(3,17);
		display(4,TIME[1]&0x0f);	
		display(5,TIME[1]/16);
		display(6,17);
		display(7,TIME[2]&0x0f);	
		display(8,TIME[2]/16);

		if(menu_10 >= 10)			 //��ʱɨ�谴��
		{
			menu_10 = 0;
			menu_scan();			 //�˵�ɨ��
			time_scan();			 //ʱ�����ɨ��
			Key4_TimeScan();		 //key4ɨ�裨���桢����ʱ����ʾģʽ��
			alarm1_led();			 //����1����
			alarm2_led();			 //����2����
		}

		if(readtime == 1)			 //��ʱ����ʾ
			Ds1302ReadTime();
		if(readtime == 0)			 //���³�ʼ��ʱ��
			Ds1302Init();
	}	
}
#endif
