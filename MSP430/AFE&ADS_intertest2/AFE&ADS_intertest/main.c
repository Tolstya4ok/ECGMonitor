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
#include "MSP430_Spi.h"
#include "TI_ADS1293.h"
#include "TI_ADS1293_setting.h"

uint8_t c=1;
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
  P4DIR = 0x01;
  P4OUT = 0x01;
  P2DIR = 0xfa;
  P2OUT = 0xf0;
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  CCTL1 = CCIE;                            // CCR1 �ж�ʹ��  
  CCR1 = 50000;  
  TACTL = TASSEL_2 + MC_2 + ID_3;          // SMCLK = 1MHz, ��������ģʽ 
  uint8_t count=3;

  uint8_t read_buf[3];
  uint8_t read_buff[8] = {0,0,0,0,0,0,0,0};
  
  unsigned long val;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //���ڲ����У��յ����ֽ�
  // ����ģʽ��������4000000,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
  SpiMasterInit(4000000,8,3,0);
  UartInit(115200,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;
  Delays(10000);
  TI_ADS1293_WriteRegSettings();
  TI_AFE4400_WriteRegSettings();  
  while(1)                    //���ڲ���
  {
    

    chr = UartReadChar();
    
    switch(chr)
      {
      case 'S':
        UartWriteStr(str1);
        TI_ADS1293_SPIWriteReg(0x00, 0x01);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read 0x000001ul
        UartWriteChar(q);
        break;
      case 'T':
        //UartWriteStr(str2);
        TI_ADS1293_SPIWriteReg(0x00, 0x00);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 0, 3);//enable read
        //UartWriteChar(q);
        break;
      case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        {
          if(c==1)
          {
            P2OUT = 0xf0;
          }
          else if(c==2)
          {
            P2OUT = 0xff;
            
          }
          else if(c==3)
          {
            P2OUT = 0xf0;
            //TI_ADS1293_SPIStreamReadReg(read_buf, count);           
            //read_buff[2] = read_buff[2] & ( read_buf[1]>>4 );
            //read_buff[3] = read_buf[1] << 4;
            //read_buff[3] = read_buff[3] & (( read_buf[2]>>4 ) | 0x0c);
          }
          else
          {
            P2OUT = 0xff;
            //TI_ADS1293_SPIStreamReadReg(read_buf, count);           
            //read_buff[3] = read_buff[3] & ( read_buf[1]>>6 );
            //read_buff[4] = read_buf[1] << 2;
            //read_buff[4] = read_buff[4] & (( read_buf[2]>>6 ) | 0x03);
            //val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            //read_buff[5] = (val>>12) & 0xFF;
            //read_buff[6] = (val>>4) & 0xF0;
            //val = TI_AFE4400_SPIAutoIncReadReg(LED2VAL, count);
            //read_buff[6] = read_buff[6] & (( val>>16 ) | 0x0F);
            //read_buff[7] = (val>>8) & 0xFF;
            //UartWriteint(read_buff[0]);
            //UartWriteint(read_buff[1]);
            //UartWriteint(read_buff[2]);
            //UartWriteint(read_buff[3]);
            //UartWriteint(read_buff[4]);
            //UartWriteint(read_buff[5]);
            //UartWriteint(read_buff[6]);
            //UartWriteint(read_buff[7]); 
            c = 0;
          }
          
        }
        break;
      case 'B':
        for(uint8_t k = 1;k<48;k++)
        {
            val = TI_AFE4400_SPIAutoIncReadReg(k, count);
            read_buf[0] = val & 0xFF;
            read_buf[1] = (val>>8) & 0xFF;
            read_buf[2] = (val>>16) & 0xFF;
            UartWriteint(read_buf[2]);
            UartWriteint(read_buf[1]);
            UartWriteint(read_buf[0]);
            UartWriteChar(0x0d);    //����"����"(\r)"
            UartWriteChar(0x0a);    //����"�س�"(\n)"  
            Delays(10);
        }
        break;
        case 'A':
        for(uint32_t k = 0;k<1000000;k++)
        {
            TI_AFE4400_SPIAutoIncWriteReg(0x00, 0x010203, 3);//enable read 
            Delays(10);
        }
        break;
        case 'E':
        for(uint32_t k = 0;k<1000;k++)
        {
            TI_AFE4400_WriteRegSettings();
            UartWriteChar(0x0d);    //����"����"(\r)"
            UartWriteChar(0x0a);    //����"�س�"(\n)"
            Delays(10);
        }
        break;
      }

    }
    
  }



#pragma vector=TIMERA1_VECTOR  
__interrupt void Timer_A1(void)  
{  
    switch(TAIV){  
        case 2:  
            CCR1 += 50000;                    //   
            c = c + 1;            
    }      
}  



