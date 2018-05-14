/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Getchar.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ������scanf��ֲ
 *      ˵����  
 *          �ó�����ɼ��̵�getchar����������ֲscanf����
 *  
 *          ʹ��ʱֻ���getchar.c��ӵ������У�Ȼ����Դ�����ļ�
 *          �м����stdio.h�İ�����Ȼ��Ϳ��Ե���scanf������
 *          
 * ������ʶ��   ����ԭ20110820
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/
#include <stdio.h>
#include "Key.h"

#define LINE_LENGTH 20          //�л�������С������ÿ�����������ַ���

/*��׼�ն��豸�У�����ASCII�붨�壬�����޸�*/
#define InBACKSP 0x08           //ASCII  <--  (�˸��)
#define InDELETE 0x7F           //ASCII <DEL> (DEL ��)
#define InEOL '\r'              //ASCII <CR>  (�س���)
#define InLF '\n'                 //ASCII <LF>  (�س�)
#define InSKIP '\3'             //ASCII control-C
#define InEOF '\x1A'            //ASCII control-Z

#define OutDELETE "\x8 \x8"     //VT100 backspace and clear
#define OutSKIP "^C\n"          //^C and new line
#define OutEOF "^Z"             //^Z and return EOF

/****************************************************************************
* ��    �ƣ�getchar
* ��    �ܣ��Ӽ����豸����һ�ֽ�����(1��ASCII�ַ�)
* ��ڲ�������
* ���ڲ������յ����ַ�
* ˵    ��: scanf��������øú�����Ϊ�ײ����롣����4*4�������밴���ַ���scanf
            ���������뼴Դ��PC�����̡����޸ĸú��������ַ���������ʽ���룬��
            ��Ƭ�����ڵȡ�
            ���������л��棬�ܹ������˸��ɾ��������������Ҫɾ����������ֱ��
            ����ReadKey()������
****************************************************************************/
int getchar()
{
    static char inBuffer[LINE_LENGTH + 2];      //Where to put chars
    static char ptr;                            //Pointer in buffer
    char c;
    
    while(1)
    {
        if(inBuffer[ptr])                       //������������ַ�
            return (inBuffer[ptr++]);           //����������ַ�
        ptr = 0;                                //ֱ��������ϣ�������ָ�����
        while(1)                                //������û���ַ�����ȴ��ַ�����
        {
            c = ReadKey();                      //�ȴ�����һ���ַ�==��ֲʱ�ؼ�
            if(c == InEOF && !ptr)              //==EOF==  Ctrl+Z 
            {                                   //ֻ����δ�������ַ�ʱ����Ч
                printf(OutEOF);                 //�ն���ʾEOF��
                return EOF;                     //���� EOF��-1��
            }
            if(c==InDELETE || c==InBACKSP)      //==�˸��ɾ����==
            {
                if(ptr)                         //��������ֵ
                {
                    ptr--;                      //�ӻ������Ƴ�һ���ַ�
                    printf(OutDELETE);          //ͬʱ��ʾҲɾ��һ���ַ�
                }
            }
            else if(c == InSKIP)                //==ȡ���� Ctrl+C ==
            {
                printf(OutSKIP);                //�ն���ʾ������һ��
                ptr = LINE_LENGTH + 1;          //==0 ������==
                break;
            }
            else if(c == InEOL||c == InLF)      //== '\r' �س�=='\n'�س�
            {
                putchar(inBuffer[ptr++] = '\n');//�ն˻���
                inBuffer[ptr] = 0;              //ĩβ��ӽ�������NULL��
                ptr = 0;                        //ָ�����
                break;
            }
            else if(ptr < LINE_LENGTH)          //== �����ַ� ==
            {
                if(c >= ' ')                    //ɾ�� 0x20�����ַ�
                {
                    //���뻺����
                    putchar(inBuffer[ptr++] = c);
                }
            }
            else                                //����������
            {
                putchar('\7');                  //== 0x07 ��������PC����һ��
            }
        }
    }
}

/****************************************************************************
* ��    �ƣ�getchar
* ��    �ܣ��Ӽ����豸����һ�ֽ�����(1��ASCII�ַ�)
* ��ڲ�������
* ���ڲ������յ����ַ�
* ˵    ��: scanf��������øú�����Ϊ�ײ����롣����ֱ�ӵ���ReadKey()������
****************************************************************************/
/*int getchar()
{
    return ReadKey();
}*/