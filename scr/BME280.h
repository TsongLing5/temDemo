/*
 * BME280.h
 *
 *  Created on: 2021年1月23日
 *      Author: aria
 */

#ifndef SCR_BME280_H_
#define SCR_BME280_H_

#include <stdint.h>

//#define int8_t char;
//#define uint8_t char;
//#define int16_t char;
//#define uint16_t char;
//#define int32_t char;
#define BYTES 20 //Max number of bytes sent on SPI


volatile int32_t RawTemp, RawPress, RawHumid;
volatile uint16_t dig_T1, dig_P1;
volatile int16_t  dig_T2, dig_T3;
volatile int16_t  dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
volatile int32_t t_fine;
volatile uint8_t dig_H1, dig_H3, i;
volatile int8_t dig_H6;
volatile int16_t dig_H2, dig_H4, dig_H5;
volatile unsigned char  Tempbuf[30], Humbuf[15];
char RXbuf[BYTES], RXdata[BYTES], TXbuf[BYTES], Data[BYTES];

uint8_t ReadTHid(void);

#endif /* SCR_BME280_H_ */
