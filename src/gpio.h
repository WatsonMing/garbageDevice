#ifndef __GPIO_H__
#define __GPIO_H__

#include <Arduino.h>
/****************************GPIO 的端口在这定义*********************************/
#define LED PB11
#define Relay PB10
#define HX711_SCK PB2
#define HX711_DOUT PB1
void initGPIO(void);
//void usePWM(unsigned char PIN,unsigned int val);
void useIO(unsigned char PIN,unsigned char OP);
#endif