#include <gpio.h>


void initGPIO(void)
{
    // 初始化模式
    pinMode(LED, OUTPUT);
    pinMode(Relay, OUTPUT);
    // 初始化状态
    digitalWrite(LED, HIGH);
    digitalWrite(Relay, HIGH);
}

void useIO(unsigned char PIN, unsigned char OP)
{
    if (PIN != LED && PIN != Relay)
        return;
    if (OP != HIGH && OP != LOW)
        return;
    //判断是否符合写入端口条件
    digitalWrite(PIN, OP);

}