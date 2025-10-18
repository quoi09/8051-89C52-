#include <reg51.h>
#include <intrins.h>

sbit SRCLK = P3^6;
sbit RCLK = P3^5;
sbit SER = P3^4;

sbit btn_up = P3^0;
sbit btn_down = P3^1;
sbit btn_left = P3^2;
sbit btn_right = P3^3;

#define COMMONPORTS P0

// B?ng ch?n hàng cho LED ma tr?n 8x8
unsigned char code ROW_SELECT[8] = {0x7f, 0xbf, 0xdf, 0xef, 0xf7, 0xfb, 0xfd, 0xfe};

// D? li?u ma tr?n LED d? v? r?n và m?i
unsigned char led_matrix[8] = {0};

// T?a d? r?n
unsigned char snake_x[64] = {0};
unsigned char snake_y[64] = {0};
unsigned char snake_length = 3; // Chi?u dài ban d?u c?a r?n

unsigned char food_x = 3;
unsigned char food_y = 5;
unsigned char direction = 1; // 0: lên, 1: ph?i, 2: xu?ng, 3: trái

void delay(unsigned int time) {
    unsigned int i, j;
    for (i = 0; i < time; i++)
        for (j = 0; j < 121; j++);
}

// G?i d? li?u d?n 74HC595
void Hc595SendByte(unsigned char dat) {
    unsigned char a;
    SRCLK = 0;
    RCLK = 0;
    for (a = 0; a < 8; a++) {
        SER = dat >> 7;
        dat <<= 1;

        SRCLK = 1;
        _nop_();
        _nop_();
        SRCLK = 0;
    }
    RCLK = 1;
    _nop_();
    _nop_();
    RCLK = 0;
}

// Hi?n th? d? li?u lên ma tr?n LED
void display_matrix() {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        Hc595SendByte(led_matrix[i]);
        COMMONPORTS = ROW_SELECT[i];
        delay(1);
    }
}

// T?o th?c an ? v? trí ng?u nhiên
void generate_food() {
    food_x = rand() % 8;
    food_y = rand() % 8;
}

// Ki?m tra di?u khi?n di chuy?n c?a ngu?i choi
void update_direction() {
    if (!btn_up && direction != 2) direction = 0;
    if (!btn_right && direction != 3) direction = 1;
    if (!btn_down && direction != 0) direction = 2;
    if (!btn_left && direction != 1) direction = 3;
}

// Di chuy?n r?n trên ma tr?n LED
void move_snake() {
    unsigned char i;

    // D?ch v? trí thân r?n
    for (i = snake_length; i > 0; i--) {
        snake_x[i] = snake_x[i - 1];
        snake_y[i] = snake_y[i - 1];
    }

    // Di chuy?n d?u r?n theo hu?ng hi?n t?i
    switch (direction) {
        case 0: snake_y[0] = (snake_y[0] - 1) % 8; break; // lên
        case 1: snake_x[0] = (snake_x[0] + 1) % 8; break; // ph?i
        case 2: snake_y[0] = (snake_y[0] + 1) % 8; break; // xu?ng
        case 3: snake_x[0] = (snake_x[0] - 1) % 8; break; // trái
    }

    // Ki?m tra n?u r?n an m?i
    if (snake_x[0] == food_x && snake_y[0] == food_y) {
        snake_length++;
        generate_food();
    }

    // C?p nh?t ma tr?n LED v?i v? trí m?i c?a r?n
    for (i = 0; i < 8; i++) {
        led_matrix[i] = 0x00; // Xóa ma tr?n
    }
    for (i = 0; i < snake_length; i++) {
        led_matrix[snake_y[i]] |= (0x80 >> snake_x[i]);
    }

    // V? th?c an trên ma tr?n LED
    led_matrix[food_y] |= (0x80 >> food_x);
}

void main() {
    // Kh?i t?o v? trí ban d?u c?a r?n
    snake_x[0] = 4;
    snake_y[0] = 4;
    snake_x[1] = 3;
    snake_y[1] = 4;
    snake_x[2] = 2;
    snake_y[2] = 4;

    generate_food(); // T?o m?i ban d?u

    while (1) {
        update_direction(); // C?p nh?t hu?ng di c?a r?n
        move_snake();       // Di chuy?n r?n
        display_matrix();   // Hi?n th? ma tr?n LED
        delay(200);         // Ð? tr? d? di?u ch?nh t?c d? r?n
    }
}
