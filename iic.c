#include "iic.h"

void delayus(unsigned char x)	//��ʱ����
{
	while(x)
	{
		x--;
	}
}

void iic_start(void)    //��ʼ���亯��
{
	SDA = 1;			//�������ź���
	delayus(5);				
	SCL = 1;			//����ʱ����
	delayus(5);				
	SDA = 0;			//�����ź��ߣ���ʾ���俪ʼ
	delayus(3);				
	SCL = 0;   			//����ʱ����
}

void iic_stop(void)		//�������亯��
{
	SCL = 0;			//����ʱ����
	SDA = 0;			//�����ź���
	SCL = 1;			//����ʱ���ߣ���ʾ�������
	delayus(5);			
	SDA = 1;			//�����ź���
}

void iic_ask(bit askbit)	//����Ӧ����
{
	SCL = 0;				//����ʱ����
  if(askbit)				//���Ӧ�𣬾ͽ��ź�������
		SDA = 0;
	else					//�����Ӧ�𣬾ͽ��ź�����һ
		SDA = 1;
	delayus(5);			
	SCL = 1;				//����ʱ����
	delayus(5);			
	SCL = 0;				//����ʱ����
	SDA = 1;				//�����ź���
	delayus(5);			
}

unsigned char iic_waitask(void)		//�����ȴ��ӻ�Ӧ���ź�
{
	SCL = 0;				//������ʱ����
	SDA = 1;				//�ͷ����ߣ��������ź���
	delayus(5);			
	SCL = 1;				//����ʱ����
	delayus(5);			
	if(SDA)					//�鿴�ź��ߵĵ�ƽ
	{
		SCL = 0;			//����ʱ���ߣ�����ź���Ϊ�ߣ����Ǵӻ���Ӧ�𣬷��ʹ������ָ�������0
		iic_stop();
		return 0;
	}
	else
	{
		SCL = 0;			//����ʱ���ߣ�����ź���Ϊ�ͣ����ӻ�Ӧ�𣬷���1
		return 1;
	}
}

void iic_sendbyte(unsigned char byte)			//�����������ݵ��ӻ�����
{
	unsigned char i;
	
	SCL = 0;					//������ʱ����
	for (i=0;i<8;i++)			//�����ɸ�λ����λ����
	{
		SDA = (byte & 0x80); 	//ȡ���ݵ����λ,������λ�����λ�����͵��ӻ�
		delayus(5);				
		SCL = 1;				//����ʱ���ߣ�����һ��������
		byte <<= 1;				//��������һλ
		delayus(5);				
		SCL = 0;				//����ʱ����
	}
}

unsigned char iic_readbyte(void)//�������մӻ�������������
{
	unsigned char i,byte;
	
	SCL=0;						//������ʱ����
	delayus(5);										
	for (i=0;i<8;i++)			//���յ������ɸ�λ����λ
	 {
		 SCL = 1;				//����ʱ���ߣ�����������
		 delayus(5);								
		 byte <<= 1;			//�Ƚ��������ݵı�������һλ
		 byte |= SDA;		    //�ٺͽ��յ���������������
		 SCL = 0;				//����ʱ����
		 delayus(5);			//����ʱ����
	 }
	 return byte;				//���ؽ��յ�������
}

/******************************************************************************
�������ܣ�    ��AT2402 EEPROMоƬдһ������
��ڲ�����		saddr IIC�豸�ĵ�ַ�� baddr Ҫд�����ݵĵ�ַ�� byte Ҫд������
******************************************************************************/
void AT2402_SendByte(unsigned char saddr, unsigned char baddr, unsigned char byte)
{
	unsigned char ask;
	
	saddr &= 0xfe;					//�Ƚ��豸��ַ�����һλ���㣬��ʾ������Ҫ��ӻ�д����
	
	iic_start();					//���Ϳ�ʼ����ָ��
	iic_sendbyte(saddr);			//�����豸��ַ
	
	ask = iic_waitask();			//���ӻ���Ӧ�������ŵ����Ӧ���־����
	if (ask)						//���Ӧ���ˣ���������Ĳ��������û�У�ֱ�ӽ���
	{
		iic_sendbyte(baddr);		//����Ҫ������ݵĵ�ַ
		ask = iic_waitask();		//���ӻ���Ӧ�������ŵ����Ӧ���־����
		if (ask)					//���Ӧ���ˣ���������Ĳ��������û�У�ֱ�ӽ���
		{
			iic_sendbyte(byte);		//����Ҫд��EEPROM������
			ask = iic_waitask();	//���ӻ���Ӧ�������ŵ����Ӧ���־����
		}	
	}
	iic_stop();						//��������
}

unsigned char AT2402_ReceiveByte(unsigned char saddr,unsigned char baddr)
{
	unsigned char ask, receive_data;
	
	saddr &= 0xfe;
	
	iic_start();	
	iic_sendbyte(saddr);
	
	ask = 0;
	ask = iic_waitask();
	if (ask)
	{
		iic_sendbyte(baddr);

		ask = 0;
		ask = iic_waitask();
		if (ask)
		{
			iic_start();
			saddr |= 0x01;
			iic_sendbyte(saddr);
			
			ask = 0;
			ask = iic_waitask();
			if (ask)
			{
				receive_data = iic_readbyte();
				iic_ask(0);
				iic_stop();
				return receive_data;
			}
			else 
			{
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}
	else 
	{
		return 0;
	}
}
