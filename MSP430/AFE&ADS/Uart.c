/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Uart.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ������ͨѶ������(�򻯰�)
 *      ˵����  ����������ɴ��ڵĳ�ʼ���������ַ����ַ���������
 *              ʹ�ַ������÷�������Uart.c���뵽��Ŀ��ͬʱ��Ҫ����
 *              �˳����ĳ����ļ��м������Uart.h����䣻��������
 *              ����ʹ��ͷ�ļ��������ĳ����ˣ���ϸ�ɲο� ������ ��
 *               main.c
 *              �� ��Ҫ��ΪUART1ģ�飬ֻ��Ѻ궨���x ��Ӧ��0��Ϊ1
 *              ���ɣ����Ҫͬʱʹ��0��1����ģ�飬����Ҫ�Ѻ�������
 *              �ָ�Ϊ��ͬ�������ļ��ֱ����0��1����ģ��
 * 		
 * ������ʶ��   ����ԭ20110622
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/
#include <stdint.h>
#include <msp430x14x.h>
#include "Uart.h"

/***********************************�궨��*********************************/
#define UxCTL   U0CTL
#define UxRCTL  U0RCTL
#define UxTCTL  U0TCTL

#define UxBR0   U0BR0
#define UxBR1   U0BR1
#define UxMCTL  U0MCTL

#define UxRXBUF U0RXBUF
#define UxTXBUF U0TXBUF

#define UxME    U0ME
#define UxIE    U0IE
#define UxIFG   U0IFG

#define UTXEx   UTXE0
#define URXEx   URXE0

#define URXIEx  URXIE0
#define UTXIEx  UTXIE0

#define UARTxTX_VECTOR  UART0TX_VECTOR
#define UARTxRX_VECTOR  UART0RX_VECTOR

#define UARTON  P3SEL |= 0X30           // P3.4,5 = USART0 TXD/RXD
/**************************************************************************/

char TxFlagu=1;
char RxFlagu=0;


/****************************************************************************
* ��    �ƣ�abs
* ��    �ܣ������ֵ
* ��ڲ�����i:����
* ���ڲ�����int ��õľ���ֵ
****************************************************************************/
int abs(int i)
{
    return (i>=0?i:(-i));
}

/***************************************************************************
* ��    �ƣ�SetBaudRateRegisters
* ��    �ܣ�����ʱ�� ���������ö�Ӧ�Ĵ���
* ��ڲ�����
*           clk:        ��ѡʱ��Ƶ�ʣ��磺32768��
*           baud:       ������      (300~115200)
* ���ڲ�������
* ��    ��: SetBaudRateRegisters(32768,9600)//��ʱ��Ƶ��32768����9600�Ĳ�����
****************************************************************************/
void SetBaudRateRegisters(long clk,long baud)
{
    int n = clk / baud;     //����������
    char mSum = 0;          //��mi
    int txEr0;              //��ӦλΪ0ʱ������
    int txEr1;              //��ӦλΪ1ʱ������
    char i = 0;             //ѭ������
    
    UxBR1 = n >> 8;         //��8λ
    UxBR0 = n & 0xff;       //��8λ
    UxMCTL = 0;
    
    //ѭ�� �Ƚϴ����ʴ�С ����UxMCTL
    for(;i < 8;i++)
    {
        txEr0 = 100 * baud * ((i + 1) * n + mSum) / clk - 100 * (i + 1);
        txEr1 = 100 * baud * ((i + 1) * n + mSum + 1) / clk - 100 * (i + 1);
        if(abs(txEr1) < abs(txEr0))
        {
            mSum++;
            UxMCTL |= (1<<i);
        }
    }
}

/****************************************************************************
* ��    �ƣ�UartInit()
* ��    �ܣ���ʼ�����ڡ����ò����ʵ�
* ��ڲ�����
*           baud:       ������      (300~115200)
            parity:     ��żУ��λ  ('n'=��У��  'p'=żУ��  'o'=��У��)
            datsBits:   ����λλ��  (7��8)
            stopBits:   ֹͣλλ��  (1��2)
* ���ڲ���������ֵΪ1ʱ������ɹ���Ϊ0��ʾ�����д���
* ��    ��: UartInit(9600,'n',8,1) //���9600bps����У�飬8λ���ݣ�1λֹͣλ
            UartInit(2400,'p',7,2) //���2400bps��żУ�飬7λ���ݣ�2λֹͣλ
****************************************************************************/
char UartInit(long baud,char parity,char dataBits,char stopBits)
{
    long int brclk;                 //�����ʷ�����ʱ��Ƶ��
    
    UxCTL |= SWRST;                 //��ʼ
    
    //ʱ��Դ����
    UxTCTL &=~ (SSEL0+SSEL1);       //���֮ǰ��ʱ������
    if(baud<=9600)                  //
    {
      UxTCTL |= SSEL0;              //ACLK�����͹���
      brclk = 32768;                //�����ʷ�����ʱ��Ƶ��=ACLK(32768)
    }
    else
    {
      UxTCTL |= SSEL1;              //SMCLK����֤�ٶ�
      brclk = 8000000;              //�����ʷ�����ʱ��Ƶ��=SMCLK(1MHz)
    }
    
    //------------------------���ò�����-------------------------   
    if(baud < 300||baud > 115200)   //�����ʳ�����Χ
    {
        return 0;
    }
    SetBaudRateRegisters(brclk,baud);         //���ò����ʼĴ���
  
    //------------------------����У��λ-------------------------  
    switch(parity)
    {
        case 'n':case'N': UxCTL &=~ PENA;               break;  //��У��
        case 'p':case'P': UxCTL |= PENA + PEV;          break;  //żУ��
        case 'o':case'O': UxCTL |= PENA; UxCTL &=~ PEV; break;  //��У��  
        default :         return(0);                            //��������
    }
    
    //------------------------��������λ-------------------------    
    switch(dataBits)
    {
        case 7:case'7': UxCTL &=~ CHAR; break;      //7λ����
        case 8:case'8': UxCTL |= CHAR;  break;      //8λ����
        default :       return(0);                  //��������
    } 
    //------------------------����ֹͣλ-------------------------    
    switch(stopBits)
    {
        case 1:case'1': UxCTL &= ~SPB;  break;      //1λֹͣλ
        case 2:case'2': UxCTL |= SPB;   break;      //2λֹͣλ
        default :       return(0);                  //��������
    }
    
    UARTON;                     //�˿�ʹ��
    UxME |= UTXEx + URXEx;      //���� ����ʹ��
    
    UCTL0 &= ~SWRST;            // Initialize USART state machine
    
    UxIE |= URXIEx + UTXIEx;    // Enable USART0 RX interrupt 
    
    return(1);                  //���óɹ�
}

/****************************************************************************
* ��    �ƣ�UartLpm
* ��    �ܣ�������/���ȴ������У���ʱ��ϵͳ�Ĳ���ʱ�Ӹùص�������ʡ��
* ��ڲ�������
* ���ڲ�������
* ˵    ��: �������������ʱ�ӳ�ͻ����ע�͵��ú������������ӹ��ġ�
****************************************************************************/
void UartLpm()
{
    if(UxTCTL&SSEL0) LPM3;  //����ACLK ��ʱ�ӣ�����LPM3����(����ACLK)
    else             LPM0;  //����SMCLK��ʱ�ӣ�����LPM0����(���ر�SMCLK)
}

/****************************************************************************
* ��    �ƣ�UartWriteChar
* ��    �ܣ��򴮿�дһ���ֽ�����
* ��ڲ�����c:  �����͵�һ�ֽ�����
* ���ڲ�������
* ˵    ��: �ڵȴ�������ϵĹ����У�CPU������
****************************************************************************/
void UartWriteChar(char c)
{ 
    while (TxFlagu==0) UartLpm();  // �ȴ���һ�ֽڷ��꣬������
    TxFlagu=0;                     //
    UxTXBUF=c;
}

void UartWriteint(uint8_t c)
{ 
  while (TxFlagu==0) UartLpm();  // �ȴ���һ�ֽڷ��꣬������
    TxFlagu=0;                     //
    UxTXBUF=c;
}
/****************************************************************************
* ��    �ƣ�UartReadChar
* ��    �ܣ��Ӵ��ڶ�ȡ1�ֽ�����
* ��ڲ�������  
* ���ڲ������յ���һ�ֽ�����
* ˵    ��: �������û�����ݣ���һֱ�ȴ����ȴ������У�CPU������
****************************************************************************/
char UartReadChar()
{ 
    while (RxFlagu==0) UartLpm(); // �յ�һ�ֽ�?
    RxFlagu=0;
    return(UxRXBUF);
}

uint8_t UartReadint()
{ 
    while (RxFlagu==0) UartLpm(); // �յ�һ�ֽ�?
    RxFlagu=0;
    return(UxRXBUF);
}
/****************************************************************************
* ��    �ƣ�UartWriteStr
* ��    �ܣ��򴮿�дһ���ַ���
* ��ڲ�����s��Ҫд����ַ���  
* ���ڲ�������
* ˵    ��: ���û�е� NULL ����д���ַ�
****************************************************************************/
void UartWriteStr(char *s)
{
    while(*s)
    {
        UartWriteChar(*s++);
    }
}

void UartWritearray(uint8_t *s)
{
    while(*s)
    {
        UartWriteint(*s++);
    }
}
/****************************************************************************
* ��    �ƣ�UartRx
* ��    �ܣ����ڽ����жϣ�ÿ���յ�1�ֽڻᷢ��һ���ж�
****************************************************************************/
#pragma vector=UARTxRX_VECTOR
__interrupt void UartRx()
{
    RxFlagu=1;
    __low_power_mode_off_on_exit();
}

/****************************************************************************
* ��    �ƣ�UartTx
* ��    �ܣ����ڷ����жϣ�ÿ����1�ֽڻᷢ��һ���ж�
****************************************************************************/
#pragma vector=UARTxTX_VECTOR
__interrupt void UartTx ()
{
    TxFlagu=1;
    __low_power_mode_off_on_exit();
}
