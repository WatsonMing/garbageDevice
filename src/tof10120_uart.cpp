#include <tof10120_uart.h>
#include <string.h>

char USART_RX_BUF[UartReadSize] = {};
unsigned char numSize = 0;
unsigned short lenth_buf[UartReadSize];
unsigned short avelenth = 0;

void CalAveLength(void)
{
    unsigned char i;
    unsigned short sum = 0;
    unsigned short max = 0;
    unsigned short min = 9999;
    for (i = 0; i < numSize; i++)
    {
        sum += lenth_buf[i];
        if (lenth_buf[i] > max)
            max = lenth_buf[i];
        if (lenth_buf[i] < min)
            min = lenth_buf[i];
    };
    avelenth = (sum - max - min) / (numSize - 2);
}
unsigned short CharToUnsignedShort(char *str)
{
    unsigned short temp = 0;
    temp = (unsigned short)strtol(str, NULL, 10);
    //Serial.println(temp);
    return temp;
}
void CalLength(void)
{
    char *result = NULL;
    result = strtok(USART_RX_BUF, "mm");
    while (result)
    {
        lenth_buf[numSize] = CharToUnsignedShort(result) - 0x30;
        result = strtok(NULL, "mm");
        //Serial.write(result);
        numSize++;
        //Serial.write(lenth_buf[numSize]); //打印接收数据
    }

    //取平均值
    CalAveLength();
    //格式化输出字符串
    //FormatStr();
    //清空串口接收数组
    numSize = 0;
    memset(USART_RX_BUF, 0, sizeof(USART_RX_BUF));
}

unsigned short Read_ToF_Sensor(void)
{
    unsigned char usart_read_count = 0; //读取数
    bool usart_read_start = false;      //读取正常数据标志
    bool isM = false;                   //是否是m
    char res = 0;                       //读取缓存

    while (Serial1.available() > 0) //串口接收到了数据长度
    {
        res = Serial1.read();
        if (res == 0x6d) //m
        {
            isM = true;
            //Serial.write("-read m-");
        }
        if (res != 0x6d && isM == true) //取到m且,下一位不为m
        {
            usart_read_start = true;
            //Serial.write("-read num-");
        }
        if (usart_read_start == true)
        {
            //Serial.write("--read-normal--");
            if (usart_read_count == UartReadSize) //一次读16个字节
            {
                //usart_read_start = false;//读完去除
                //usart_read_count = 0;//读完去除
                break; //读取完毕
            }
            else
            {
                if ((res != 0x0a) && (res != 0x0d)) //去除回车，换行
                {

                    USART_RX_BUF[usart_read_count] = res;
                    usart_read_count++;
                }
            }
        }
    }
    if (usart_read_start == true)
    {
        CalLength();
        usart_read_start = false;
        usart_read_count = 0;
        return avelenth;
    }
    return 0;
}
void FormatStr(void)
{
    char dispbuf[6] = {"-----"};
    if (avelenth >= 1000)
    {
        dispbuf[0] = avelenth / 1000 + 0x30;
        dispbuf[1] = avelenth / 100 % 10 + 0x30;
        dispbuf[2] = avelenth / 10 % 10 + 0x30;
        dispbuf[3] = avelenth % 10 + 0x30;
        dispbuf[4] = 'm';
        dispbuf[5] = 'm';
    }
    else if (avelenth >= 100)
    {
        dispbuf[0] = avelenth / 100 % 10 + 0x30;
        dispbuf[1] = avelenth / 10 % 10 + 0x30;
        dispbuf[2] = avelenth % 10 + 0x30;
        dispbuf[3] = 'm';
        dispbuf[4] = 'm';
        dispbuf[5] = 0x00;
    }
    else if (avelenth >= 10)
    {
        dispbuf[0] = avelenth / 10 % 10 + 0x30;
        dispbuf[1] = avelenth % 10 + 0x30;
        dispbuf[2] = 'm';
        dispbuf[3] = 'm';
        dispbuf[4] = 0x00;
        dispbuf[5] = 0x00;
    }
    else
    {
        dispbuf[0] = avelenth % 10 + 0x30;
        dispbuf[1] = 'm';
        dispbuf[2] = 'm';
        dispbuf[3] = 0x00;
        dispbuf[4] = 0x00;
        dispbuf[5] = 0x00;
    }
    Serial.write(dispbuf);
    memset(dispbuf, 0, sizeof(dispbuf));
}