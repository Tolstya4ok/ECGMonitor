/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� main.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ��Flash�����ʹ����ʾ
 *      ˵����  
 *          ���Flash�������������ʹ����ʾ���۲�Flash���ݣ���
 *          ��ʱ��View-Memory���Դ�Flash�洢������,�鿴Flash
 *          �洢�������ݡ�
 *          
 * ������ʶ��   ����ԭ20110830
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include "Flash.h"

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

/****************************************************************************
* ��    �ƣ�main������
* ��    �ܣ���ʾFlash����� ��ʹ��
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
int a;
void main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();
    FlashInit();
    FlashWriteChar(InfoB,0x25);
    a=FlashReadChar (InfoB);        //InfoB��H�ļ����к궨��
    FlashWriteWord(InfoB+2,0x5669);
    a = FlashReadWord(InfoB+2);
    FlashErase(InfoB);
    LPM0;
}
