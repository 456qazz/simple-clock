/*****************************************************
				�Գ��õĺ�����������������
*****************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

#include"reg52.h"
#include"intrins.h"

#define uint  unsigned int
#define uchar unsigned char

void save_clock();			//����ʱ��
void save_clock2();			//����ʱ��
void Ds1302ReadTime();		//��ȡʱ��
void alarm1_led();			//����1����
void alarm2_led();			//����2����

uchar flag = 1;				//�˵�flag
bit set_time = 1;			//����ʱ��Ŀ���
bit readtime = 0;			//ʱ����ʾ����
bit alarm_1 = 0;			//������Ӧ����
bit alarm_2 = 0;			//������Ӧ����
sbit led_long = P2^7;		//�������̰�ָʾ��
sbit led_short = P2^6;
sbit led_alarm1 = P2^0;		//����1��Ӧ��LED��
sbit led_alarm2 = P2^1;		//����2��Ӧ��LED��

uchar ms_10 = 0,menu_10 = 0,alarm_10 = 0,ms_300 = 0,alarm1_100 = 0,alarm2_100 = 0;//ʱ��Ƭ����
 
uchar alarm_clock[3] = {0x00, 0x00,0x00};	//����1ʱ�䣬�洢˳��ֱ��� ���ء��֡�ʱ	 
uchar alarm_clock2[3] = {0x00, 0x00,0x00};	//����2ʱ�䣬�洢˳��ֱ��� ���ء��֡�ʱ	 

sbit key1 = P3^1; 			//��������
sbit key2 = P3^0; 
sbit key3 = P3^2;
sbit key4 = P3^3;

/**************************************************************
						�������ʾ����
***************************************************************/
sbit wei_A = P2^2;
sbit wei_B = P2^3;
sbit wei_C = P2^4;

uchar duan_table[] = {		   //0-15;16����ʾ;17��ʾ'-';18��ʾ��ĸn
						0x3f,0x06,0x5b,0x4f,0x66,0x6d,
						0x7d,0x07,0x7f,0x6f,0x77,0x7c,
						0x39,0x5e,0x79,0x71,0x00,0x40,0x37};	

void display(uchar i, uchar j) //�������ʾ������iΪλѡ��jΪ��ѡ
{
	P0 = duan_table[16];	   //����
	switch (i)
	{
		case 1: wei_C = 0; wei_B = 0; wei_A = 0; break;
		case 2: wei_C = 0; wei_B = 0; wei_A = 1; break;
		case 3: wei_C = 0; wei_B = 1; wei_A = 0; break;
		case 4: wei_C = 0; wei_B = 1; wei_A = 1; break;
		case 5: wei_C = 1; wei_B = 0; wei_A = 0; break;
		case 6: wei_C = 1; wei_B = 0; wei_A = 1; break;
		case 7: wei_C = 1; wei_B = 1; wei_A = 0; break;
		case 8: wei_C = 1; wei_B = 1; wei_A = 1; break;
	}
	P0 = duan_table[j];
	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); //����ɨ��Ƶ�ʹ��ߣ���ʾ�������
	P0 = duan_table[16];
}
							   //������˸��ʾ������λ����ַ
							   //��ָ��λ��˸������ֱ����ʾ
void shanshuo_display(uchar position,uchar alarm_time[3])	
{															
	switch (position)
	{
		case 0: display(8, alarm_time[2] / 10);
				display(7, alarm_time[2] % 10);
				display(6, 17);
				display(5, alarm_time[1] / 10);
				display(4, alarm_time[1] % 10);
				display(3, 17);
	
				if (ms_300 >= 150)						//������˸
				{
					if (alarm_time[0])					//��ʾon
					{
						display(2, 0);
						display(1, 18);
					}
					else								//��ʾoff
					{
						display(2, 15);
						display(1, 15);
					}
				}
		
				if (ms_300 >= 300)
				{
					display(1, 16);
					display(2, 16);
					ms_300 = 0;						    //��˸ʱ��Ƭ����
				}
				break;
		
		case 1: display(8, alarm_time[2] / 10);
				display(7, alarm_time[2] % 10);
				display(6, 17);
				display(3, 17);
				if (alarm_time[0])				 	    //��ʾon
				{
					display(2, 0);
					display(1, 18);
				}
				else								    //��ʾoff
				{
					display(2, 15);
					display(1, 15);
				}
		
				if (ms_300 >= 150)				        //������˸
				{
					display(5, alarm_time[1] / 10);
					display(4, alarm_time[1] % 10);
				}
				if (ms_300 >= 300)
				{
					display(4, 16);
					display(5, 16);
					ms_300 = 0;							//��˸ʱ��Ƭ����
				}								   		
				break;

		case 2: display(6, 17);
				display(5, alarm_time[1] / 10);
				display(4, alarm_time[1] % 10);
				display(3, 17);
		
				if (alarm_time[0])						//��ʾon
				{
					display(2, 0);
					display(1, 18);
				}
				else									//��ʾoff
				{
					display(2, 15);
					display(1, 15);
				}
				if (ms_300 >= 150)						//ʱ��˸
				{
					display(8, alarm_time[2] / 10);
					display(7, alarm_time[2] % 10);
				}
				if (ms_300 >= 300)
				{
					display(7, 16);
					display(8, 16);
					ms_300 = 0;							//��˸ʱ��Ƭ����
				}
				break;

	}
}
/***************************************************************
			ʱ��������������������λ���������ݵĵ�ַ��
***************************************************************/

void time_up(uchar position,uchar alarm_time[3])					 //ʱ������
{
	if (position == 2)
	{
		if (alarm_time[position] >=23)
			alarm_time[position] = 0;
		else
			alarm_time[position]++;
	}
	else if (position == 1)
	{
		if (alarm_time[position] >= 59)
			alarm_time[position] = 0;
		else
			alarm_time[position]++;
	}
	else if (position == 0)
		alarm_time[0] = 1;
}

void time_down(uchar position, uchar alarm_time[3])						//ʱ���С
{
	if (position == 2)
	{
		if (alarm_time[position] == 0)
			alarm_time[position] = 23;
		else
			alarm_time[position]--;
	}
	else if (position == 1)
	{
		if (alarm_time[position] == 0)
			alarm_time[position] = 59;
		else
			alarm_time[position]--;
	}
	else if (position == 0)
		alarm_time[0] = 0x00;
}
/***************************************************************
					    	�˵���Ӧ����
***************************************************************/
void menu_handler();						//�˵���Ӧ����

uchar last_time = 0;   						//������ʱ����
bit  long_press = 0,change_menu = 0;		//������ǡ��ı�˵�������

void menu_scan()							//�˵�ɨ�躯��
{
	static uchar menu_state = 0;   		

	switch (menu_state)
	{
		case 0: if (key1 == 0)
				{
					menu_state = 1;
				}
				else
					menu_state = 0;
				break;
	
		case 1: if (key1 == 0)
				{
					menu_state = 2;
				}
				else
					menu_state = 0;
				break;
	
		case 2: if (key1 != 0)
				{
					
					menu_handler(); 		//��Ӧ����
					menu_state = 0;
				}
				else
					menu_state = 3;			//����״̬3���ж��Ƿ񱻳���
				break;
	
		case 3: if (key1 == 0)
				{
					last_time++;
					if (last_time >= 200)	//����
					{
						last_time = 0;
						long_press = 1;		//ȷ���ǳ���
						if(flag == 1)		//�����������У�����ֹͣʱ���߶�
						{
							readtime = 0;	//ֹͣ��ȡʱ��
							set_time = 1;	//����ʱ������
						}
					}
				}
				else
					menu_state = 2;				 
				break;
	}
}

void menu_handler()							//�˵���Ӧ����
{
	if(long_press)
	{
		long_press = 0;
		led_long = ~led_long;				//����ָʾ��
		change_menu = ~change_menu;			
	}
	
	else if(change_menu)
	{
		led_short = ~led_short; 			//�̰�ָʾ��
		flag++;								//�л��˵�
		if(flag >= 5)
			flag = 1;
		if(flag == 1)						//����ʱ������
		{
			Ds1302ReadTime();
			readtime = 0;
			set_time = 1;
		}
		else if(flag == 3)
				save_clock();			 	//�˵��л����̣�����ʱ��
		else if(flag == 4)
				save_clock2();			    //�˵��л����̣�����ʱ��
		if(flag != 1)
			{
				readtime = 1;				//��ʱ���ȡ
				set_time = 0;				//�ر�ʱ������
			}
	}		
}

/**************************************************************
					��ʼ����ʱ�����жϺ���
**************************************************************/

void Timer0_init()						//��ʱ��0�ĳ�ʼ��  
{
		
	TMOD = 0x01;		  
	TH0 = (65536 - 1000)/256;			//��ʼ��1ms
	TL0 = (65536 - 1000)%256;
	EA  = 1;
	ET0 = 1;
	TR0 = 1;							//������ʱ��
}

void T0_time() interrupt 1	 			//��ʱ���жϺ���
{
	TH0 = (65536 - 1000)/256;
	TL0 = (65536 - 1000)%256;

	ms_10++;							//����ɨ��ʱ��Ƭ
	ms_300++;							//��˸��ʾʱ��Ƭ
	menu_10++;							//�˵�ɨ��ʱ��Ƭ
   	alarm_10++;							//����ʱ��Ƭ
	alarm1_100++;
	alarm2_100++;
}


#endif