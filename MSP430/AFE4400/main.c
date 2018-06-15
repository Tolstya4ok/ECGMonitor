/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� main.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ������ͨѶ������ʾ������
 *      ˵����  
 *          �ó���ʾ������ͨѶ�������ڵĸ�����ʹ�÷�����ͬʱ����Ϊ
 *          ���ڲ��Գ����á��ó������ʾ��Ҫͨ��Windows�Դ���"����
 *          �ն�"��������Ǵ��ڵ��������С�
 *
 *          ���к�����������ַ���  
 *          UartWriteChar ������������!
 *          ������Դ����շ�����
 *          Ȼ�󣬽���ԭ�ַ��ظ������ڵ���������ʲô�ͽ���ʲô
 *
 * ������ʶ��   ����ԭ20110622
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/
#include <stdint.h>
#include "msp430x14x.h"   //430�Ĵ���ͷ�ļ�
#include "Uart.h"         //����ͨѶ�����ͷ�ļ�
#include "Spi.h"
#include "MSP430_AFE_SPI.h"
#include "TI_AFE4400.h"
#include "TI_AFE4400_setting.h"

char str[] = "UartWriteChar"; //ROM��һ���ַ���
char str1[] = "start conversation!"; //ROM��һ���ַ���
char str2[] = "stop conversation!"; //ROM��һ���ַ���
char q = 'Q';

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
    BCSCTL2 |= SELM_2+SELS+DIVS_0;  //MCLKΪ8MHz��SMCLKΪ1MHz
}

/*******************************************
�������ƣ�Delays
��    �ܣ���ʱһ��
��    ������
����ֵ  ����
********************************************/
void Delays(uint32_t k)
{
    uint8_t i=100;
    uint32_t j;
    while(i--)
    {
            j=k;
            while(j--);
    }
}

/****************************************************************************
* ��    �ƣ�main������
* ��    �ܣ����ô��ڣ������Ϣ���Ӵ��ڶ�����������������ݣ����Դ����շ�
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  
  uint8_t count=3;
  uint8_t read_buf[3];
  unsigned long val;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //���ڲ����У��յ����ֽ�
  // ����ģʽ��������4000000,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
  SpiMasterInit(4000000,8,3,1);
  UartInit(115200,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;  
  Delays(10000);
  TI_AFE4400_WriteRegSettings();  
  while(1)                    //���ڲ���
  {
      chr = UartReadChar();
      switch(chr)
      {
      case 'S':
        UartWriteStr(str1);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 0x000001ul, 3);
        UartWriteChar(q);
        break;
      case 'T':
        UartWriteStr(str2);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 0x000000ul, 3);
        UartWriteChar(q);
        break;
      case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        {
            val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            read_buf[0] = val & 0xFF;
            read_buf[1] = (val>>8) & 0xFF;
            read_buf[2] = (val>>16) & 0xFF;
            UartWriteint(read_buf[0]);
            UartWriteint(read_buf[1]);
            UartWriteint(read_buf[2]);
            UartWriteChar(0x0d);    //����"����"(\r)"
            UartWriteChar(0x0a);    //����"�س�"(\n)"  
            Delays(10);
        }
            break;


      }
      
  }

}

