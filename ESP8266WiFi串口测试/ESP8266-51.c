#include <reg52.h>
#include <intrins.h>
#include <stdio.h>

#define uchar  unsigned char
#define uint   unsigned int

#define USART_REC_LEN  100        //接收缓冲,最大USART_REC_LEN个字节.

unsigned char USART_RX_BUF[USART_REC_LEN];

unsigned int USART_RX_STA1=0;       //接收状态标记

unsigned int USART_RX_CNT = 0,flag=0;

sbit led1=P1^0;	 //第一个led是和P1^0口连接的。
sbit led3=P1^2;
sbit led5=P1^4;
sbit led7=P1^6;

sbit led=P1^7;

/**********************************************************

 延时子程序

**********************************************************/
void delay(unsigned int z)
{
	unsigned int x,y;
	for(x=z; x>0; x--)
		for(y=110; y>0; y--);
}

int find_01()
{
	int i=0,weizhi=0,len=0;
	for(i=0; i<USART_REC_LEN; i++)
		{
			if(USART_RX_BUF[i]=='R')
				{
					weizhi=i;
//          printf("s:%d\n",weizhi);
					break;
				}
		}
	return weizhi;

}


void uart_receiver(void) interrupt 4 //串口中断
{
	unsigned char res;
	int rev_flag=0;
  if(RI)
  {
    res=SBUF; //取出接受到的数据
    RI=0;
    //if(res==':'||USART_RX_CNT>0||res!='\n') //i是全局变量
		{
      rev_flag=1;
			USART_RX_BUF[USART_RX_CNT]=res; //receive数组也是全局变量
			USART_RX_CNT++;
		}
    if(rev_flag)
    {
      flag=1; 
      rev_flag=0;
    }
  }

}


void init()
{
	TMOD=0X20;
	TH1=0XFD;
	TL1=0XFD;  //设置波特率9600
	TR1=1;
	SM0=0;
	SM1=1;
	REN=1;
	EA=1;
	ES=1;
	TI=1;//直接使用printf必须加入此句才能实现发送


//  SCON=0x50;           //设定串口工作方式
//	PCON=0x00;           //波特率不倍增

//	TMOD=0x20;           //定时器1工作于8位自动重载模式, 用于产生波特率
//	EA=1;
//	ES = 1;              //允许串口中断
//	TL1=0xfd;
//	TH1=0xfd;             //波特率9600
//	TR1=1;
//	TI=1;//直接使用printf必须加入此句才能实现发送


}
void led_buling()
{
	led=~led;	//没隔一段时间即对输出取反。
	delay(200); //闪烁间隔。

}

/*********************************************************

  主函数

*********************************************************/
void main(void)
{

	unsigned char rev_data[3];
	int i=0,weizhi_01=0,deal_flag=0;
	init();
  delay(150);
  printf("AT+CWJAP=\"123\",\"QAZXCVBNM123\"\r\n");
  delay(400);
	printf("AT+CIPMUX=1\r\n");
	delay(100);
	printf("AT+CIPSERVER=1,2001\r\n");
	delay(100);
	while(1)
		{
			if(flag&&!deal_flag)
				{
					led=~led;	//没隔一段时间即对输出取反。
					ES = 0;      //关闭串口中断
					USART_RX_BUF[USART_RX_CNT]='\0';
					weizhi_01=find_01();
					for(i=0; i<3; i++)
						{
              deal_flag=1;
							rev_data[i]=USART_RX_BUF[weizhi_01+i];
//							printf("rev_data[%d]:%c\n",i,rev_data[i]);
						}

					if(rev_data[0]=='R')
						{
							//if(rev_data[1]==0xf0)
								{
									if(rev_data[1]==rev_data[2])
										{
//											printf("rev:%c\n",rev_data[2]);
											switch(rev_data[2])
												{
												case '1':
													led1=0;
													break;
												case '3':
													led1=1;
													break;
												case '5':
													led3=0;
													break;
												case '7':
													led3=1;
													break;
												default :
													led_buling();
													break;
												}
										}
//					else{led5=0;}
								}
//				else{led5=1;}
						}
//			else{led5=0;}
					
          for (USART_RX_CNT=0;USART_RX_CNT<USART_REC_LEN;USART_RX_CNT++)
          {
          
            USART_RX_BUF[USART_RX_CNT]='K';
          }
          USART_RX_CNT=0;
          flag=0;
          deal_flag=0;
					ES = 1;      //开串口中断
				}
			delay(10);
		}
}

/*********************************************************/
