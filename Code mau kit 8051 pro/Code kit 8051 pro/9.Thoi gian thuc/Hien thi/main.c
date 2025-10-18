/*******************************************************************************
* 实验名			   : 万年历实验
* 使用的IO	     : 
* 实验效果       :1602显示时钟
*	注意					 ：
*******************************************************************************/

#include<reg51.h>
#include"ds1302.h"

//数码管IO
#define DIG	P0
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

unsigned char code DIG_CODE[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char Num=0;
unsigned int disp[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};

void LcdDisplay();
void Timer0Configuration();

/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void main()
{
	Ds1302Init();
    Timer0Configuration();
	while(1)
	{	
		Ds1302ReadTime();
		disp[7] = DIG_CODE[TIME[0]&0x0f];
		disp[6] = DIG_CODE[TIME[0]>>4];
		disp[5] = 0X40;
		disp[4] = DIG_CODE[TIME[1]&0x0f];
		disp[3] = DIG_CODE[TIME[1]>>4];
		disp[2] = 0X40;
		disp[1] = DIG_CODE[TIME[2]&0x0f];
		disp[0] = DIG_CODE[TIME[2]>>4];	
	}
	
}

/*******************************************************************************
* 函数名         : Timer0Configuration()
* 函数功能		   : 设置计时器
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void Timer0Configuration()
{
	TMOD=0X02;//选择为定时器模式，工作方式2，仅用TRX打开启动。

	TH0=0X9C;	//给定时器赋初值，定时100us
	TL0=0X9C;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器		
}

/*******************************************************************************
* 函数名         : DigDisplay() interrupt 1
* 函数功能		   : 中断数码管显示
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void DigDisplay() interrupt 1
{
//定时器在工作方式二会自动重装初，所以不用在赋值。
//	TH0=0X9c;//给定时器赋初值，定时1ms
//	TL0=0X00;		
	DIG=0; //消隐
	switch(Num)	 //位选，选择点亮的数码管，
	{
		case(7):
			LSA=0;LSB=0;LSC=0; break;
		case(6):
			LSA=1;LSB=0;LSC=0; break;
		case(5):
			LSA=0;LSB=1;LSC=0; break;
		case(4):
			LSA=1;LSB=1;LSC=0; break;
		case(3):
			LSA=0;LSB=0;LSC=1; break;
		case(2):
			LSA=1;LSB=0;LSC=1; break;
		case(1):
			LSA=0;LSB=1;LSC=1; break;
		case(0):
			LSA=1;LSB=1;LSC=1; break;	
	}
	DIG=disp[Num]; //段选，选择显示的数字。
	Num++;
	if(Num>7)
		Num=0;
}
