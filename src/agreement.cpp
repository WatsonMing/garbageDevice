#include <agreement.h>
/***************变量****************/
HX711 hX711;
unsigned char OPCode = 0x00;        //操作码
unsigned char StatusCode = 0xFF;    //状态码
unsigned char deviceAddress = 0x00; //读取拨码开关获取
unsigned char doorTime = 0;         //定时关闭时间
/**************************************/
// 打印软件版本
void PrintVersion()
{
    Serial.println("Version->");
    Serial.println(Version);
    Serial.print("BuildData->");
    Serial.println(BuildData);
    Serial.print("BuildUser->");
    Serial.println(BuildUser);
    Serial.print("Function->");
    Serial.println(Function);
}
//软件初始化函数
void SoftWareInit()
{
    Serial.begin(9600);                           //USB
    Serial1.begin(9600);                          //TOF
    initGPIO();                                   //gpio初始化
    hX711.init(HX711_SCK, HX711_DOUT, _AMP, _Co); //hX711初始化
    hX711.removeInitOffset();                     //去毛重
    PrintVersion();
}

char *substring(char *ch, int pos, int length)
{
    int i;
    //定义字符指针 指向传递进来的ch地址
    char *pch = ch;
    //通过calloc来分配一个length长度的字符数组，返回的是字符指针。
    char *subch = (char *)calloc(sizeof(char), length);
    pch = pch + pos;
    //是pch指针指向pos位置。
    for (i = 0; i < length; i++)
    {
        subch[i] = *(pch++);
        //循环遍历赋值数组。
    }
    return subch; //返回分配的字符数组地址。
}

/**
 * 亦或校验
 */
unsigned char getXor(char *datas, unsigned char length)
{
    unsigned char i;
    unsigned char temp = 0x00;
    for (i = 0; i < length; i++)
    {
        temp ^= datas[i];
    }
    //Serial.write(temp);
    return temp;
}
unsigned short weight = 0;
unsigned short full = 0;
void analysis(char *received)
{
    unsigned char i;
    OPCode = *(received + 2);
    // if ((*received == FrontRx) && (*(received + ReadSize - 1) == TailRx) && (*(received + 1) == Address)) //确认是否是有效数据
    if (*(received + 1) == deviceAddress)
    {
        //Serial.write(*(received + ReadSize - 2));
        if (getXor(received, ReadSize - 2) == *(received + ReadSize - 2)) //校验成功
        {
            switch (OPCode)
            {
            case ReadAll:
                for (i = 0; i < 24; i++) // 循环的越多精度越高，当然耗费的时间也越多
                {
                    weight += hX711.bias_read(); // 累加
                }
                weight /= 24; // 求平均值进行均差
                full = Read_ToF_Sensor();

                buildReadTx();
                break;
            case CloseDoor:
                useIO(Relay, OFF);
                useIO(LED, OFF);
                buildCommandTx();
                break;
            case OpenDoor:
                useIO(LED, ON);
                useIO(Relay, ON);
                buildCommandTx();
                break;
            default:
                break;
            }
        }
    }
}

void buildReadTx()
{
    char send[DataSize];
    unsigned char i = 0;
    send[0] = FrontTx;
    send[1] = deviceAddress;
    send[2] = OPCode;
    send[3] = (weight & 0xff00) >> 8;
    send[4] = weight & 0xff;
    send[5] = (full & 0xff00) >> 8;
    send[6] = full & 0xff;
    //数据
    send[7] = getXor(send, DataSize - 2);
    send[8] = TailTx;
    for (i = 0; i < DataSize; i++)
    {
        Serial.write(send[i]);
    }
}
void buildCommandTx()
{
    char send[CommandSize];
    unsigned char i = 0;
    send[0] = FrontTx;
    send[1] = deviceAddress;
    send[2] = OPCode;
    send[3] = getXor(send, CommandSize - 2);
    send[4] = TailTx;
    for (i = 0; i < CommandSize; i++)
    {
        Serial.write(send[i]);
    }
}
