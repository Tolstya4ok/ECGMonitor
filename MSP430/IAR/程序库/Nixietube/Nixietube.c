/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Nixietube.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ������ܳ����
 *      ˵����  
 *          ���������������˳��õ�������ʾ����,����ʾ���֡���ĸ�ȿ�
 *          ����Ϊ���ֳ���ĵײ�����ʹ�á�
 *          ʹ��ʱֻ���Nixietube.c��ӵ������У�Ȼ����Ҫ��������ܳ�
 *          ���Դ�����ļ��м����Nixietube.h�İ�����Ȼ��Ϳ��Ե���ͷ
 *          �ļ��������ĸ���������
 *          
 * ������ʶ��   ����ԭ20110822
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include "Nixietube.h"

/*�궨�壬�����a-h���ζ�Ӧ�ı��أ�����Ӳ��ֻ�øĶ�����8��*/
#define a       0x01                            //  AAAA
#define b       0x02                            // F    B
#define c       0x04                            // F    B
#define d       0x08                            //  GGGG
#define e       0x10                            // E    C
#define f       0x20                            // E    C
#define g       0x40                            //  DDDD  HH
#define h       0x80        //С����

/*�ú궨���Զ����ɶ�����ܺõ�д����ֵ��ѧϰ*/
/*����Ӳ��������д�����*/
const char Tab[] = {
    a + b + c + d + e + f,                  // Displays "0"
    b + c,                                  // Displays "1"
    a + b + d + e + g,                      // Displays "2"
    a + b + c + d + g,                      // Displays "3"
    b + c + f + g,                          // Displays "4"
    a + c + d + f +g,                       // Displays "5"
    a + c + d + e + f + g,                  // Displays "6"
    a + b + c,                              // Displays "7"
    a + b + c + d + e + f + g,              // Displays "8"
    a + b + c + d + f + g,                  // Displays "9"
    a + b + c + e + f + g,                  // Displays "A"
    c + d + e + f + g,                      // Displays "B"  
    a + d + e + f,                          // Displays "C"
    b + c + d + e + g,                      // Displays "D"
    a + d + e + f + g,                      // Displays "E"
    a + e + f + g,                          // Displays "F"
    a + c + d + e + f,                      // Displays "G"
    b + c + e + f + g,                      // Displays "H"  
    e + f,                                  // Displays "I"
    b + c + d + e,                          // Displays "J"
    b + d + e + f + g,                      // Displays "K"
    d + e + f,                              // Displays "L"  
    a + c + e + g,                          // Displays "M" 
    a + b + c + e + f,                      // Displays "N"   
    c + e + g,                              // Displays "n"
    c + d + e + g,                          // Displays "o"
    a + b + c + d + e + f,                  // Displays "O"
    a + b + e + f + g,                      // Displays "P"
    a + b + c + f + g,                      // Displays "Q" 
    e + g,                                  // Displays "r"
    a + c + d + f +g,                       // Displays "S"
    d + e + f + g,                          // Displays "t"
    a + e + f ,                             // Displays "T" 
    b + c + d + e + f,                      // Displays "U"
    c + d + e,                              // Displays "v" 
    b + d + f + g,                          // Displays "W"
    b + c + d + f + g,                      // Displays "Y" 
    a + b + d + e + g,                      // Displays "Z"
    g,                                      // Displays "-"
    h,                                      // Displays "."  
    0                                       // Displays " "
}; 
#undef a
#undef b
#undef c
#undef d
#undef e
#undef f
#undef g 

/**********************************�궨��**************************************/
#define DATA_DIR_OUT	P5DIR|=0XFF
#define CTRL_DIR_OUT    P3DIR|=0XFF
#define DATA_OUT        P5OUT
#define CTRL_OUT        P3OUT
/******************************************************************************/

char Nixie[8] = "\0\0\0\0\0\0\0\0";     //��ʼ״̬ ����ʾ

/************************************************
* �������ƣ�NixiettubeInit
* ��    �ܣ���ʼ�������ʹ�õ������Դ
* ��    ������
* �� �� ֵ����
*************************************************/
void NixiettubeInit()
{
    WDTCTL = WDT_ADLY_1_9;  //���Ź��ڲ���ʱ��ģʽ16ms
    IE1 |= WDTIE;           //�����Ź��ж�
    CTRL_DIR_OUT;
    DATA_DIR_OUT;
}

/************************************************
* �������ƣ�NixiettubeClear
* ��    �ܣ������������ǰһ�е���ʾ(��һ��8�ַ�)
* ��    ������
* �� �� ֵ����
*************************************************/
void NixiettubeClear()
{
    for(int i = 0;i < 8;i++)
        Nixie[i] = Tab[SP];     //��ʾ�ո�
}

/************************************************
* �������ƣ�NixiettubeDisplayChar
* ��    �ܣ���ʾ�ַ�;����ʾ��ĸ�ο�Nixietube.h�е�
            �궨��
* ��    ����
            ch��Ҫ��ʾ���ַ�
            addr��λ�ã�0-7�ӵ�λ����λ(�ҵ���)
* �� �� ֵ����
* ˵    �������øú����󣬲�Ӱ������λ����ʾ��ֻ��
            ����addrָ����λ��
*************************************************/
void NixiettubeDisplayChar(char ch,char addr)
{
    if(ch == DOT)       //С����,���赥��ռһλ
    {
        Nixie[addr] |= Tab[ch];
    }
    else
    {
        Nixie[addr] = Tab[ch];
    }
}

/************************************************
* �������ƣ�NixiettubeInsertChar
* ��    �ܣ��Ҷ˲����ַ�;����ʾ��ĸ�ο�Nixietube.h
            �еĺ궨��
* ��    ����ch��Ҫ������ַ�
* �� �� ֵ����
* ˵    �������øú�������������ʾ�ַ�����һλ��
            �µ��ַ����������Ҷ�һλ��������������
            �����ݺ�����ʾ��λ�ȡ�
*************************************************/
void NixiettubeInsertChar(char ch)
{
    if(ch == DOT)       ////С����,���赥��ռһλ
    {
        Nixie[0] |= Tab[ch];
        return;
    }
    for(int i = 7;i > 0;i--)
        Nixie[i] = Nixie[i - 1];     //����ʾ�ַ�����һλ
    Nixie[0] = Tab[ch];
}

/************************************************
* �������ƣ�Display
* ��    �ܣ�ɨ����ʾ���������ַ�
* ��    ������
* �� �� ֵ���� 
* ˵    ����ÿ��ɨ����ʾһλ��ͬʱָ���ƶ����Թ���
*           һ����ʾ�á�
*************************************************/
void Display()
{
    static char i = 0;      //��¼ɨ����ʾ����λ
    CTRL_OUT = 1<<i;
    DATA_OUT = Nixie[i];
    i++;
    if(i>7)
        i = 0;
}

/************************************************
�������ƣ�WDT_ISR
��    �ܣ����Ź���ʱ�жϣ�16ms��
��    ������
����ֵ  ����
*************************************************/
#pragma vector=WDT_VECTOR
__interrupt void WDT_ISR()
{
    Display();
}