#include"reg51.h"
#include"XPT2046.h"

#define GPIO_DIG P0

sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;
sbit led=P2^7;
sbit led1=P2^0;


unsigned char code DIG_CODE[17]={
0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
// 0 1 2 3 4 5 6 7 8 9 A B C D E F

unsigned char irKey = 0;
unsigned long bitPattern = 0;
unsigned long newKey = 0;
unsigned long pre_newKey = 0;
unsigned char timerValue;
unsigned char msCount = 0;
char pulseCount = 0;


void Timer0_Init(void);
unsigned char Decode_IRKey(unsigned char key);
void ToggleLED(unsigned char ledNum);  // dao trang thai led 
unsigned char DisplayData[8];		//
void DigDisplay(void);
void delay(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 120; j++) {
            ; // Do nothing for delay
        }
    }
}



void main(void)
					{
						 uint temp;
						 uint sec=5;
						 uint t1o,t1n,t2o,t2n;
						 P2 = 0xFF;          // 
						 Timer0_Init();      // 
						 EA = 1;             //
						
						while (1)
						{ 
							temp = Read_AD_Data(0xA4);
							if(temp<150) led=0; //  r quang tro <125 thi den sang 
							else led=1;   
							// troi toi den sang 
							
							
							if ((newKey != 0)&& (pre_newKey != newKey))  // bam phim moi
							{		
								irKey = Decode_IRKey(newKey);             // giai ma phim
								if (irKey == 1 )             // phim 1 thi dao led 1
                    led1=~led1;
								//ToggleLED(irKey);  
								
							// 
								
								if( irKey==2 )  // phim 2 thi dao led 2
								{ 
									for(sec=6; sec>=0;sec--)
									{
									  LSA=1;
										LSB=1;
										LSC=1;

										GPIO_DIG=DIG_CODE[sec];	
										delay(1000);
										if(sec==0) {ToggleLED(irKey); break; }  
									}
									
								}
							// hen gio sang den
								
							}
							pre_newKey = newKey;                          // C?p nh?t gi? tr? ph?m tru?c
						}
					}

void DigDisplay(void)
{
	unsigned char i;
	unsigned int j;
	for(i=0;i<0;i++)
	{
		switch(i)	 //
		{
			case(0):
				LSA=0;LSB=0;LSC=0; break;// led 0
			case(1):
				LSA=1;LSB=0;LSC=0; break;// led 1
			case(2):
				LSA=0;LSB=1;LSC=0; break;// led 2
			case(3):
				LSA=1;LSB=1;LSC=0; break;// led 3
			case(4):
				LSA=0;LSB=0;LSC=1; break;// led 4
			case(5):
				LSA=1;LSB=0;LSC=1; break;// led 5
			case(6):
				LSA=0;LSB=1;LSC=1; break;// led 6
			case(7):
				LSA=1;LSB=1;LSC=1; break;// led 7	
		}
		GPIO_DIG=DisplayData[i];//?
		j=50;						 //
		while(j--);	
		GPIO_DIG=0x00;// 
	}
	
}

unsigned char Decode_IRKey(unsigned char key){
    unsigned char returnValue = 0;
    switch (key)
    {
    case 0xFF30CF:  // Ph?m '1'
        returnValue = 1;
        break;
    case 0xFF18E7:  // Ph?m '2'
        returnValue = 2;
        break;
    case 0xFF7A85:  // Ph?m '3'
        returnValue = 3;
        break;
    default:
        returnValue = 0;  // M? kh?ng h?p l?
        break;
    }
    return returnValue;
}

// Ng?t Timer0 d? d?m mili gi?y
void timer0_isr() interrupt 1
{
    if (msCount < 50)
        msCount++;
    
    // C?u h?nh l?i gi? tr? Timer0 d? ti?p t?c d?m 1ms
    TH0 = 0xFC; 
    TL0 = 0x18;  // Gi? tr? du?c t?nh cho t?n s? 12MHz
}

// Ng?t ngo?i d? nh?n t?n hi?u IR
void externalIntr0_ISR() interrupt 0
{
    timerValue = msCount;
    msCount = 0;
    
    TH0 = 0xFC; 
    TL0 = 0x18;  // C?u h?nh l?i Timer0

    pulseCount++;
    
    if ((timerValue >= 50))  // N?u d? r?ng xung l?n hon 50ms, d?nh d?u b?t d?u khung (Start Of Frame)
    {
        pulseCount = -2;     // B? qua 2 xung d?u
        bitPattern = 0;
    }
    else if ((pulseCount >= 0) && (pulseCount < 32)) // T?ch luy gi? tr? bit t? 0 d?n 31
    {
        if (timerValue >= 2) // Xung c? d? r?ng > 2ms l? bit 1
            bitPattern |= (unsigned long)1 << (31 - pulseCount);
    }
    else if (pulseCount >= 32) // K?t th?c khung khi nh?n d? 32 bit
    {
        newKey = bitPattern;  // Luu m? ph?m m?i
        pulseCount = 0;
    }
}

// Kh?i t?o Timer0
void Timer0_Init(){
    TMOD |= 0x01;     // C?u h?nh Timer0 ? ch? d? 16-bit (MODE 1)
    TH0 = 0xFC; 
    TL0 = 0x18;       // Gi? tr? kh?i t?o cho 1ms t?i t?n s? 12 MHz
    TR0 = 1;          // B?t d?u Timer0
    ET0 = 1;          // Cho ph?p ng?t Timer0
    IT0 = 1;          // C?u h?nh ng?t ngo?i INT0 k?ch ho?t ? c?nh xu?ng
    EX0 = 1;          // Cho ph?p ng?t ngo?i INT0
}

// H?m d?i tr?ng th?i LED tuong ?ng
void ToggleLED(unsigned char ledNum){
    // ledNum = 1, 2, 3 tuong ?ng v?i LED t?i P2.0, P2.1, P2.2
    switch (ledNum) {
        case 1: 
            P2 ^= 0x01;  // ??i tr?ng th?i LED1 (P2.0)
            break;
        case 2: 
            P2 ^= 0x02;  // ??i tr?ng th?i LED2 (P2.1)
            break;
        case 3: 
            P2 ^= 0x04;  // ??i tr?ng th?i LED3 (P2.
		}
	}
