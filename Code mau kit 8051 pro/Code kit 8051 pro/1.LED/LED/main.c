#include<reg51.h>

sbit LED1=P2^0;
void Delay(unsigned int a);
void main()
{
	while(1)
	{
		LED1=0;//µãÁÁ
		Delay(5000);
		LED1=1;
		Delay(5000);
	}
}

void Delay(unsigned int a)//0~65535
{
	unsigned char b;
	for(;a>0;a--)
	{
		for(b=110;b>0;b--);
	}
}