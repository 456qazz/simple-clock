#include"ds1302.h"		 //��ʾʱ��
#include"stopwatch.h"	 //���
#include"alarm1.h"		 //����1
#include"alarm2.h"		 //����2

extern uchar flag;

void main()
{
	Ds1302Init();		 //��ʼ��DS1302
	Timer0_init();		 //��ʼ����ʱ��0

	while(1)
	{		
		switch(flag)
		{
			case 1:Ds1302ShowTime();break;	   //ʱ��
			case 2:alarm1();break;			   //����1
			case 3:alarm2();break;			   //����2
			case 4:stop_watch();break;		   //���
		}
		
	}
}
