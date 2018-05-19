
#include "msp430x14x.h"   //430�Ĵ���ͷ�ļ�
#include "Uart.h"         //����ͨѶ�����ͷ�ļ�

typedef unsigned char uchar;
typedef unsigned int  uint;

char str[] = "UartWriteChar ������������!"; //ROM��һ���ַ���

/****************************************************************************
* ��    �ƣ�ClkInit
* ��    �ܣ�ʱ��ϵͳ��ʼ��  MCLKΪ8MHz��SMCLKΪ1MHz
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void ClkInit()
{
    char i;
    BCSCTL1 &= ~XT2OFF;             //��XT2����
    IFG1&=~OFIFG;                   //����񵴴����־
    while((IFG1&OFIFG)!=0)
    {
        for(i=0;i<0xff;i++);
        IFG1&=~OFIFG;               //����񵴴����־
    }
    BCSCTL2 |= SELM_2+SELS+DIVS_3;  //MCLKΪ8MHz��SMCLKΪ1MHz
}

/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
********************************************/
void Delays(void)
{
    uchar i=20;
    uint j;

    while(i--)
    {
            j=2000;
            while(j--);
    }
}

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;

  char value = 0;
    
  //���ڳ�ʼ��֮ǰ������ô˺�����SMCLK��Ϊ1M��9600���²����ʿ��Բ�����
  ClkInit();
    
  UartInit(9600,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT();
    
  UartWriteStr(str);
  UartWriteChar(0x0d);    //����"����"(\r)"
  UartWriteChar(0x0a);    //����"�س�"(\n)"   
    
  UartWriteStr("������Դ����շ�����\r\n");
    
  while(1)                    //���ڲ���
  {
    int k=50;

    while(k--)
    {
       UartWriteChar(value);     //���յ������ݷ���   
       value++;
       Delays();
    }
  }
  //return 0;
}
