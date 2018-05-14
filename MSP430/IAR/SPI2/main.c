
#include <msp430x14x.h>
#include "Spi.h"         //����ͨѶ�����ͷ�ļ�

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

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  // ����ģʽ��������25000,8λ����λ������ģʽ��ʱ��ģʽ0�������spi.c��
  SpiMasterInit(25000,8,3,0);
  _EINT(); 
    
    
  while(1)                    //���ڲ���
  {
      SpiWriteData(0X20);     //ֻд��
      char a = SpiWriteData(0xff);    //ֻ��ȡ

  }

}

