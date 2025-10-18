#include <main.h>
int chedo;
int chuyenhuong;
unsigned char code Code7segCatot[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77};
unsigned char BNchay[] = {1,1,0,1,1,1,1,0}, BNvang[] = {1,1,1,0,1,1,1,0}, DTchay[] = {1,1,1,1,0,0,1,1}, DTvang[] = {1,1,1,1,0,1,0,1};
int dem;
int tanso;
void delay_ms(unsigned int time)
{	
	/* f = 12M hz*/
	/*f vào timer 1 = 12M/2 = 6M*/
	/*T = 1/6M s*/
	/* 6000 --> 1ms*/
	/*59536--> 65535 -> 0 = 1ms*/
	while(time--){
		TMOD = 0x01;
		TH1 = 0xE8;
		TL1 = 0x90;
		TR1 = 1;
		while(!TF1);
		TF1 = 0;
	}
}
void Traffic_Auto(void)
{
	int i;
//	StatusTraffic = 0x21;
		P3_6 = 0;
		P3_5 = 0;
		for(i = 0; i< 8;i++)
		{
			P3_4 = BNchay[i];
			delay_ms(1);
			P3_6 = 1;
			delay_ms(1);
			P3_6 = 0;
		}
		P3_5 = 1;
    // delay10s
    for (dem = 10; dem >= 3; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {  
						chonLED = 0x08;
            /*dvi_DT = 1;*/
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_DT = 0;
            dvi_BN = 1;*/
						chonLED = 0x00;
            Display = Code7segCatot[(dem - 3)%10];
            delay_ms(10);
            /*dvi_BN = 0;
            chuc_DT = 1;*/
					chonLED = 0x0c;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);
						chonLED = 0x04;
            Display = Code7segCatot[(dem - 3)/10];
            delay_ms(10);

        }
    }
//	StatusTraffic = 0x9;
		P3_6 = 0;
		P3_5 = 0;
		for(i = 0; i< 8;i++)
		{
			P3_4 = BNvang[i];
			delay_ms(1);
			P3_6 = 1;
			delay_ms(1);
			P3_6 = 0;
		}
		P3_5 = 1;
    // delay3s
    for (dem = 2; dem >= 0; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {

           chonLED = 0x08;
            /*dvi_DT = 1;*/
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_DT = 0;
            dvi_BN = 1;*/
						chonLED = 0x00;
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_BN = 0;
            chuc_DT = 1;*/
					chonLED = 0x0c;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);
            /*
            chuc_DT = 0;
            chuc_BN = 1;*/
						chonLED = 0x04;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);

        }
    }
//	StatusTraffic = 0x12;
    // delay10s
		P3_6 = 0;
		P3_5 = 0;
		for(i = 0; i< 8;i++)
		{
			P3_4 = DTchay[i];
			delay_ms(1);
			P3_6 = 1;
			delay_ms(1);
			P3_6 = 0;
		}
		P3_5 = 1;
    for (dem = 10; dem >= 3; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {
						/*0001_1100*/
            /*dvi_BN = 1;*/
						chonLED = 0x00;
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_BN = 0;
            dvi_DT = 1;*/
						chonLED = 0x08;
            Display = Code7segCatot[(dem - 3)%10];
            delay_ms(10);
            /*dvi_DT = 0;
            chuc_BN = 1;*/
						chonLED = 0x04;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);
						/*
            chuc_BN = 0;
            chuc_DT = 1;*/
						chonLED = 0x0c;
            Display = Code7segCatot[(dem - 3)/10];
            delay_ms(10);

        }
    }
//	StatusTraffic = 0x6; 
    // delay3s
		P3_6 = 0;
		P3_5 = 0;
		for(i = 0; i< 8;i++)
		{
			P3_4 = DTvang[i];
			delay_ms(1);
			P3_6 = 1;
			delay_ms(1);
			P3_6 = 0;
		}
		P3_5 = 1;
    for (dem = 2; dem >= 0; dem--)
    {
        if (chedo == 1)
        {
            return;
        }
        for (tanso = 0; tanso < 30; tanso++)
        {
            /*0001_1100*/
            /*dvi_BN = 1;*/
						chonLED = 0x0;
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_BN = 0;
            dvi_DT = 1;*/
						chonLED = 0x08;
            Display = Code7segCatot[dem % 10];
            delay_ms(10);
            /*dvi_DT = 0;
            chuc_BN = 1;*/
						chonLED = 0x04;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);
            /*
            chuc_BN = 0;
            chuc_DT = 1;*/
						chonLED = 0x0c;
            Display = Code7segCatot[dem / 10];
            delay_ms(10);

        }
    }
}
void Interrupt_Timer0(void)
{
		/* f = 12M hz*/
		/*f vào timer 0 = 12M/12 = 1M*/
		/*T = 1/6M s*/
		/* 1000 --> 1ms*/
		/*64536--> 65535 -> 0 = 1ms*/
    /* 16 bits timer */
    TMOD = 0x01;
    /* setup 1ms for overforlow */
		
    TH0 = 0xFC;
    TL0 = 0x18;
    /* Enable interrupt */
    ET0 = 1;
    EA = 1;
    /* Enable timer0 */
    TR0 = 1;
}

void ISR_TIMER(void) interrupt 1
{
    if (ET0 == 1)
    {
        if (SW_chedo == 0)
        {
            delay_ms(100);
            if (SW_chedo == 1)
            {
                chedo++;
            }
            if (chedo > 1)
                chedo = 0;
        }
        /*setup 1ms for overforlow*/
        TH0 = 0xFC;
        TL0 = 0x18;
    }
}

void main(void)
{
	int i;
    Interrupt_Timer0();
    while (1)
    {
        if (chedo == 1)
        {
						P2 = 0xff;
						P0 = 0x00;
            if (SW == 0)
            {
                delay_ms(100);
                if (SW == 1)
                {
                    chuyenhuong++;
                }
                if (chuyenhuong > 1)
                    chuyenhuong = 0;
            }
						if(chuyenhuong)
						{
							P3_6 = 0;
							P3_5 = 0;
							for(i = 0; i< 8;i++)
							{
								P3_4 = BNchay[i];
								delay_ms(1);
								P3_6 = 1;
								delay_ms(1);
								P3_6 = 0;
							}
							P3_5 = 1;
						}
						else
						{
							P3_6 = 0;
							P3_5 = 0;
							for(i = 0; i< 8;i++)
							{
								P3_4 = DTchay[i];
								delay_ms(1);
								P3_6 = 1;
								delay_ms(1);
								P3_6 = 0;
							}
							P3_5 = 1;							
						}
        }
        if (chedo == 0)
            Traffic_Auto();
    }
}
								
