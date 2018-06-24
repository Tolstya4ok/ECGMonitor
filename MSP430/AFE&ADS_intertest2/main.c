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
#include<msp430.h>  

int count = 0;
void select_xt2(void){  
      
    unsigned char i;  
      
    /*------ѡ��ϵͳ��ʱ��Ϊ8MHz-------*/  
    BCSCTL1 &= ~XT2OFF;                 //��XT2��Ƶ�����񵴇�      
    do{  
        IFG1 &= ~OFIFG;                 //�������ʧ�ܱ�־  
        for (i = 0xFF; i > 0; i--);     //�ȴ�8MHz��������  
    }while ((IFG1 & OFIFG));             //����ʧЧ��־��Ȼ���ڣ�    
    BCSCTL2 |= SELM_2 + SELS;           //MCLK��SMCLKѡ���Ƶ����    
  
}  
  
void main(){  
    WDTCTL = WDTPW + WDTHOLD;  
    select_xt2();  
      
    P2DIR = 0xfa;  
    P2OUT = 0xff;    
    CCTL1 = CCIE;                            // CCR1 �ж�ʹ��  
    CCR1 = 10000;  
    TACTL = TASSEL_2 + MC_2 + ID_3;          // SMCLK = 1MHz, ��������ģʽ  
    _EINT();                                //��ȫ���ж�  
      
    while(1){  
          
        if(count == 50){  
            count = 0;  
            P2OUT ^= BIT3;                   //led��ÿ0.5s�仯һ��  
        }  
          
    }  
  
}
#pragma vector=TIMERA1_VECTOR  
__interrupt void Timer_A1(void)  
{  
    switch(TAIV){  
        case 2:  
            CCR1 += 10000;                    //   
            count ++;  
    }  
      
      
}   



