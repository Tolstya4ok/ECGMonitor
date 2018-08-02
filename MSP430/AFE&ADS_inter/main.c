/*----------------------------------------------------------------
 * Copyright (C) 2018 西安交通大学 李金铭
 * 版权所有。 
 * 
 * 文件名： main.c
 * 
 * 文件功能描述：   
 *          MSP430F16x,MSP430F14X系列单片机串口通讯函数库示例程序
 *      说明：  
 *          该程序示范串口通讯函数库内的各函数使用方法，同时可作为
 *          串口测试程序用。该程序的演示需要通过Windows自带的"超级
 *          终端"软件，或是串口调试器进行。
 *
 *          运行后，先输出两个字符串  
 *          UartWriteChar 函数工作正常!
 *          下面测试串口收发函数
 *          然后，将按原字符回复，串口调试器发送什么就接收什么
 *
 * 创建标识：   刘中原20110622
 * 
 * 修改标识：   
 * 修改描述：   
**----------------------------------------------------------------*/
#include <stdint.h>
#include "msp430x14x.h"   //430寄存器头文件
#include "Uart.h"         //串口通讯程序库头文件
#include "Spi.h"
#include "MSP430_AFE_SPI.h"
#include "TI_AFE4400.h"
#include "TI_AFE4400_setting.h"
#include "MSP430_Spi.h"
#include "TI_ADS1293.h"
#include "TI_ADS1293_setting.h"

uint8_t c=1;
char str[] = "UartWriteChar"; //ROM中一个字符串
char str1[] = "start conversation!"; //ROM中一个字符串
char str2[] = "stop conversation!"; //ROM中一个字符串
char q = 'Q';

/****************************************************************************
* 名    称：ClkInit
* 功    能：时钟系统初始化  MCLK为8MHz，SMCLK为1MHz
* 入口参数：无
* 出口参数：无
****************************************************************************/
void ClkInit()
{
    char i;
    BCSCTL1 &= ~XT2OFF;             //打开XT2振荡器
    IFG1&=~OFIFG;                   //清除振荡错误标志
    while((IFG1&OFIFG)!=0)
    {
        for(i=0;i<0xff;i++);
        IFG1&=~OFIFG;               //清除振荡错误标志
    }
    BCSCTL2 |= SELM_2+SELS+DIVS_0;  //MCLK为8MHz，SMCLK为1MHz
}

/*******************************************
函数名称：Delays
功    能：延时一会
参    数：无
返回值  ：无
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
* 名    称：main主程序
* 功    能：设置串口，输出信息，从串口读计算机键盘输入数据，测试串口收发
* 入口参数：无
* 出口参数：无
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
  CCTL1 = CCIE;                            // CCR1 中断使能  
  CCR1 = 10000;  
  TACTL = TASSEL_2 + MC_2 + ID_3;          // SMCLK = 1MHz, 连续计数模式 
  uint8_t count=3;

  uint8_t read_buf[3];
  uint8_t read_buff[8] = {0,0,0,0,0,0,0,0};
  
  unsigned long val;
 // int i = 0;

//  count = CH_DATA_SIZE;                                                        // bytes to read: ADC_DOUT2 - ADCDOUT0

  
  char chr;               //串口测试中，收到的字节
  // 主机模式，波特率4000000,8位数据位，三线模式，时钟模式1（具体见spi.c）
  SpiMasterInit(4000000,8,3,1);
  UartInit(115200,'n',8,1);//串口初始化,设置成38400bps,无校验,8位数据,1位停止
  _EINT(); 
  
  P1DIR = 0xff;
  P1SEL = 0x00;
  Delays(10000);
  TI_ADS1293_WriteRegSettings();
  Delays(1);
  TI_AFE4400_WriteRegSettings();  
  while(1)                    //串口测试
  {
    

    chr = UartReadChar();
    
    switch(chr)
      {
      case 'S':
        //UartWriteStr(str1);
        TI_ADS1293_SPIWriteReg(0x00, 0x01);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read 0x000001ul
        //P2OUT = 0x90;
        //UartWriteChar(q);
        break;
      case 'T':
        //UartWriteStr(str2);
        TI_ADS1293_SPIWriteReg(0x00, 0x00);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 0, 3);//enable read
        //UartWriteChar(q);
        break;
      case 'P':
        TI_ADS1293_SPIWriteReg(0x00, 0x01);
        Delays(1);
        TI_AFE4400_SPIAutoIncWriteReg(0x00, 1, 3); //enable read 0x000001ul
        Delays(1);
        for(uint32_t k = 0;k<1000000;k++)
        //while(1)
        {
          P2OUT ^= BIT3;
          //UartWriteChar('C');
          //UartWriteint(c);
          //UartWriteChar('K');
          //UartWriteint(k);
          if(c==1)
          {
            //P2OUT = 0xf0;
            P2OUT ^= BIT6;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[0] = read_buf[0] << 4;
            read_buff[0] = read_buff[0] | (( read_buf[1]>>4 ) & 0x3f);
            read_buff[1] = read_buf[1] << 4;
            read_buff[1] = read_buff[1] | (( read_buf[2] >> 4 )&0x0c );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else if(c==2)
          {
            //P2OUT = 0xff;
            P2OUT ^= BIT6;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[1] = read_buff[1] | (( read_buf[0] >> 2 )&0x03 );
            read_buff[2] = read_buf[0] << 6;
            read_buff[2] = read_buff[2] | (( read_buf[1]>>2 ) & 0x3f);
            read_buff[3] = read_buf[1] << 6;
            read_buff[3] = read_buff[3] | (( read_buf[2] >> 2 )&0x30 );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else
          {
            c = 0;
            //P2OUT = 0xf0;
            P2OUT ^= BIT6;
            P2OUT ^= BIT5;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[3] = read_buff[3] | ( read_buf[0]&0x0f );
            read_buff[4] = read_buf[1];
            read_buff[5] = read_buf[2] & 0xc0;
            val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            read_buff[5] = read_buff[5] | ((val>>15) & 0x3F);
            read_buff[6] = (val>>7) & 0xF8;
            val = TI_AFE4400_SPIAutoIncReadReg(LED2VAL, count);
            //P2OUT ^= BIT5;
            read_buff[6] = read_buff[6] | (( val>>19 ) & 0x07);
            read_buff[7] = (val>>11) & 0xFF;
            //Delays(10);
            UartWriteint(read_buff[0]);
            UartWriteint(read_buff[1]);
            UartWriteint(read_buff[2]);
            UartWriteint(read_buff[3]);
            UartWriteint(read_buff[4]);
            UartWriteint(read_buff[5]);
            UartWriteint(read_buff[6]);
            UartWriteint(read_buff[7]);
            // FOR PROCESSING, WE NEED 0D,OA
            Delays(1);
            _BIS_SR(CPUOFF);
            _NOP();
          }
        }
        break;
      case 'M':
        for(uint32_t k = 0;k<1000000;k++)
        //while(1)
        {
          P2OUT ^= BIT3;
          //UartWriteChar('C');
          //UartWriteint(c);
          //UartWriteChar('K');
          //UartWriteint(k);
          if(c==1)
          {
            //P2OUT = 0xf0;
            P2OUT ^= BIT6;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[0] = read_buf[0] << 4;
            read_buff[0] = read_buff[0] | (( read_buf[1]>>4 ) & 0x3f);
            read_buff[1] = read_buf[1] << 4;
            read_buff[1] = read_buff[1] | (( read_buf[2] >> 4 )&0x0c );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else if(c==2)
          {
            //P2OUT = 0xff;
            P2OUT ^= BIT6;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[1] = read_buff[1] | (( read_buf[0] >> 2 )&0x03 );
            read_buff[2] = read_buf[0] << 6;
            read_buff[2] = read_buff[2] | (( read_buf[1]>>2 ) & 0x3f);
            read_buff[3] = read_buf[1] << 6;
            read_buff[3] = read_buff[3] | (( read_buf[2] >> 2 )&0x30 );
            _BIS_SR(CPUOFF);
            _NOP();
          }
          else
          {
            c = 0;
            //P2OUT = 0xf0;
            P2OUT ^= BIT6;
            P2OUT ^= BIT5;
            TI_ADS1293_SPIStreamReadReg(read_buf, count); 
            read_buff[3] = read_buff[3] | ( read_buf[0]&0x0f );
            read_buff[4] = read_buf[1];
            read_buff[5] = read_buf[2] & 0xc0;
            val = TI_AFE4400_SPIAutoIncReadReg(LED1VAL, count);
            read_buff[5] = read_buff[5] | ((val>>15) & 0x3F);
            read_buff[6] = (val>>7) & 0xF8;
            val = TI_AFE4400_SPIAutoIncReadReg(LED2VAL, count);
            //P2OUT ^= BIT5;
            read_buff[6] = read_buff[6] | (( val>>19 ) & 0x07);
            read_buff[7] = (val>>11) & 0xFF;
            //Delays(10);
            UartWriteint(read_buff[0]);
            UartWriteint(read_buff[1]);
            UartWriteint(read_buff[2]);
            UartWriteint(read_buff[3]);
            UartWriteint(read_buff[4]);
            UartWriteint(read_buff[5]);
            UartWriteint(read_buff[6]);
            UartWriteint(read_buff[7]);
            // FOR PROCESSING, WE NEED 0D,OA
            UartWriteChar(0x0d);
            UartWriteChar(0x0a);
            Delays(1);
            _BIS_SR(CPUOFF);
            _NOP();
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
            UartWriteChar(0x0d);    //发送"换行"(\r)"
            UartWriteChar(0x0a);    //发送"回车"(\n)"  
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
            UartWriteChar(0x0d);    //发送"换行"(\r)"
            UartWriteChar(0x0a);    //发送"回车"(\n)"
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
            CCR1 += 10000;                    //   
            c = c + 1;
            __bic_SR_register_on_exit(CPUOFF);            
    }      
}  
