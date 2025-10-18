/*******************************************************************************
* 实验名			   : 定时器实验
* 使用的IO	     : 
* 实验效果       :1602显示时钟，按K3进入时钟设置，按K1选择设置的时分秒，按K2选择
*选择设置加1。 
*	注意					 ：
*******************************************************************************/
#include<reg51.h>	
#include"lcd.h"
sbit K1=P3^0;
sbit K2=P3^1;
sbit K3=P3^2;
sbit K4=P3^3;

unsigned char Time;
//用来计时间的值

void Delay1ms(unsigned int c);
void TimerConfiguration();
void Int0Configuration();

unsigned char SetPlace;
 
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	unsigned char hour,minit,second;
	unsigned int i;
	TimerConfiguration();
	Int0Configuration();
	LcdInit();
	hour=12;
	LcdWriteData('0'+hour/10);
	LcdWriteData('0'+hour%10);
	LcdWriteData('-');
	LcdWriteData('0'+minit/10);
	LcdWriteData('0'+minit%10);
	LcdWriteData('-');
	LcdWriteData('0'+second/10);
	LcdWriteData('0'+second%10);
	while(1)
	{
		if(TR0==0)
		{
			if(K1==0)		//检测按键K2是否按下
			{
				Delay1ms(10);	//消除抖动
				if(K1==0)
				{
					SetPlace++;
					if(SetPlace>=3)
						SetPlace=0;					
				}

				while((i<50)&&(K1==0))	 //检测按键是否松开
				{
					Delay1ms(1);
					i++;
				}
				i=0;
			}
			if(K2==0)		//检测按键K3是否按下
			{
				Delay1ms(10);	//消除抖动
				if(K2==0)
				{
					if(SetPlace==0)
					{
						second++;
						if(second>=60)
							second=0;
					}
					else if(SetPlace==1)
					{
						minit++;
						if(minit>=60)
							minit=0;	
					}
					else
					{
						hour++;
						if(hour>=24)
							hour=0;
					}				
				}

				while((i<50)&&(K2==0))	 //检测按键是否松开
				{
					Delay1ms(1);
					i++;
				}
				i=0;
			}
		}	
		if(Time>=20)	 //一秒钟来到改变数值
		{
			Time=0;
			second++;
			if(second==60)
			{
				second=0;
				minit++;
				if(minit==60)
				{
					minit=0;
					hour++;
					if(hour==24)
					{
						hour=0;	
					}
				}				
			}					
		}	
	    //--显示时钟--//
		LcdWriteCom(0x80);
		LcdWriteData('0'+hour/10);
		LcdWriteData('0'+hour%10);
		LcdWriteCom(0x83);
		LcdWriteData('0'+minit/10);
		LcdWriteData('0'+minit%10);
		LcdWriteCom(0x86);
		LcdWriteData('0'+second/10);
		LcdWriteData('0'+second%10);
	
	}				
}
/*******************************************************************************
* 函 数 名       : Delay1ms()
* 函数功能		 : 延时1ms
* 输    入       : c
* 输    出       : 无
*******************************************************************************/		   

void Delay1ms(unsigned int c)   //误差 0us
{
    unsigned char a,b;
	for (; c>0; c--)
	{
		for(b=199;b>0;b--)
		{
			for(a=1;a>0;a--);
		}
        
	}
    
}

/*******************************************************************************
* 函 数 名         : TimerConfiguration()
* 函数功能		   : 配置定时器值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		   

void TimerConfiguration()
{
    TMOD = 0x01; //选择工作方式1
    TH0 = 0x3C;	 //设置初始值
    TL0 = 0x0B0; 
    EA = 1;			 //打开总中断
    ET0 = 1;		 //打开定时器0中断
    TR0 = 1;		 //启动定时器0
}
/*******************************************************************************
* 函 数 名         : Timer0()
* 函数功能		   : 定时器0中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		   

void Timer0() interrupt 1
{
	  TH0 = 0x3C;	 //设置初始值
    TL0 = 0x0B0;
		Time++;
}
/*******************************************************************************
* 函 数 名         : Int0Configuration()
* 函数功能		   : 配置外部中断0
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		   

void Int0Configuration()
{
	//设置INT0
	IT0=1;//跳变沿出发方式（下降沿）
	EX0=1;//打开INT0的中断允许。
	EA=1;//打开总中断	
}
/*******************************************************************************
* 函 数 名         : Int0() interrupt 0
* 函数功能		   : 外部中断0的中断函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/		   

void Int0() interrupt 0		 
{
	Delay1ms(10);
	if(K3==0)
	{
		TR0=~TR0;
		SetPlace=0;
	}
}
