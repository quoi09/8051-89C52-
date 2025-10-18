#ifndef _LED7SEG_ONKIT_H_
#define _LED7SEG_ONKIT_H_

#include <REGX52.h>
#include "base_lib.h"
#include "ThreeWiresProtocol.h"

// Chân di?u khi?n v? trí LED 7-Segment
sbit GND0 = P2^2;
sbit GND1 = P2^3;
sbit GND2 = P2^4;

// C?ng xu?t d? li?u hi?n th? LED 7-Segment
#define LED_7SEG P0

// Mã hi?n th? cho các s? t? 0-9 và ký t? d?c bi?t
const ubyte DIGIT_CODE[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 
    0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x01, 0x40, 0x08
};

// Bi?n toàn c?c
ubyte LED[8] = {0};    // M?ng ch?a d? li?u hi?n th? trên LED
ubyte DISP_Time = 5;   // Th?i gian quét m?t LED (ms)
ubyte DISP_Freq = 24;  // T?n s? quét LED (Hz)
ubyte DISP = 0;        // C? ki?m soát hi?n th?

/**
 * @brief C?p nh?t t?n s? quét LED.
 * @param newDISP_Freq T?n s? quét m?i (Hz).
 */
void set_disp_freq(uint newDISP_Freq) {
    DISP_Freq = newDISP_Freq;
    DISP_Time = 1000 / (DISP_Freq * 8);
}

/**
 * @brief Hi?n th? m?t ký t? lên LED t?i v? trí ch? d?nh.
 * @param pos V? trí LED (1-8).
 * @param code Mã hi?n th? ký t?.
 */
void led7seg_disp(uint pos, uint code) {
    // T?t t?t c? các chân GND
    GND0 = GND1 = GND2 = 1;

    // Thi?t l?p v? trí LED
    if (pos > 0 && pos <= 8) {
        switch (pos) {
            case 7: GND0 = 0; break;
            case 6: GND1 = 0; break;
            case 5: GND0 = GND1 = 0; break;
            case 4: GND2 = 0; break;
            case 3: GND0 = GND2 = 0; break;
            case 2: GND1 = GND2 = 0; break;
            case 1: GND0 = GND1 = GND2 = 0; break;
        }
        LED_7SEG = code;
    } else {
        LED_7SEG = 0x00; // T?t n?u v? trí không h?p l?
    }
}

/**
 * @brief Hi?n th? toàn b? d? li?u t? m?ng LED[] lên LED 7-Segment.
 */
void Disp8leds7seg() {
    for (uint i = 0; i < 8; ++i) {
        led7seg_disp(i + 1, LED[i]);
        // Ð?i m?t kho?ng th?i gian nh? d? quét LED
        for (uint j = 0; j < 180; ++j);
    }
    LED_7SEG = 0x00; // T?t t?t c? sau khi quét
}

#endif // _LED7SEG_ONKIT_H_
