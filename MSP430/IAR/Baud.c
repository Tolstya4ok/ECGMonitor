#include<stdio.h>
#include<math.h>

//��������
void SetBaudRateRegisters(long clk,int baud);

/************************�궨��***********************/
#define UxBR1	a[0]
#define	UxBR0	a[1]
#define	UxMCTL	a[2]

unsigned char a[3];                     //����ģ��Ĵ���
void main()
{
	long clk;                           //ʱ��
	long baud;                           //������
	printf("\t---�����ʼ��������---\n");
	printf("\n������ʱ��Ƶ�ʣ�Hz����");
	scanf("%ld",&clk);					
	printf("\n�����벨���ʣ�");
	scanf("%ld",&baud);
	getchar();                          //��ȡ����س���

	SetBaudRateRegisters(clk,baud);     //���üĴ���ֵ

    //��ʾ�Ĵ���ֵ
	printf("\nUxBR1:0x%x\tUxBR0:0x%x\tUxMCTL:0x%x\n",UxBR1,UxBR0,UxMCTL);
	
	getchar();
}

/****************************************************************************
* ��    �ƣ�SetBaudRateRegisters
* ��    �ܣ�����ʱ�� ���������ö�Ӧ�Ĵ���
* ��ڲ�����
*           clk:        ��ѡʱ��Ƶ�ʣ��磺32768��
            baud        ������      (300~115200)
* ���ڲ�������
* ��    ��: SetBaudRateRegisters(32768,9600) //��ʱ��Ƶ��32768����9600�Ĳ�����
****************************************************************************/
void SetBaudRateRegisters(long clk,long baud)
{
	int n = clk / baud;     //����������
	char mSum = 0;			//��mi
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
