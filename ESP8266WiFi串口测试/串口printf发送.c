#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#define uchar  unsigned char
#define uint   unsigned int

#define S_RECEVIER_SIZE  32
unsigned char RevBuffer[S_RECEVIER_SIZE];
int revDataCount = 0;
int revTempLength = 0;

/**********************************************************

 ��ʱ�ӳ���

**********************************************************/
void delay(unsigned int z)  
{  
    unsigned int x,y;  
    for(x=z;x>0;x--)  
        for(y=110;y>0;y--);  
}


void uart_receiver(void) interrupt 4 //�����ж�
{
    unsigned char temp; //��ʱ���������ڻ����շ�����

    if(RI)  // �ж��Ǵ��ڽ��ղ����ж�
    {
        RI = 0;       // ������жϱ�־
        temp = SBUF;  // ���յ�������д�뻺��BUF
//        SBUF = temp;  // ���յ������ݷ��ظ����Զ�

        if (temp != '\n') // �ԡ�\n����Ϊ�����ַ���������־ 
        {
            RevBuffer[revDataCount] = temp;
            revDataCount++;
        } 
        else
        {     
            printf("%s\n", &RevBuffer[0]);
            revTempLength = revDataCount;
            revDataCount = 0;
        } 
    }

    //if (TI)      // �ж��Ǵ��ڷ��Ͳ����ж�
    //  TI = 0;  // �巢���ж�
}


/*********************************************************

  ������

*********************************************************/
void main(void)
{

	SCON=0x50;           //�趨���ڹ�����ʽ
	PCON=0x00;           //�����ʲ�����

	TMOD=0x20;           //��ʱ��1������8λ�Զ�����ģʽ, ���ڲ���������
	EA=1;
	ES = 1;              //�������ж�
	TL1=0xfd;
	TH1=0xfd;             //������9600
	TR1=1;
	TI=1;//ֱ��ʹ��printf�������˾����ʵ�ַ���

	while(1)
		{
			printf("51printf\n");
      delay(100);
		}
}

/*********************************************************/
