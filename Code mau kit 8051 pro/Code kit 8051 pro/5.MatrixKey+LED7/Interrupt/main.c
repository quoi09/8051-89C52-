/*******************************************************************************
* 实 验 名		 : 动态显示数码管实验
* 使用的IO	     : 数码管使用P0,P2.2,P2.3,P2.4键盘使用P1
* 实验效果       : 按矩阵键盘分别显示在数码管上面显示十六进制的0到F。
* 注    意		 ：
*******************************************************************************/
#include<reg51.h>
//#include<intrins.h>	

#define GPIO_DIG P0
#define GPIO_KEY P1

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char KeyValue;
//用来存放读取到的键值
unsigned char KeyState;	//记录按键的状态，0没有，1有
unsigned char DisplayData[8];
//用来存放要显示的8位数的值
unsigned char Num;//用来存放中断的时候显示的第位数值
void Delay50us();   //延时50us
void KeyDown();		 //检测按键函数
void DigDisplay(); //动态显示函数
void TimerConfiguration();//定时器初始化设置
/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void main(void)
{
	TimerConfiguration();
	KeyState=0;	 //初始化按键状态
	while(1)
	{
		KeyDown();
	 	if(KeyState==1)
		{
			DisplayData[7]=DisplayData[6];
			DisplayData[6]=DisplayData[5];
			DisplayData[5]=DisplayData[4];
			DisplayData[4]=DisplayData[3];
			DisplayData[3]=DisplayData[2];
			DisplayData[2]=DisplayData[1];
			DisplayData[1]=DisplayData[0];
			DisplayData[0]=DIG_CODE[KeyValue];
			KeyState=0;
		}
//		DigDisplay();
	}				
}
/*******************************************************************************
* 函 数 名         : TimerConfiguration
* 函数功能		   : 定时器初始化
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void TimerConfiguration()
{
	TMOD=0X02;//选择为定时器0模式，工作方式2，仅用TRX打开启动。

	TH0=0X9C;	//给定时器赋初值，定时100us
	TL0=0X9C;	
	ET0=1;//打开定时器0中断允许
	EA=1;//打开总中断
	TR0=1;//打开定时器			
}
/*******************************************************************************
* 函 数 名         : DigDisplay
* 函数功能		   : 使用数码管显示
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void DigDisplay()
{
	unsigned char i,j;
//	for(i=0;i<8;i++)
//	{
		GPIO_DIG=0x00;//消隐
		switch(i)	 //位选，选择点亮的数码管，
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;
			case(1):
				LSA=1;LSB=0;LSC=0; break;
			case(2):
				LSA=0;LSB=1;LSC=0; break;
			case(3):
				LSA=1;LSB=1;LSC=0; break;
			case(4):
				LSA=0;LSB=0;LSC=1; break;
			case(5):
				LSA=1;LSB=0;LSC=1; break;
			case(6):
				LSA=0;LSB=1;LSC=1; break;
			case(7):
				LSA=1;LSB=1;LSC=1; break;	
		}
		GPIO_DIG=DisplayData[i];
		i++;
		if(i>7)
			i=0;
//		j=10;						 //扫描间隔时间设定
//		while(j--)
//			Delay50us();	
//		GPIO_DIG=0x00;//消隐
//	}
}
/*******************************************************************************
* 函 数 名         : KeyDown
* 函数功能		   : 检测有按键按下并读取键值
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void KeyDown(void)
{
	unsigned int a=0;
	GPIO_KEY=0x0f;
	if(GPIO_KEY!=0x0f)
  {
		Delay50us();
		a++;	
		a=0;
		if(GPIO_KEY!=0x0f)
		{
			ET0=0;//关定时器中断
			KeyState=1;//有按键按下
			//测试列
			GPIO_KEY=0X0F;
// 			Delay50us();
			switch(GPIO_KEY)
			{
				case(0X07):	KeyValue=0;break;
				case(0X0b):	KeyValue=1;break;
				case(0X0d): KeyValue=2;break;
				case(0X0e):	KeyValue=3;break;
//				default:	KeyValue=17;	//检测出错回复17意思是把数码管全灭掉。
			}
			//测试行
			GPIO_KEY=0XF0;
			Delay50us();
			switch(GPIO_KEY)
			{
				case(0X70):	KeyValue=KeyValue;break;
				case(0Xb0):	KeyValue=KeyValue+4;break;
				case(0Xd0): KeyValue=KeyValue+8;break;
				case(0Xe0):	KeyValue=KeyValue+12;break;
//				default:	KeyValue=17;
			}
			ET0=1;//开定时器中断
			while((a<5000)&&(GPIO_KEY!=0xf0))	 //检测按键松手检测
			{
				Delay50us();
				a++;
			}
			a=0;
		}
	}
}
/*******************************************************************************
* 函 数 名         : Delay50us
* 函数功能		   : 延时函数，延时50us
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Delay50us(void)   //延时50us误差 0us
{
    unsigned char a,b;
    for(b=1;b>0;b--)
        for(a=22;a>0;a--);
}
/*******************************************************************************
* 函 数 名         : Delay50us
* 函数功能		   : 延时函数，延时50us
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
void Timer() interrupt 1
{
	DigDisplay();	
}