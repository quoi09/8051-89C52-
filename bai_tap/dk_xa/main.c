#include"reg51.h"
#include"XPT2046.h"




sbit led1=P2^0;
sbit led2=P2^1;
sbit led3=P2^2;

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
							
							
							
							if ((newKey != 0)&& (pre_newKey != newKey))  // bam phim moi
							{		
								irKey = Decode_IRKey(newKey);             // giai ma phim
								if (irKey == 1 )             // phim 1 thi dao led 1
                    led1=~led1;
								//ToggleLED(irKey); 
								if( irKey==2 )  // phim 2 thi dao led 2
										led2=~led2;
								if( irKey==3 )  // phim 2 thi dao led 2
										led3=~led3;
				
							pre_newKey = newKey;                          // C?p nh?t gi? tr? ph?m tru?c
						}
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



