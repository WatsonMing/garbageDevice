#ifndef __HX711_H__
#define __HX711_H__

#include <Arduino.h>

#define _Co 0.00245845
#define _AMP 128

#define pulse(pin)               \
    {                            \
        digitalWrite(pin, HIGH); \
        digitalWrite(pin, LOW);  \
    }

class HX711
{
private:
    byte SCK;
    byte DOUT;
    byte AMP;
    long OFFSET;
    double COEFFICIENT;

public:
//clean=8253450.69;
    double clean=0.0;
    void removeInitOffset(void);
     //start up clean
    // define sck , dout pin, amplification factor and coefficient
    //HX711(byte sck, byte dout, byte amp = 128, double co = 1);
    // set amplification factor, take effect after one call to read()
    void set_amp(byte amp);
    // test hx711 is ready or not, will be called in read()
    bool is_ready();
    // return difference votage, will be blocked if hx711 is not ready
    long read();
    // return (read() - offset) * coefficient
    unsigned short bias_read();
    // set no-load value to offset, euqla to average of t times read();
    void tare(int t = 10);
    // set coefficient
    void set_co(double co = 1);
    // set offset
    void set_offset(long offset = 0);
    void init(byte sck, byte dout, byte amp = 128, double co = 1);

    
};
#endif