#ifndef __LCD_H_
#define __LCD_H_

#include<reg51.h>
//#define uint unsigned int 
//#define uchar unsigned char

#define GPIO_LCD P0
sbit LCDE=P2^7;
sbit RW=P2^5;
sbit RS=P2^6;

void Delay1ms(unsigned int);
void LcdWriteCom(unsigned char);
void LcdWriteData(unsigned char );
void LcdInit();

#endif
