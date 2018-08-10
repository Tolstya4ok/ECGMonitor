/*--------------------------------------------------------------
 * Copyright (C) 2018 ������ͨ��ѧ ����ѧԺ �����
 * ��Ȩ���С� 
 * 
 * �ļ����� main.c
 * 
 * �ļ�����������   
 *          MSP430F149����ADS1293��AFE4400�ɼ��źŲ�ͨ���������͸�
 *          �ֻ���
 *      ˵����  
 *          ���к��ȹرտ��Ź��ͳ�ʼ��ʱ��ϵͳ��Ȼ���ʼ��ADS1293
 *          ��AFE4400��Ȼ��ͨ�����ڶ�ȡָ����շ����ݡ�������ͨ��
 *          ��ʱ���жϽ��ж�ʱ������ÿ�ν��ɼ����������ĵ��һ��Ѫ��
 *          �ź�ѹ��������͸��ֻ�
 *   
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
//uint32_t val=1;
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

/*******************************************
�������ƣ�readecg1
��    �ܣ���ȡ��һ��ecg�źŲ�ѹ��
��    ����
����ֵ  ����
********************************************/
void readecg1(uint8_t *buffer, uint8_t *buffer1, uint8_t count)
{
    TI_ADS1293_SPIStreamReadReg(buffer, count);
    *(buffer1) = *(buffer) << 4;
    *(buffer1) = *(buffer1) | ((*( buffer+1 )>>4 ) & 0x0f);
    *(buffer1+1) = *( buffer+1 ) << 4;
    *(buffer1+1) = *(buffer1+1) | ((*( buffer+2 ) >> 4 )&0x0c );
}

/*******************************************
�������ƣ�readecg2
��    �ܣ���ȡ�ڶ���ecg�źŲ�ѹ��
��    ����
����ֵ  ����
********************************************/
void readecg2(uint8_t *buffer, uint8_t *buffer1, uint8_t count)
{
    TI_ADS1293_SPIStreamReadReg(buffer, count);
    *(buffer1+1) = *(buffer1+1) | (( *(buffer) >> 2 )&0x03 );
    *(buffer1+2) = *(buffer) << 6;
    *(buffer1+2) = *(buffer1+2) | (( *( buffer+1 )>>2 ) & 0x3f);
    *(buffer1+3) = *( buffer+1 ) << 6;
    *(buffer1+3) = *(buffer1+3) | (( *( buffer+2 ) >> 2 )&0x30 );
}

/*******************************************
�������ƣ�readecg3
��    �ܣ���ȡ������ecg�źŲ�ѹ��
��    ����
����ֵ  ����
********************************************/
void readecg3(uint8_t *buffer, uint8_t *buffer1, uint8_t count)
{
    TI_ADS1293_SPIStreamReadReg(buffer, count);
    *(buffer1+3) = *(buffer1+3) | ( *(buffer)&0x0f );
    *(buffer1+4) = *( buffer+1 );
    *(buffer1+5) = *( buffer+2 ) & 0xc0;
}

/*******************************************
�������ƣ�readspo2
��    �ܣ���ȡ��һ��spo2�źŲ�ѹ��
��    ����
����ֵ  ����
********************************************/
void readspo2(uint8_t *buffer1, uint8_t count)
{
    unsigned long value = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
    *(buffer1+5) = *(buffer1+5) | ((value>>15) & 0x3F);
    *(buffer1+6) = (value>>7) & 0xF8;
    value = TI_AFE4400_SPIAutoIncReadReg(LED2VAL, count);
    *(buffer1+6) = *(buffer1+6) | (( value>>19 ) & 0x07);
    *(buffer1+7) = (value>>11) & 0xFF;
}

/****************************************************************************
* ��    �ƣ�main������
* ��    �ܣ����ô��ڣ������Ϣ���Ӵ��ڶ�����������������ݣ����Դ����շ�
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
int main( void )
{
  

  /*�������г���ر����е�IO��*/
  P1DIR = 0XFF;    P1OUT = 0X00;
  P2DIR = 0XFA;    P2OUT = 0XF0;
  P3DIR = 0XFF;    P3OUT = 0X00;
  P4DIR = 0x00;    P4OUT = 0x00;
  P4DIR |= 0x80;   P4OUT |= 0x80;
  P5DIR = 0XFF;    P5OUT = 0X00;
  P6DIR = 0XFF;    P6OUT = 0X00;
  
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  ClkInit();
  CCTL1 = CCIE;                            // CCR1 �ж�ʹ��  
  CCR1 = 6667;
  //CCR1 = 50000;  
  TACTL = TASSEL_2 + MC_2 + ID_3;          // SMCLK = 1MHz, ��������ģʽ 
  uint8_t count=3;

  uint8_t read_buf[3];
  uint8_t read_buff[8] = {0,0,0,0,0,0,0,0};
  
  //unsigned long val;
  
  char exit_state_flag = 0;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //���ڲ����У��յ����ֽ�
  // ����ģʽ��������4000000,8λ����λ������ģʽ��ʱ��ģʽ1�������spi.c��
  SpiMasterInit(4000000,8,3,0);
  UartInit(115200,'n',8,1);//���ڳ�ʼ��,���ó�38400bps,��У��,8λ����,1λֹͣ
  _EINT(); 
  
  //P1DIR = 0xff;
  //P1SEL = 0x00;
  Delays(10000);
  TI_ADS1293_WriteRegSettings();
  TI_AFE4400_WriteRegSettings(); 
  
  chr = UartReadChar();
  
  while(1)                    //���ڲ���
  {
      
    TI_ADS1293_SPIWriteReg(0x00, 0x01);
    TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read AFE (0x000001ul)
        
    switch(chr)
      {
      case 'T':
        P2OUT &= (~BIT5);
        P2OUT &= (~BIT6);
        exit_state_flag=0;
        while(exit_state_flag==0)
        {
          if(UartReadState()==1)
          {
            exit_state_flag=1;
            chr = UartForceReadChar();
          }
          P2OUT ^= BIT7;
          UartWriteint(0x00);
          UartWriteint(0x01);
          UartWriteint(0x02);
          UartWriteint(0x03);
          UartWriteint(0x04);
          UartWriteint(0x05);
          UartWriteint(0x06);
          UartWriteint(0x07);
          Delays(1);
          _BIS_SR(CPUOFF);
          _NOP();
        }
        break;
      case 'M': //moblie recieving mode
        exit_state_flag=0;
        while(exit_state_flag==0)
        {
          if(UartReadState()==1)
          {
            exit_state_flag=1;
            chr = UartForceReadChar();
          }
          if(c==1)
          {
            readecg1(read_buf, read_buff, count);
            P2OUT ^= BIT6; 
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else if(c==2)
          {
            readecg2(read_buf, read_buff, count);
            P2OUT ^= BIT6;
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else
          {
            c = 0;
            readecg3(read_buf, read_buff, count);
            P2OUT ^= BIT6;
            readspo2(read_buff, count);
            P2OUT ^= BIT5;
            UartWriteint(read_buff[0]);
            UartWriteint(read_buff[1]);
            UartWriteint(read_buff[2]);
            UartWriteint(read_buff[3]);
            UartWriteint(read_buff[4]);
            UartWriteint(read_buff[5]);
            UartWriteint(read_buff[6]);
            UartWriteint(read_buff[7]);
            Delays(1);
            // FOR PROCESSING, WE NEED 0D,OA
            //UartWriteChar(0x0d);
            //UartWriteChar(0x0a);
            _BIS_SR(CPUOFF);
            _NOP();
          }
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
            //CCR1 += 20000;                    
            CCR1 += 6667;
            c = c + 1;
            __bic_SR_register_on_exit(CPUOFF);            
    }      
}  

