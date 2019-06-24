/**************************************************************
						���ͷ�ļ�
**************************************************************/

#ifndef __STOPWATCH_H__
#define __STOPWATCH_H__

#include"main.h"
#include"reg52.h"

void stop_watch();		 //���������
void ShowTime();		 //�������ʾʱ�� 
void stopwatch_scan();	 //key2��key3����ɨ��
void Key4Stop_Scan(); 	 //key4ɨ�躯��
void Key4Stop_Handler(); //key4����������

extern uchar flag;
extern bit long_press;

uchar keytemp;						  //����ֵ
bit  stop_flag = 1;					  //������ͣ���
uchar stop_time[]={0x00,0x00,0x00};	  //����10���룬�룬��

/********************************************************
						���������
********************************************************/

void stop_watch()
{	
	Timer0_init();		   //��ʼ����ʱ��0
	ms_10 = 0;			   //ʱ��Ƭ��0

	while(flag == 4)	   //ѡ�������
	{
		ShowTime();

		if(ms_10 >= 10)				   //��ʱ10�������ɨ��
		{
		 	ms_10 = 0;
			menu_scan();			   //�˵�ɨ��
			stopwatch_scan();		   //key2,key3ɨ��
			Key4Stop_Scan();		   //key4ɨ��
			
			if(!stop_flag)			   //ʱ�����ӣ������ͣ��ֹͣ����
				stop_time[0]++;
		}
		
		if(stop_time[0] >= 99)		   //��ʱ�����ӵĴ���
		{
			stop_time[0] = 0;
		 	stop_time[1]++;

			if(stop_time[1] >= 60)
			{
				stop_time[1] = 0;
				stop_time[2]++;

				if(stop_time[2] > 99)  //ʱ��ﵽ���ֵ�����¸�ֵ
				{					   
					stop_time[0] = 0x00;
					stop_time[1] = 0x00;
					stop_time[2] = 0x00;
				}
			}
		}
		
	}
}

/**************************************************************
						  ����ɨ��
**************************************************************/
void stopwatch_scan()					//����2��3ɨ��
{
	static char state = 0; 

	if (key2 == 0)
		keytemp = 2;
	else if (key3 == 0)
		keytemp = 3;
	else
		keytemp = 0;

 	switch(state)
	{
		case 0: if(keytemp == 0)
				{
					state = 1;
				}
				else
					state = 0;
				break;		
		case 1: if(keytemp == 0)
				{
					state = 2;
				}
				else
					state = 0;
				break;
		case 2: if(keytemp != 0) 
				{							 	//�԰������д���
					if(keytemp == 2)
						stop_flag = ~stop_flag;	//��ͣ
					else if(keytemp == 3)
						stop_flag = 1;	 		//ֹͣ
					state = 0;
				}
				else
					state = 2;
				break;
		
	}
}

void Key4Stop_Scan()			   //����4ɨ��
{
	static uint Key4_State = 0; 

	switch (Key4_State)
	{
		case 0: if (key4 == 0)
				{
					Key4_State = 1;
				}
				else
					Key4_State = 0;
				break;
	
		case 1: if (key4 == 0)
				{
					Key4_State = 2;
				}
				else
					Key4_State = 0;
				break;
	
		case 2: if (key4 != 0)
				{
					
					Key4Stop_Handler(); 		//�԰������д���
					Key4_State = 0;
				}
				else
					Key4_State = 3;				//����״̬3���ж��Ƿ񱻳���
				break;
	
		case 3: if (key4 == 0)
				{
					last_time++;
					if (last_time >= 200)		//����
					{
						last_time = 0;
						long_press = 1;			//ȷ���ǳ���
					}
				}
				else
					Key4_State = 2;				 
				break;

		   		}
}

void Key4Stop_Handler()				//key4��������
{
	if(long_press == 1)
	{
		long_press = 0;
		led_long = 0;				//����ָʾ��
		flag = 1;					//�ص�ʱ����ʾģʽ
	}

	else if(long_press == 0)
	{
	 	led_short = ~led_short; 	//�̰�ָʾ�ƣ��������
		stop_time[2] = 0;
		stop_time[1] = 0;
		stop_time[0] = 0;
	}
}

/**************************************************************
						�������ʾʱ��
***************************************************************/
void ShowTime()
{
	display(1,stop_time[0]%10);	
	display(2,stop_time[0]/10);
	display(3,17);
	display(4,stop_time[1]%10);	
	display(5,stop_time[1]/10);
	display(6,17);
	display(7,stop_time[2]%10);	
	display(8,stop_time[2]/10);	
}
#endif