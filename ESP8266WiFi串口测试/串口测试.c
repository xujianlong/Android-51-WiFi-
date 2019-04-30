#include<reg52.h>
#include<sdio.h>

#define uchar unsigned char
#define uint  unsigned int
  
uchar flag,i;
uchar code table[]="I get ";
uchar a[110]={0};

void init()
{
  TMOD=0X20;
  TH1=0XFD;
  TL1=0XFD;  //设置波特率9600
  TR1=1;
  REN=1;
  SM0=0;
  SM1=1;
  EA=1;
  ES=1;
}


void main()
{
  init();
  while(1)
  {
    if(flag)
    {
      ES=0;
      for(i=0;i<6;i++)
      {
        SBUF=table[i];
        while(!TI);
        TI=0;
      }
      SBUF=a[1];
      while(!TI);
      TI=0;
      ES=1;
      flag=0;
    }
  }
}

////串口中断，接收单个
//void ser() interrupt 4
//{
//  RI=0;
//  a[1]=SBUF;
//  flag=1;
//}

////串口中断，接收多个
//void ser() interrupt 4
//{
//  int i=0;
//  if(RI)
//  {
//    RI=0;
//    a[i]=
//  }
//}

