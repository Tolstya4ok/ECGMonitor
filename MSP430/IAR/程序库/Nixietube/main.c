/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� main.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ������ܳ�����ʾ
 *      ˵����  
 *          �ó�����ɵ�������ܳ�����������������ʾ�ģ����
 *          ����ܳ������ʾ;����������ܵ�printf��ʹ�á�
 *          
 *          
 * ������ʶ��   ����ԭ20110822
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include <stdio.h>
#include "Nixietube.h"

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
* ��    �ܣ�����ܳ�����ʾ
* ��ڲ�������
* ���ڲ�������
****************************************************************************/
void main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;
    ClkInit();
    NixiettubeInit();
    _EINT();
    //while(1)
    {
        NixiettubeDisplayChar(AA,5);
        NixiettubeDisplayChar(DOT,5);
        NixiettubeInsertChar(2);
        NixiettubeInsertChar(DOT);
        NixiettubeInsertChar(2);
        printf("%1.2f",1.2);
    }
}
