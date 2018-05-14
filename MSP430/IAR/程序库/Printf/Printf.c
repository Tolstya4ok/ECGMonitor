/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Printf.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ��printf������ֲ
 *      ˵����  
 *          �ó������printf����ֲ��printfͬʱ�����Һ����Uart0��
 *          ��������������ʾ
 *          ʹ��ʱֻ���Printf.c��ӵ������У�Ȼ����Ҫ����printf
 *          �����Դ�����ļ��м����stdio.h�İ�����Ȼ��Ϳ�����
 *          ��printf�����ˣ����⣬Һ��������Ҫ��λ���������
 *          GotoXY��������Printf.h���ļ�������
 *          
 * ������ʶ��   ����ԭ20110729
 * 
 * �޸ı�ʶ��   ����ԭ20110820
 * �޸�������   Һ����printf��֧���˸��ڴ�����˸�֧�֣��˸��ں���
 *              putchar2Lcd��,�ַ�=\b=\x8ʱ���ã��˸���:BackSpace
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include "Lcd12864.h"
#include "Uart.h"
#include "Printf.h"

/****************************************************************************
* ��    �ƣ�putchar2Com
* ��    �ܣ���괮���豸����һ�ֽ�����(1��ASCII�ַ�)
* ��ڲ�����ch: �����͵��ַ�  
* ���ڲ������������ַ�
* ˵    ��: �˺�����putchar�������ã����printf�����ײ�����������豸��
****************************************************************************/
int putchar2Com(int ch)
{
    if (ch == '\n')           //  '\n'(�س�)��չ�� '\n''\r' (�س�+����) 
    {
        UartWriteChar('\r') ;   //0x0d ����
    }
    UartWriteChar(ch);        //�Ӵ��ڷ�������  
    return (ch);
}

/****************************************************************************
* ��    �ƣ�AddNewline
* ��    �ܣ�Һ��д�������һ�У������������ƶ�
* ��ڲ�������
* ���ڲ�������
* ˵    ��: �˺�����putchar2Lcd�������ã��������Һ�����Զ����Ϲ�����
****************************************************************************/
void AddNewline()
{
    char str[17];
    str[16] = 0;
    
    //�ڶ��� ������һ��
    LcdWriteComm(0x90);
    LcdReadData();              //�ն�ȡ
    for(int i = 0;i<16;i++)
    {
        str[i] = LcdReadData();
    }
    LcdWriteString(0x80,str);
    
    //������ �����ڶ���
    LcdWriteComm(0x88);
    LcdReadData();
    for(int i = 0;i<16;i++)
    {
        str[i] = LcdReadData();
    }
    LcdWriteString(0x90,str);
    
    //������ ����������
    LcdWriteComm(0x98);
    LcdReadData();
    for(int i = 0;i<16;i++)
    {
        str[i] = LcdReadData();
    }
    LcdWriteString(0x88,str);
    
    //������ �հ�
    LcdWriteString(0x98,"                ");    //ʮ�����ո�
}

/****************************************************************************
* ��    �ƣ�ChangeNextRow
* ��    �ܣ�Һ����ʾ����һ��
* ��ڲ�������
* ���ڲ�������
* ˵    ��: �˺�����putchar2Lcd�������ã���Һ��һ��д���������������з��󣬵�
            �ô˺�����д����һ�С�
****************************************************************************/
void ChangeNextRow()
{
    char addr;
    
    addr = LcdReadAddr();       //��ǰ��ַ
    if(addr <= 0x88)
    {
        LcdWriteComm(0x90);
    }
    else if(addr <= 0x90)
    {
        LcdWriteComm(0x98);
    }
    else if(addr <= 0x98)
    {
        LcdWriteComm(0x88);
    }
    else
    {
        AddNewline();           //����У�ͬʱ���Ϲ���
        LcdWriteComm(0x98);
    }
}

/****************************************************************************
* ��    �ƣ�BackSpace
* ��    �ܣ�Һ����ʾ���˸񣬵�ַָ��ǰһ�����˰��֣�
* ��ڲ�������
* ���ڲ�������
* ˵    ��: �˺�����putchar2Lcd�������ã���Һ���յ��˸��ַ����󣬵��ô˺�����
            ��ַ�˻�һ��(Ӣ�ģ����֣����ģ�һ��)
****************************************************************************/
void BackSpace()
{
    char addr,dat;
    
    addr = LcdReadAddr();       //��ǰ��ַ
    LcdWriteData(0x20);         //д��һ�����ַ������ݵ�ַ�ж��Ƿ�Ϊǰ����
    if(addr == LcdReadAddr())   //ǰ����
    {
        if(addr == 0x80)
            return;
        else if(addr == 0x90)
            addr = 0x87;
        else if(addr == 0x88)
            addr = 0x97;
        else if(addr == 0x98)
            addr = 0x8F;
        else 
            addr = addr - 1;
       
        LcdWriteComm(addr);
        LcdReadData();          //�ն�ȡ
        dat = LcdReadData();
        LcdWriteComm(addr);
        if(dat < 0x80)
            LcdWriteData(dat);
    }
    else
    {
        LcdWriteComm(addr);
    }
}

/****************************************************************************
* ��    �ƣ�putchar2Lcd
* ��    �ܣ����12864Һ���豸����һ�ֽ�����(1��ASCII�ַ�)
* ��ڲ�����ch: �����͵��ַ�  
* ���ڲ������������ַ�
* ˵    ��: �˺�����putchar�������ã����printf�����ײ������Һ����
****************************************************************************/
int putchar2Lcd(int ch)
{
    char addr,dat;
    char changeRowFlag = 0;
    
    if (ch == '\n')         //  '\n'(�س�),����
    {
        ChangeNextRow();
        changeRowFlag = 1;
    }
    else if (ch == '\b')    // '\b' (�˸�)
    {
        BackSpace();
    }
    else
    {
        addr = LcdReadAddr();
        if(ch < 0x80)
        {
            LcdWriteData(ch);
        }
        else
        {
            LcdWriteData(0x20);     //д��һ�����ַ������ݵ�ַ�ж��Ƿ�Ϊǰ����
            if(addr == LcdReadAddr())   //ǰ���� ����д��ch�ַ�
            {
                LcdWriteComm(addr);
                LcdWriteData(ch);
            }
            else
            {
                LcdWriteComm(addr);
                dat = LcdReadData();
                if(dat < 0x80)           //ǰһ���ַ���Ӣ���ַ�
                {
                    LcdWriteData(0x20);                 //�ո�
                }
                if((addr != LcdReadAddr()) &&               //д����������λ�ĺ��������
                   (addr==0x87 || addr==0x97 || addr==0x8F || addr==0x9F))
                {
                    ChangeNextRow();
                    changeRowFlag = 1;
                }
                LcdWriteData(ch);
            }
        }
    }
    if((addr != LcdReadAddr()) &&   //д����������λ�ĺ��������,��δ������
       (changeRowFlag == 0) &&   
       (addr==0x87 || addr==0x97 || addr==0x8F || addr==0x9F))
    {
        ChangeNextRow();
    }
    return (ch);
}

/****************************************************************************
* ��    �ƣ�putchar
* ��    �ܣ����׼�ն��豸����һ�ֽ�����(1��ASCII�ַ�)
* ��ڲ�����ch: �����͵��ַ�  
* ���ڲ������������ַ�
* ˵    ��: printf��������øú�����Ϊ�ײ����������Ӵ�������ַ���PC���ĳ�
            ���ն�����ϣ�printf�Ľ������ӡ�������ն��ϡ����޸ĸú���������
            ����������ʽ���������ʾ��LCD�ϣ�printf�Ľ������ʾ��LCD�ϡ�����
            ���ַ�ͬʱ����������ն˺�LCD�ϣ���������printfʱ����ͬʱ�����
            �����ն˺�LCD�ϡ�
****************************************************************************/
int putchar(int ch)
{
    putchar2Com(ch);
    putchar2Lcd(ch);
    return (ch);
}

/****************************************************************************
* ��    �ƣ�GotoXY
* ��    �ܣ���λ��Һ���ľ���λ�ã���x��y����0-15,0-3��
* ��ڲ�������
* ���ڲ�������
* ˵    ��: ����printf֮ǰ���ô˺��������Զ�λ��Һ���ľ���λ�ã���������ż��
            ��Ч�������������Զ�����һλ
****************************************************************************/
void GotoXY(char x,char y)
{
    char addr;
    
    if(y==0)
    {
        addr = 0x80 + x / 2;
    }
    else if(y==1)
    {
        addr = 0x90 + x / 2;
    }
    else if(y==2)
    {
        addr = 0x88 + x / 2;
    }
    else
    {
        addr = 0x98 + x / 2;
    }
    LcdWriteComm(addr);
    if(x % 2)                   //������������һλ��д��ո�
    {
        LcdWriteData(0x20);
    }
}