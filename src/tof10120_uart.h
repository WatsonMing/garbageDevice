#ifndef __TOF10120_UART_H__
#define __TOF10120_UART_H__

#include <Arduino.h>

#define Distance_xmm 3
#define Distance_xxmm 4
#define Distance_xxxmm 5
#define Distance_xxxxmm 6

#define NumSize 10
#define LengthSize NumSize*Distance_xmm
#define UartReadSize Distance_xxxxmm *NumSize
#define StandardError 10

unsigned short CharToUnsignedShort(char *charBuff);
void FormatStr(void);
void CalAveLength(void);
void CalLength(void);
unsigned short Read_ToF_Sensor(void);

#endif