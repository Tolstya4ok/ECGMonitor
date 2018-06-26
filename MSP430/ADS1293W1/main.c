#include <stdint.h>
#include <msp430x14x.h>
#include "Spi.h"         //����ͨѶ�����ͷ�ļ�
#include "MSP430_Spi.h" 
#include "TI_ADS1293.h" 
#include "TI_ADS1293_setting.h"
#include "Uart.h"

//#define SAMPLE_ARRAY_SIZE 10                                                   // Store last 10 ecg samples for processing
//#define CH_DATA_SIZE 6                                                         // 6 bytes: ch1 ecg + ch2 ecg
//uint8_t adc_sample_array[9];

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
    BCSCTL2 |= SELM_2+SELS+DIVS_0;  //MCLKΪ8MHz��SMCLKΪ8MHz
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

int main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  
  uint8_t count=3;
  uint8_t read_buf[3];
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //���ڲ����У��յ����ֽ�
  // ����ģʽ��������4000000,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
  SpiMasterInit(4000000,8,3,1);
  UartInit(115200,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;  
  P2DIR = 0xfa;
  P2OUT = 0xf0;
  Delays(10000);
  TI_ADS1293_WriteRegSettings();  
  P2OUT = 0xb0;
  while(1)                    //���ڲ���
  {
      chr = UartReadChar();
      switch(chr)
      {
      case 'S': // start conversation
        UartWriteStr(str1);
        TI_ADS1293_SPIWriteReg(0x00, 0x01);
        UartWriteChar(q);
        break;
      case 'T': // stop conversation
        //UartWriteStr(str2);
        TI_ADS1293_SPIWriteReg(0x00, 0x00);
        //UartWriteChar(q);
        break;
      case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        {
            TI_ADS1293_SPIStreamReadReg(read_buf, count);           
            UartWriteint(read_buf[0]);
            UartWriteint(read_buf[1]);
            UartWriteint(read_buf[2]);
            UartWriteChar(0x0d);    //����"����"(\r)"
            UartWriteChar(0x0a);    //����"�س�"(\n)"  
            Delays(10);
        }
            break;
      case 'N':
        for(uint8_t j = 0x00; j<0xff; j++)
        {
          uint8_t adsreg = TI_ADS1293_SPIReadReg(j);
          UartWriteint(adsreg);
        }
        break;
      case 'D':
        TI_ADS1293_SPIWriteReg(TI_ADS1293_FLEX_CH1_CN_REG, 
                          TI_ADS1293_FLEX_CH1_CN_REG_VALUE);
        uint8_t adsreg = TI_ADS1293_SPIReadReg(TI_ADS1293_FLEX_CH1_CN_REG);
        UartWriteint(adsreg);
        break;
      }
      
  }

}

