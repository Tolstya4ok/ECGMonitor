/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Nixietube.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ�������printf��ֲ
 *      ˵����  
 *          �ó������һ��puchar���������ַ���ʾ�������,printf������
 *          �ײ�����������������ܷ���Ҫ��ʾ���ַ���
 *          ʹ��ʱֻ���Nixietube.c��ӵ������У�Ȼ����Ҫʹ��printf��
 *          Դ�����ļ��м����stdio.h�İ�����Ȼ��Ϳ���ʹ��printf����
 *          ������ܷ���Ҫ��ʾ���ַ��ˡ�֧�ֵ��ַ��ο���Nixietube.h
 *          
 * ������ʶ��   ����ԭ20110823
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/
#include <stdio.h>
#include "ctype.h"       /*isdigit������Ҫ��ͷ�ļ�*/
#include "Nixietube.h"

/****************************************************************************
* ��    �ƣ�putchar
* ��    �ܣ����������ʾ���һ��ASCII�ַ���
* ��ڲ�����ch: �����͵��ַ�  
* ���ڲ������������ַ�
* ˵    ��: printf��������øú�����Ϊ�ײ�������ú������ַ�������������
            ���printf�Ľ������ʾ��������ϡ�
****************************************************************************/
int putchar(int ch)
{
    //'\f'��ʾ��ֽ��ҳ���൱�������ʾ
    if(ch=='\n'||ch=='\r')
        NixiettubeClear();
    
    //���ֺͶ�ӦASCII��ĸ֮���0x30   '1'=0x31 '2'=0x32... 
    //isdigitҲ��C���Ա�׼����
    if(isdigit(ch))
        NixiettubeInsertChar(ch-0x30);  //���ַ�����������ʾ����
    else             //���򣬲������֣�����ĸ
        {
        switch(ch)    //������ĸѡ������֧
        {
            case 'A': case 'a': NixiettubeInsertChar(AA);break; //�ַ�A
            case 'B': case 'b': NixiettubeInsertChar(BB);break; //�ַ�B
            case 'C': case 'c': NixiettubeInsertChar(CC);break; //...
            case 'D': case 'd': NixiettubeInsertChar(DD);break;  
            case 'E': case 'e': NixiettubeInsertChar(EE);break;
            case 'F': case 'f': NixiettubeInsertChar(FF);break;
            case 'G': case 'g': NixiettubeInsertChar(GG);break;
            case 'H': case 'h': NixiettubeInsertChar(HH);break;
            case 'I': case 'i': NixiettubeInsertChar(II);break; 
            case 'J': case 'j': NixiettubeInsertChar(JJ);break;
            case 'K': case 'k': NixiettubeInsertChar(KK);break;
            case 'L': case 'l': NixiettubeInsertChar(LL);break;
            case 'M': case 'm': NixiettubeInsertChar(mm);break;
            case 'N':           NixiettubeInsertChar(NN);break;
            case 'n':           NixiettubeInsertChar(nn);break;
            case 'O':           NixiettubeInsertChar(OO);break;
            case 'o':           NixiettubeInsertChar(oo);break;
            case 'P': case 'p': NixiettubeInsertChar(PP);break; 
            case 'Q': case 'q': NixiettubeInsertChar(QQ);break; 
            case 'R': case 'r': NixiettubeInsertChar(rr);break; 
            case 'S': case 's': NixiettubeInsertChar(SS);break; 
            case 'T': case 't': NixiettubeInsertChar(tt);break; 
            case 'U': case 'v': NixiettubeInsertChar(UU);break; 
            case 'V': case 'u': NixiettubeInsertChar(VV);break; 
            case 'W': case 'w': NixiettubeInsertChar(WW);break; 
            case 'Y': case 'y': NixiettubeInsertChar(YY);break; //...
            case 'Z': case 'z': NixiettubeInsertChar(ZZ);break; //�ַ�Z
            case '-':           NixiettubeInsertChar(NEG);break;//�ַ�-
            case '.':           NixiettubeInsertChar(DOT);break;//С���㣬ֱ����ʾ�����½�
            case ' ':           NixiettubeInsertChar(SP);break; //�ո�
            default :           NixiettubeInsertChar(SP);break;//��ʾ����������ĸ�ÿո����
        } 
    } 
    return(ch);  //������ʾ���ַ�(putchar������׼��ʽҪ�󷵻���ʾ�ַ�)
}
