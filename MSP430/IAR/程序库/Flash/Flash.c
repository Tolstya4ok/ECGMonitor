/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� Flash.c
 * 
 * �ļ�����������   
 *          MSP430F16x,MSP430F14Xϵ�е�Ƭ��Flash�����
 *      ˵����  
 *          �ó������Flash������ģ���ͨ�ó���⣬���ñ��������
 *          ���Flash�Ĳ�����д���ֻ��ֽڣ�������û��ʵ�ֿ�д�빦
 *          �ܡ����ڳ��������ɲ������еĶΣ� ����ʹ��ʱһ��Ҫע
 *          �⣬��Ҫ�Ѽ������еĳ��������ʹ��ʱע�⣬Flash������
 *          ÿ��д��󣬲����ڲ���֮ǰ�ٴ�д�����ݣ� �����������
 *          �𺦡�
 *      ʹ�÷�ʽ��
 *          �����м���Flash.C�ļ����ļ�����Flash.h�� Ȼ�󼴿�ʹ��
 *          �������ĺ����ˡ�
 *          
 * ������ʶ��   ����ԭ20110830
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include "Flash.h"

unsigned char FlashSR;
/*=======================�궨��==========================*/
#define DINT    FlashSR=__get_SR_register();_DINT() // ���жϣ�������״̬�Ĵ���
#define EINT    if(FlashSR & GIE)           _EINT() //���ݱ���Ľ���ж� ���ж�
/*=======================================================*/

/************************************************************
* �������ƣ�FlashInit
* ��    �ܣ�Flash��ؼĴ�������Դ��ʼ��
* ��    ������
* �� �� ֵ����
* ˵    ����ʹ�ó����֮ǰӦ�ȵ��ô˺�����
*************************************************************/
void FlashInit()
{
    FCTL2 = FWKEY + FSSEL_2 + FN1;          // Ĭ�� SMCLK/3 =333KHz 
}

/****************************************************************************
* ��    �ƣ�WaitForEnable
* ��    �ܣ�Flashæ�ȴ�
* ��ڲ������� 
* ���ڲ�������
* ˵    ��: ��Flashæ��ȴ���ֱ��Flash�������(����)�ŷ��ء�
****************************************************************************/
void WaitForEnable()
{
    while((FCTL3 & BUSY) == BUSY);      //Busy
}

/****************************************************************************
* ��    �ƣ�FlashErase
* ��    �ܣ�����Flash��һ�����ݶ�
* ��ڲ�����Addr:���������ݶε��׵�ַ 
* ���ڲ�������
****************************************************************************/
void FlashErase(unsigned int Addr)  
{ 
    char *FlashPtr;
    FlashPtr = (char *)Addr;
    FCTL1 = FWKEY + ERASE;                      // Set Erase bit
    FCTL3 = FWKEY;                              // Clear Lock bit
    DINT;
    *FlashPtr = 0;                              // Dummy write to erase Flash segment B
    WaitForEnable();                            //Busy
    EINT;
    FCTL1 = FWKEY;                              // Lock
    FCTL3 = FWKEY + LOCK;                       // Set Lock bit  
}

/****************************************************************************
* ��    �ƣ�FlashWriteChar
* ��    �ܣ���Flash��д��һ���ֽ�(Char�ͱ���)
* ��ڲ�����Addr:������ݵĵ�ַ
            Data:��д�������
* ���ڲ�������
* ˵    ������ַֻ����ż������������������Զ�ǰ��һλ
* ��    ����FlashWriteChar(0x1000,123);������123д��0x1000��Ԫ
            FlashWriteChar(0x1002,a);�����ͱ���aд��0x1002��Ԫ 
****************************************************************************/
void FlashWriteChar(unsigned int addr,char Data)
{
    char *FlashPtr = (char *)addr;              // Segment A pointer
    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    DINT;
    *FlashPtr = Data;                           // Save Data
    WaitForEnable();                            //Busy
    EINT;
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}  

/****************************************************************************
* ��    �ƣ�FlashWriteChar
* ��    �ܣ���Flash��д��һ����(�޷������ͱ���)
* ��ڲ�����Addr:������ݵĵ�ַ
            Data:��д�������
* ���ڲ�������
* ��    ����FlashWriteChar(0x1000,123);������123д��0x1000��Ԫ
            FlashWriteChar(0x1002,a);�����ͱ���aд��0x1002��Ԫ 
****************************************************************************/
void FlashWriteWord(unsigned int addr,unsigned int Data)
{
    unsigned int *FlashPtr = (unsigned int *)addr;
    FCTL1 = FWKEY + WRT;                        // Set WRT bit for write operation
    FCTL3 = FWKEY;                              // Clear Lock bit
    DINT;
    *FlashPtr = Data;                           // Save Data
    WaitForEnable();                            //Busy
    EINT;
    FCTL1 = FWKEY;                              // Clear WRT bit
    FCTL3 = FWKEY + LOCK;                       // Set LOCK bit
}  

/****************************************************************************
* ��    �ƣ�FlashReadChar
* ��    �ܣ���Flash�ж�ȡһ���ֽ�
* ��ڲ�����Addr:������ݵĵ�ַ
* ���ڲ��������ص�����
****************************************************************************/
char FlashReadChar(unsigned int Addr)
{ 
    char Data;
    char *FlashPtr = (char *) Addr; 
    Data = *FlashPtr;
    return(Data);
}

/****************************************************************************
* ��    �ƣ�FlashReadWord
* ��    �ܣ���Flash�ж�ȡһ����
* ��ڲ�����Addr:������ݵĵ�ַ
* ���ڲ��������ص����ݣ��޷������ͣ�
****************************************************************************/
unsigned int FlashReadWord(unsigned int Addr)
{ 
    unsigned int Data;
    unsigned int *FlashPtr = (unsigned int *)Addr; 
    Data = *FlashPtr;
    return(Data);
}
