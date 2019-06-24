/***********************************************************
					   ʵ������2����
***********************************************************/
#ifndef __ALARM2_H__
#define __ALARM2_H__
#include"main.h"
#include"iic.h"

void save_clock2();			 //����ʱ��
void Key4_Handler2();		 //����4�Ĵ���

extern uchar flag;
extern uchar alarm_10;		 
extern bit long_press;
extern bit alarm_2;
extern uchar alarm_clock2[];

/***************************************************************
						�������ӵļ���ɨ��
***************************************************************/
uchar clock2_keytemp = 0,i2_c = 0;			  //clock2_keytempΪ������ֵ��alarm_clock2[i2_c]						  
void clock2_scan()							 //key2��key3����ɨ��
{
	static char clock_state = 0; 
	
	if (key2 == 0)
		clock2_keytemp = 2;
	else if (key3 == 0)
		clock2_keytemp = 3;
	else
		clock2_keytemp = 0;

 	switch(clock_state)
	{
		case 0: if(clock2_keytemp == 0)
				{
					clock_state = 1;
				}
				else
					clock_state = 0;
				break;		
		case 1: if(clock2_keytemp == 0)
				{
					clock_state = 2;
				}
				else
					clock_state = 0;
				break;
		case 2: if(clock2_keytemp != 0) 
				{
					if(clock2_keytemp == 2)
					 	time_up(i2_c,alarm_clock2);	
					else if(clock2_keytemp == 3)
						time_down(i2_c,alarm_clock2);
					clock_state = 0;
				}
				else
					clock_state = 2;
				break;
	}
}

void Key4_Scan2()					   		//Key4ɨ��
{
	static uint Key4_State = 0; 			//Key4״̬

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
				
				Key4_Handler2(); 			//�԰������д���
				Key4_State = 0;
			}
			else
				Key4_State = 3;
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
				Key4_State = 2;				 
			break;
	   		}
}

void Key4_Handler2()					//key4��������
{
	if(long_press == 1)
	{
		long_press = 0;
		led_long = 0;				    //����ָʾ��
		save_clock2();					//��������
		flag = 1;						//�ص�ʱ����ʾģʽ
	}
	else if(long_press == 0)
	{
	 	led_short = ~led_short;		    //�̰�ָʾ��
		i2_c++;
		if(i2_c > 2)
			i2_c = 0;
	}
}
/****************************************************************
						 ��������
****************************************************************/

uchar flash_read2 = 0;
void save_clock2()		  //��������
{
	AT2402_SendByte(0xa6,*alarm_clock2,alarm_clock2[0]);	
	AT2402_SendByte(0xa8,*alarm_clock2+1,alarm_clock2[1]);
	AT2402_SendByte(0xaa,*alarm_clock2+2,alarm_clock2[2]);
	flash_read2 = 1;
}

void read_clock2()		  //��ȡ�趨������ʱ��
{
	alarm_clock2[0] = AT2402_ReceiveByte(0xa7,*alarm_clock2);
	alarm_clock2[1] = AT2402_ReceiveByte(0xa9,*alarm_clock2+1);
	alarm_clock2[2] = AT2402_ReceiveByte(0xab,*alarm_clock2+2);
	flash_read2 = 0;
}

void alarm2_led()		  //������Ӧ������LED��
{
	if(alarm_clock2[0] == 1 && (TIME[1]/16)*10 + (TIME[1]&0x0f) == alarm_clock2[1] && (TIME[2]/16)*10 + (TIME[2]&0x0f) ==alarm_clock2[2])
		alarm_2 = 1;
	else 
	{
		led_alarm1 = 1;
		alarm_2 = 0;
	}
}

/***************************************************************
							����2������
****************************************************************/
void alarm2()
{
	ms_300 = 0;				 		//��˸ʱ��Ƭ����
	while(flag == 3)
	{
		if(flash_read2)				//ˢ�£���ȡ�±������ӵ�ʱ��
			read_clock2();
		shanshuo_display(i2_c,alarm_clock2);//��˸��ʾ��Ҫ������λ
				
		if(alarm_10 >=10)	 		//��ʱɨ�谴��
		{
			alarm_10 = 0;
			clock2_scan();			//key2��key3ɨ��
			Key4_Scan2();			//key4ɨ��
			menu_scan();		 	//�˵�ɨ��
		}
	}
}
#endif