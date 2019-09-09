#include <hx711.h>

void HX711::init(byte sck, byte dout, byte amp, double co)
{
    SCK = sck;
    DOUT = dout;
    set_amp(amp);
    COEFFICIENT = co;
    pinMode(SCK, OUTPUT);
    pinMode(DOUT, INPUT);
    digitalWrite(SCK, LOW);
}
void HX711::set_amp(byte amp)
{
    switch (amp)
    {
    case 32:
        AMP = 2;
        break;
    case 64:
        AMP = 3;
        break;
    case 128:
        AMP = 1;
        break;
    }
}

bool HX711::is_ready()
{
    return digitalRead(DOUT) == LOW;
}

long HX711::read()
{
    long val = 0;
    while (!is_ready())
        ;
    for (int i = 0; i < 24; i++)
    {
        pulse(SCK);
        val <<= 1;
        if (digitalRead(DOUT) == HIGH)
            val++;
    }
    for (int i = 0; i < AMP; i++)
    {
        pulse(SCK);
    }
    val ^= 0x800000;
    if ((val - clean) > 0)
        val = val - clean;
    else
        val = 0;
    return val; // & (1L << 23) ? val | ((-1L) << 24) : val;
}

unsigned short HX711::bias_read()
{
    return ((read() - OFFSET) * COEFFICIENT);
}

void HX711::tare(int t)
{
    double sum = 0;
    for (int i = 0; i < t; i++)
    {
        sum += read();
    }
    set_offset(sum / t);
}

void HX711::set_offset(long offset)
{
    OFFSET = offset;
}

void HX711::set_co(double co)
{
    COEFFICIENT = co;
}

void HX711::removeInitOffset(void) //开机去偏移量
{
    int i = 0;
    double sum = 0;
    for (i = 0; i < 100; i++)
    {
        sum += read(); // 累加
    }
    clean = sum / 100;
    //Serial.print("clean=>");
    //Serial.print(clean);
    //Serial.write(0x0d);
}