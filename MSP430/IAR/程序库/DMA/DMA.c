/*----------------------------------------------------------------
 * Copyright (C) 2011 ���մ�ѧ ����ѧԺ416 ����ԭ
 * ��Ȩ���С� 
 * 
 * �ļ����� DMA.c
 * 
 * �ļ�����������   
 *          MSP430F16xϵ�е�Ƭ��DMA�����
 *      ˵����  
 *          �ó������DMAģ���ͨ�ó���⣬���ñ�����������DMA����
 *          ��������ʹ��DMA���Լ���Ҫ�������ʱ���������亯����
 *      ʹ�÷�ʽ��
 *          �����м��뱾�ļ�DMA.c��Դ�������ļ�����DMA.h��Ȼ�������
 *          Ӧ�������DMA�����õȹ��ܡ�
 *          
 * ������ʶ��   ����ԭ20110829
 * 
 * �޸ı�ʶ��   
 * �޸�������   
**----------------------------------------------------------------*/

#include <msp430x16x.h>
#include "DMA.h"

/************************************************************
* �������ƣ�DMAInit
* ��    �ܣ�DMA�õ�����ؼĴ�������Դ��ʼ��
* ��    ����channel:ͨ�� 0 1 2
*           trigger������Դ 
        ����Դ��0000    DMAREQ bit (software trigger)
                0001    TACCR2 CCIFG bit
                0010    TBCCR2 CCIFG bit
                0011    URXIFG0 (UART/SPI mode), USART0 data received (I2C mode)
                0100    UTXIFG0 (UART/SPI mode), USART0 transmit ready (I2C mode)
                0101    DAC12_0CTL DAC12IFG bit
                0110    ADC12 ADC12IFGx bit
                0111    TACCR0 CCIFG bit
                1000    TBCCR0 CCIFG bit
                1001    URXIFG1 bit
                1010    UTXIFG1 bit
                1011    Multiplier ready
                1100    No action
                1101    No action
                1110    DMA0IFG bit triggers DMA channel 1
                        DMA1IFG bit triggers DMA channel 2
                        DMA2IFG bit triggers DMA channel 0
                1111    External trigger DMAE0
*           transMode������ģʽ
                'S'�����δ���   's'���ظ����δ���(single)
                'B'���鴫��     'b'���ظ��鴫��(block)
                'I'��ͻ���鴫�� 'i'���ظ�ͻ���鴫��(interleaved) ����
*           srcMode��Դ��ַ��ģʽ
                0   000���ִ��䣬��ַ����
                1   001���ִ��䣬��ַ����
                2   010���ִ��䣬��ַ�ݼ�
                3   011���ִ��䣬��ַ����
                4   100���ֽڴ��䣬��ַ����
                5   101���ֽڴ��䣬��ַ����
                6   110���ֽڴ��䣬��ַ�ݼ�
                7   111���ֽڴ��䣬��ַ����
*           dstMode��Ŀ�ĵ�ַģʽ ȡֵ������ͬsrcMode
*           src��Դ��ַ 16λ
*           dst��Ŀ�ĵ�ַ 16λ
*           size��ÿ������ֽڻ��� ��
* �� �� ֵ����
* ˵    �������ô˺����������ö�ӦDMAͨ������ز���������Ĭ��
            ���£���DMA����ʱ����CPU��ǰָ����ɺ���Ӧ�� ����
            ��˳��̶� DMA0-2ͨ�����ȼ����ͣ� �������ⲿNMI��
            ��DMA���䣻������DMA�жϣ������ش���DMA���䡣����
            ������Ҫ�������и��ġ�
* ʾ    ����DMAInit(0,0x01,'s',0,1,0x0240,0x0220,1): �ֵ�����
            �� ���ִ��䡣
*************************************************************/
void DMAInit(char channel,char trigger,char transMode,char srcMode,char dstMode,
             unsigned int src,unsigned int dst,unsigned int size)
{
    unsigned int *DMAxCTL,*DMAxSA,*DMAxDA,*DMAxSZ;
    
    DMACTL0 = trigger << (channel << 2);
    DMACTL1 = 0x04;         //DMA�յ���������ʱ���ȴ���ǰָ��ִ����ɺ�
    
    switch (channel)        //ѡ��ǰ�����ĸ�DMAͨ��
    {
        case 0: 
            DMAxCTL = (unsigned int *)&DMA0CTL;
            DMAxSA = (unsigned int *)&DMA0SA;
            DMAxDA = (unsigned int *)&DMA0DA;
            DMAxSZ = (unsigned int *)&DMA0SZ;
            break;                                      //ָ�� = 0ͨ������
        case 1: 
            DMAxCTL = (unsigned int *)&DMA1CTL;
            DMAxSA = (unsigned int *)&DMA1SA;
            DMAxDA = (unsigned int *)&DMA1DA;
            DMAxSZ = (unsigned int *)&DMA1SZ;
            break;                                      //ָ�� = 1ͨ������
        case 2: 
            DMAxCTL = (unsigned int *)&DMA2CTL;
            DMAxSA = (unsigned int *)&DMA2SA;
            DMAxDA = (unsigned int *)&DMA2DA;
            DMAxSZ = (unsigned int *)&DMA2SZ;
            break;                                      //ָ�� = 2ͨ������
    }
    
    switch (transMode)      //����DMAͨ���Ĵ���ģʽ
    {
        case 'S': *DMAxCTL = DMADT_0;   break;          //���δ���
        case 's': *DMAxCTL = DMADT_4;   break;          //�ظ����δ���
        case 'B': *DMAxCTL = DMADT_1;   break;          //�鴫��
        case 'b': *DMAxCTL = DMADT_5;   break;          //�ظ��鴫��
        case 'I': *DMAxCTL = DMADT_2;   break;          //ͻ���鴫�� ����
        case 'i': *DMAxCTL = DMADT_6;   break;          //�ظ�ͻ���鴫�� ����
    }
    
    *DMAxCTL |= (srcMode & 0x04) << 2;                  //Դ �ֻ��ֽ�
    *DMAxCTL |= (srcMode & 0x03) << 8;                  //Դ ��ַ�ı䷽ʽ
    
    *DMAxCTL |= (dstMode & 0x04) << 3;                  //Ŀ�� �ֻ��ֽ�
    *DMAxCTL |= (dstMode & 0x03) << 10;                 //Ŀ�� ��ַ�ı䷽ʽ
    
    *DMAxSA = src;
    *DMAxDA = dst;
    *DMAxSZ = size;
    
    *DMAxCTL |= DMAEN;                                  //DMAʹ��
}

/************************************************************
* �������ƣ�DMAReEnable
* ��    �ܣ�DMA����ʹ��
* ��    ����channel:Ҫʹ�ܵ�ͨ��
* �� �� ֵ����
* ˵    ����DMA�������������ظ�ģʽ�����һ�δ����DMAENλ
            �Զ���λ��Ҫ�ٴδ��������������ܡ�������ظ�ģʽ��
            DMAENλ����λ�����Լ����������䡣   �ظ�ģʽ������
            �ظ����δ���(single)\�ظ��鴫��(block)\�ظ�ͻ����
            ����(interleaved ����)
*************************************************************/
void DMAReEnable(char channel)
{
    switch (channel)        //ʹ�ܶ�Ӧͨ��
    {
        case 0: DMA0CTL |= DMAEN;   break;      //0ͨ��
        case 1: DMA1CTL |= DMAEN;   break;      //1ͨ��
        case 2: DMA2CTL |= DMAEN;   break;      //2ͨ��
    }
}

/************************************************************
* �������ƣ�DMAStart
* ��    �ܣ���DMA����Ϊ�������ģʽʱ���ô˺�������DMA���䡣
* ��    ����channel:Ҫ���������ͨ��
* �� �� ֵ����
* ˵    ����������Դ����Ϊ��0000 DMAREQ bit (software trigger)
            ʱ����ҪDMAREQλ����DMA��ʼ���䡣 ���ô˺��������
            �˹��ܡ�
*************************************************************/
void DMAStart(char channel)
{
    switch (channel)        //ʹ�ܶ�Ӧͨ��
    {
        case 0: DMA0CTL |= DMAREQ;  break;      //0ͨ��
        case 1: DMA1CTL |= DMAREQ;  break;      //1ͨ��
        case 2: DMA2CTL |= DMAREQ;  break;      //2ͨ��
    }
}
