/*
    Project: Communicate with real-time DS1302 using Three Wires Protocol
    Header-File title: DS1302
    Author: Ngxx.fus
    Based on: DS1302-DATASHEET-DOWNLOAD.pdf
    Note: For more functions, please refer to the DS1302 datasheet.
*/

#ifndef _DS1302_H_
#define _DS1302_H_

#include "base_lib.h"
#include "ThreeWiresProtocol.h"

// Enum for days of the week
typedef enum {
    MON = 0, TUE, WED, THU, FRI, SAT, SUN
} DAY;

// Macro to unlock DS1302 control register
#define ds1302_unlock_reg() single_byte_write(0x8E, 0x0)

// TIME structure for storing date and time information
typedef struct {
    uint DAY;    // Day of the week
    uint DATE;   // Day of the month
    uint MONTH;  // Month
    uint YEAR;   // Year
    uint HOUR;   // Hour
    uint MINUTE; // Minute
    uint SECOND; // Second
} TIME;

/**
 * @brief Reads time from DS1302.
 * @param time Pointer to the TIME structure to store the read values.
 * @param SEL Bitmask to select the fields to read (e.g., 0x1 for SECOND, 0x2 for MINUTE, etc.).
 */
void ds1302_read_time(TIME* time, uint SEL) {
    uint x10, x1, byte_data, AM_PM;

    // Read seconds
    if (SEL & 0x1) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x81);
        x10 = (byte_data >> 4) & 0x07;
        x1 = byte_data & 0x0F;
        time->SECOND = x10 * 10 + x1;
    }
    // Read minutes
    if (SEL & 0x2) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x83);
        x10 = (byte_data >> 4) & 0x07;
        x1 = byte_data & 0x0F;
        time->MINUTE = x10 * 10 + x1;
    }
    // Read hours
    if (SEL & 0x4) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x85);
        if (byte_data & 0x80) { // 12-hour mode
            x10 = (byte_data >> 4) & 0x01;
            x1 = byte_data & 0x0F;
            AM_PM = (byte_data >> 5) & 0x01;
            time->HOUR = x10 * 10 + x1 + (AM_PM ? 12 : 0);
        } else { // 24-hour mode
            x10 = (byte_data >> 4) & 0x03;
            x1 = byte_data & 0x0F;
            time->HOUR = x10 * 10 + x1;
        }
    }
    // Read date
    if (SEL & 0x10) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x87);
        x10 = (byte_data >> 4) & 0x03;
        x1 = byte_data & 0x0F;
        time->DATE = x10 * 10 + x1;
    }
    // Read month
    if (SEL & 0x20) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x89);
        x10 = (byte_data >> 4) & 0x01;
        x1 = byte_data & 0x0F;
        time->MONTH = x10 * 10 + x1;
    }
    // Read year
    if (SEL & 0x40) {
        ds1302_unlock_reg();
        byte_data = single_byte_read(0x8D);
        x10 = (byte_data >> 4) & 0x0F;
        x1 = byte_data & 0x0F;
        time->YEAR = x10 * 10 + x1;
    }
}

/**
 * @brief Writes time to DS1302.
 * @param time Pointer to the TIME structure with the values to write.
 * @param SEL Bitmask to select the fields to write (e.g., 0x1 for SECOND, 0x2 for MINUTE, etc.).
 */
void ds1302_write_time(const TIME* time, uint SEL) {
    uint x10, x1, byte_data;

    // Write seconds
    if (SEL & 0x1) {
        x10 = (time->SECOND / 10) % 10;
        x1 = time->SECOND % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x80, byte_data);
    }
    // Write minutes
    if (SEL & 0x2) {
        x10 = (time->MINUTE / 10) % 10;
        x1 = time->MINUTE % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x82, byte_data);
    }
    // Write hours
    if (SEL & 0x4) {
        x10 = (time->HOUR / 10) % 10;
        x1 = time->HOUR % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x84, byte_data);
    }
    // Write date
    if (SEL & 0x8) {
        x10 = (time->DATE / 10) % 10;
        x1 = time->DATE % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x86, byte_data);
    }
    // Write month
    if (SEL & 0x10) {
        x10 = (time->MONTH / 10) % 10;
        x1 = time->MONTH % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x88, byte_data);
    }
    // Write year
    if (SEL & 0x20) {
        x10 = (time->YEAR / 10) % 10;
        x1 = time->YEAR % 10;
        byte_data = (x10 << 4) | x1;
        ds1302_unlock_reg();
        single_byte_write(0x8C, byte_data);
    }
    // Write day
    if (SEL & 0x40) {
        byte_data = time->DAY & 0x07;
        ds1302_unlock_reg();
        single_byte_write(0x8A, byte_data);
    }
}

/**
 * @brief Initializes the DS1302 module.
 */
void ds1302_initial() {
    ThreeWiresProtocol_Initial();
}

#endif // _DS1302_H_
