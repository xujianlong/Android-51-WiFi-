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

 延时子程序

**********************************************************/
void delay(unsigned int z)  
{  
    unsigned int x,y;  
    for(x=z;x>0;x--)  
        for(y=110;y>0;y--);  
}


void uart_receiver(void) interrupt 4 //串口中断
{
    unsigned char temp; //临时变量，用于缓冲收发数据

    if(RI)  // 判断是串口接收产生中断
    {
        RI = 0;       // 清接收中断标志
        temp = SBUF;  // 接收到的数据写入缓冲BUF
//        SBUF = temp;  // 将收到的数据发回给电脑端

        if (temp != '\n') // 以‘\n’做为接收字符串结束标志 
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

    //if (TI)      // 判断是串口发送产生中断
    //  TI = 0;  // 清发送中断
}


/*********************************************************

  主函数

*********************************************************/
void main(void)
{

	SCON=0x50;           //设定串口工作方式
	PCON=0x00;           //波特率不倍增

	TMOD=0x20;           //定时器1工作于8位自动重载模式, 用于产生波特率
	EA=1;
	ES = 1;              //允许串口中断
	TL1=0xfd;
	TH1=0xfd;             //波特率9600
	TR1=1;
	TI=1;//直接使用printf必须加入此句才能实现发送

	while(1)
		{
			printf("51printf\n");
      delay(100);
		}
}

/*********************************************************/
