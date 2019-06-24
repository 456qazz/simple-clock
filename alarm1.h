/***********************************************************
					����1����ͷ�ļ�
***********************************************************/
#ifndef __ALARM1_H__
#define __ALARM1_H__
#include"main.h"
#include"iic.h"

void save_clock();			 //����ʱ��
void Key4_Handler();		 //����4�Ĵ���

extern uchar flag;
extern uchar alarm_10;		 
extern bit long_press;
extern bit alarm_1;
extern uchar alarm_clock[];
extern uchar alarm1_100;

/**************************************************************
						�������ӵļ���ɨ��
**************************************************************/

uchar clock_keytemp = 0,i_c = 0;	//clock_keytempΪ������ֵ��i_cΪ������λ					  						 	    	
void clock_scan()					//key2��key3����ɨ��
{
	static char clock_state = 0; 
	
	if (key2 == 0)
		clock_keytemp = 2;
	else if (key3 == 0)
		clock_keytemp = 3;
	else
		clock_keytemp = 0;

 	switch(clock_state)
	{
		case 0: if(clock_keytemp == 0)
				{
					clock_state = 1;
				}
				else
					clock_state = 0;
				break;		
		case 1: if(clock_keytemp == 0)
				{
					clock_state = 2;
				}
				else
					clock_state = 0;
				break;
		case 2: if(clock_keytemp != 0) 
				{
					if(clock_keytemp == 2)
					 	time_up(i_c,alarm_clock);	  //ʱ������
					else if(clock_keytemp == 3)		  
					 	time_down(i_c,alarm_clock);	  //ʱ���С
					clock_state = 0;
				}
				else
					clock_state = 2;
				break;
	}
}

void Key4_Scan()					    //key4ɨ��
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
				
				Key4_Handler(); 		//�԰������д���
				Key4_State = 0;
			}
			else
				Key4_State = 3;
			break;

	case 3: if (key4 == 0)
			{
				last_time++;
				if (last_time >= 200)	//ȷ���ǳ��������볤���Ĳ���
				{
					last_time = 0;
					long_press = 1;		//������־
				}
			}
			else
				Key4_State = 2;				 
			break;
	   		}
}

void Key4_Handler()						//key4��������
{
	if(long_press == 1)
	{
		long_press = 0;
		save_clock();					//��������
		led_long = 0;				    //����ָʾ��
		flag = 1;						//�ص�ʱ����ʾģʽ
	}
	else if(long_press == 0)
	{
	 	led_short = ~led_short;		    //�̰�ָʾ��
		i_c++;
		if(i_c >= 3)
			i_c = 0;
	}
}

/****************************************************************
						 ��������
****************************************************************/

uchar flash_read = 1;		//��������ʱ������ˢ��
void save_clock()			//�������ݺ���
{
	AT2402_SendByte(0xa0,*alarm_clock,alarm_clock[0]);	
	AT2402_SendByte(0xa2,*alarm_clock+1,alarm_clock[1]);
	AT2402_SendByte(0xa4,*alarm_clock+2,alarm_clock[2]);
	flash_read = 1;
}

void read_clock()			//��ȡ�趨������ʱ��
{
	alarm_clock[0] = AT2402_ReceiveByte(0xa1,*alarm_clock);
	alarm_clock[1] = AT2402_ReceiveByte(0xa3,*alarm_clock+1);
	alarm_clock[2] = AT2402_ReceiveByte(0xa5,*alarm_clock+2);
	flash_read = 0;
}


/**************************************************************
						������Ӧ��LED����
**************************************************************/
										
void alarm1_led()						//������Ӧ������LED��
{
	if(alarm_clock[0] == 1 &&(TIME[1]/16)*10 + (TIME[1]&0x0f) == alarm_clock[1] && (TIME[2]/16)*10 + (TIME[2]&0x0f) ==alarm_clock[2])
		alarm_1 = 1;					//������Ӧ��־
	else 
	{
		led_alarm1 = 1;
		alarm_1 = 0;
	}
}


/**********************************************************
						����1������
***********************************************************/
void alarm1()
{
	ms_300 = 0;						//��˸ʱ��Ƭ����
	while(flag == 2)
	{
		if(flash_read)				//ˢ�£���ȡ�±������ӵ�ʱ��
			read_clock();
		shanshuo_display(i_c,alarm_clock);//��˸��ʾ��Ҫ������λ
		
		if(alarm_10 >=10)			//��ʱɨ�谴��
		{
			alarm_10 = 0;
			clock_scan();			//key2��key3ɨ��
			Key4_Scan();			//key4ɨ��
			menu_scan();		 	//�˵�ɨ��
		}
	}
}

#endif