#include <agreement.h>

/***************串口变量****************/
// unsigned char res = 0x00; //接受的变量
char RXBUFF[ReadSize];    //缓存数组
// unsigned char size = 0;   //接受下标
// bool recvdFlag = false;   //是否开始缓存数据
/**************************************/
void setup()
{
  SoftWareInit();
}
void loop()
{
  if (Serial.available() > 0) //串口接收到了数据长度
  {
    Serial.readBytes(RXBUFF, ReadSize);
    //Serial.print(RXBUFF); //打印接收
    if ((RXBUFF[0] == FrontRx) && (RXBUFF[ReadSize - 1] == TailRx))
    {
      //Serial.print("ok");
      analysis(RXBUFF); //解析
    }
    memset(RXBUFF, 0, sizeof(RXBUFF)); //清空数组
  }
  // res = Serial.readBytes();
  // if (res == FrontRx) //接受到报头
  // {
  //   recvdFlag = true; //开始
  //   size = 0;
  //   RXBUFF[size] = FrontRx;
  // }
  // else if (recvdFlag == true) //开始接受数据报
  // {
  //   if (size < ReadSize - 1)
  //   {
  //     size++;
  //     RXBUFF[size] = res;
  //   }
  //   else //==
  //   {
  //     if (RXBUFF[ReadSize - 1] == TailRx)
  //     {
  //       Serial.print(RXBUFF); //打印接收
  //       analysis(RXBUFF);     //解析
  //     }
  //     memset(RXBUFF, 0, sizeof(RXBUFF)); //清空数组
  //     recvdFlag = false;                 //去除标志
  //     size = 0;                          //下标归零
  //   }
  // }
}
