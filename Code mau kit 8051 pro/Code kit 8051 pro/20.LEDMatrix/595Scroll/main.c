#include"reg51.h"
#include<intrins.h>

sbit SRCLK=P3^6;
sbit RCLK=P3^5;
sbit SER=P3^4;

/*******************************************************************************
* 函数名         : Hc595SendByte(unsigned char dat)
* 函数功能		   : 想74H595发送一个字节的数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/
void Hc595SendByte(unsigned char dat)
{
	unsigned char a;
	SRCLK=0;
	RCLK=0;
	for(a=0;a<8;a++)
	{
		SER=dat>>7;
		dat<<=1;

		SRCLK=1;
		_nop_();
		_nop_();
		SRCLK=0;	
	}

	RCLK=1;
	_nop_();
	_nop_();
	RCLK=0;
}
/*******************************************************************************
* 函数名         : Delay10ms
* 函数功能		   : 延时函数，延时10ms
* 输入           : c
* 输出         	 : 无
*******************************************************************************/
void Delay10ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
    for(;c>0;c--)
        for(b=38;b>0;b--)
            for(a=130;a>0;a--);
}
void main(void)
{
	unsigned char i,num;
   while(1)
   {
   		num=0x01;
   		for(i=0;i<8;i++)
		{
		  	P0=~num;
			Hc595SendByte(0xff);
			num<<=1;
			Delay10ms(50);
		}
   		num=0x01;
   		for(i=0;i<8;i++)
		{		  	
			Hc595SendByte(num);
			P0=0x00;
			num<<=1;
			Delay10ms(50);
		}
   
   }
}