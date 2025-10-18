/*******************************************************************************
* 实验名			  : EEPROM实验
* 使用的IO	    : 
* 实验效果      : 按K1保存显示的数据，按K2读取上次保存的数据，按K3显示数据加一，
*按K4显示数据清零。
*	注意					：由于P3.2口跟红外线共用，所以做按键实验时为了不让红外线影响实验效果，最好把红外线先
*取下来。
*
*********************************************************************************/
#include<reg51.h>
#include"i2c.h"
//数码管IO
#define DIG	P0
sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
//按键IO
sbit K1=P3^1;
sbit K2=P3^0;
sbit K3=P3^2;
sbit K4=P3^3;
void At24c02Write(unsigned char ,unsigned char );
unsigned char At24c02Read(unsigned char );
void Delay1ms();
void Timer0Configuration();
unsigned char code DIG_CODE[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
unsigned char Num=0;
unsigned int disp[8]={0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};
/*******************************************************************************
* 函数名         : main
* 函数功能		   : 主函数
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void main()
{
	unsigned int num0=0,num1=0,n;
	Timer0Configuration();
	while(1)
	{
		if(K1==0)
		{
			Delay1ms();
			if(K1==0)
				At24c02Write(2,num0);
			while((n<200)&&(K3==0))
			{
				n++;
				Delay1ms();	
			}			
			n=0;
			n=0;
		}
		
		if(K2==0)
		{
			Delay1ms();
			if(K2==0)
				num0=At24c02Read(2);
			while((n<200)&&(K3==0))
			{
				n++;
				Delay1ms();	
			}			
			n=0;
		}			
		if(K3==0)
		{
			Delay1ms();
			if(K3==0)
				num0++;
			while((n<200)&&(K3==0))
			{
				n++;
				Delay1ms();	
			}			
			n=0;
			if(num0==256)
				num0=0;
		}
			
		if(K4==0)
		{
			Delay1ms();
			if(K4==0)
				num0=0;
			while((n<200)&&(K3==0))
			{
				n++;
				Delay1ms();	
			}			
			n=0;
		}			
		disp[0]=DIG_CODE[num1/1000];//千位
		disp[1]=DIG_CODE[num1%1000/100];//百位
		disp[2]=DIG_CODE[num1%1000%100/10];//十位
		disp[3]=DIG_CODE[num1%1000%100%10];//个位
		disp[4]=DIG_CODE[num0/1000];//千位
		disp[5]=DIG_CODE[num0%1000/100];//百位
		disp[6]=DIG_CODE[num0%1000%100/10];//个位
		disp[7]=DIG_CODE[num0%1000%100%10];	
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
* 函数名         : Delay1ms()
* 函数功能		   : 延时
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void Delay1ms()   //误差 0us
{
    unsigned char a,b,c;
    for(c=1;c>0;c--)
        for(b=142;b>0;b--)
            for(a=2;a>0;a--);
}
/*******************************************************************************
* 函数名         : void At24c02Write(unsigned char addr,unsigned char dat)
* 函数功能		   : 往24c02的一个地址写入一个数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2cStart();
	I2cSendByte(0xa0);//发送写器件地址
	I2cSendByte(addr);//发送要写入内存地址
	I2cSendByte(dat);	//发送数据
	I2cStop();
}
/*******************************************************************************
* 函数名         : unsigned char At24c02Read(unsigned char addr)
* 函数功能		   : 读取24c02的一个地址的一个数据
* 输入           : 无
* 输出         	 : 无
*******************************************************************************/

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	I2cStart();
	I2cSendByte(0xa0); //发送写器件地址
	I2cSendByte(addr); //发送要读取的地址
	I2cStart();
	I2cSendByte(0xa1); //发送读器件地址
	num=I2cReadByte(); //读取数据
	I2cStop();
	return num;	
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