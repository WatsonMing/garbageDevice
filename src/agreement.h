#ifndef __AGREEMENT_H__
#define __AGREEMENT_H__

#include <Arduino.h>
#include <gpio.h>
#include <hx711.h>
#include <version.h>
#include <tof10120_uart.h>
/**************************接收报文*******************************/
/* 报头 地址 操作码  校验码   报尾 */
/*各占字节数 */
/* 0xAA  1  OPCode   CRC8    0x55 */
#define FrontRx 0xAA
#define TailRx 0x55
#define ReadSize 5
#define CommandSize 5
#define DataSize 9
/**************************发送报文*******************************/
/* 报头 地址 操作码  *数据*  校验码   报尾 */
/*各占字节数 Byte*/
/* 0x55  1  OPCode  noLimit  CRC8    0xAA */
#define FrontTx 0x55
#define TailTx 0xAA

/****************************操作码 OPCode*********************************/
//读取操作使用段 0x0*
#define ReadAll 0xA0
// #define ReadWeight 0x02
// #define ReadFull 0x01
//读取操作使用段 0x5*
#define CloseDoor 0x50
#define OpenDoor 0x51
/****************************操作数据*********************************/
#define ON LOW
#define OFF HIGH

void PrintVersion();
void SoftWareInit(); //软件初始化入口
char *substring(char *ch, int pos);
unsigned char getXor(char *datas, unsigned char length);
void analysis(char *received);
void buildReadTx();
void buildCommandTx();
#endif
