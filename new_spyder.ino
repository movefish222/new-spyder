#define LED 2   // LED引脚配置
#include<SoftwareSerial.h>
// #include"duka.h"
// #include"yvyin.h"

//TODO:猜想1,谁放最后谁不响
//TODO:现在张艳蕊的车已经可以了
SoftwareSerial softSerial1(5,4); //软串口 rx:5 tx:4

//读卡器自动读数据模式
byte auto_read_config[] = { 
      0x7F, 0x0D, 0x00, 0x2E, 0x02, 0x0C, 0x04,0x00, 
      0x00, 0x00, 0x01, 0x23, 0x12, 0x54, 0x4D 
  };
  
//读卡器秘钥设置，系统默认秘钥A：FFFFFFFFFFFF  秘钥B：FFFFFFFFFFFF  
byte key_Setting[] = { 
      0x7F, 0x15, 0x00, 0x2B, 0xFF, 0xFF, 0xFF, 0xFF, 
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
      0x00, 0x03, 0x08, 0x05, 0x02, 0x07, 0x35 
  };
  
//UID标签里存储的GB2312信息比对数组定义


//语音播报模块调用指令
byte yuyin_voiceSetting[] = {
      0x7E, 0x04, 0x31, 0x1E, 0x2B, 0xEF
  };
/**
 * 初始化函数
 */
const int IN1 = 9, IN2 = 10, STBY = 11, PWM = 8;
bool status[20];
void setup() 
{
    for(int i = 0; i < 16; i++){
      *(status + i) = true;
    }
    pinMode( 13, OUTPUT );
    pinMode( LED, OUTPUT);  //设置管脚为输出
    pinMode( IN1, OUTPUT); 
    pinMode( IN2, OUTPUT);
    pinMode( PWM, OUTPUT);
    pinMode( STBY, OUTPUT);
        
    Serial.begin(9600); // 波特率9600
    //while (!Serial);    // 等待打开串口
    
   //初始化软串口通信；
   softSerial1.begin(9600);     
   //监听软串口通信
   softSerial1.listen();
   delay(30);
   //读卡器自动读数据模式
  //  dump_byte_array1(auto_read_config, 15);
  //  delay(30);
  //  //读卡器秘钥设置，系统默认秘钥A：FFFFFFFFFFFF  秘钥B：FFFFFFFFFFFF  
  //  dump_byte_array1(key_Setting, 23);
  //  dump_byte_array(yuyin_voiceSetting, 6);

  /*
   delay(100);
   //读卡器自动读数据模式
   dump_byte_array1(auto_read_config, 15);
    delay(100);
   //读卡器秘钥设置，系统默认秘钥A：FFFFFFFFFFFF  秘钥B：FFFFFFFFFFFF  
   dump_byte_array1(key_Setting, 23);
   */
   while (softSerial1.available())
   {
    softSerial1.read();
    }
    digitalWrite(STBY, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    delay(3000);
    digitalWrite(LED, LOW);//关闭LED
    digitalWrite(PWM, HIGH);
  // dump_byte_array(yuyin_ruijin, 7);
  // delay(3000);
  // dump_byte_array(yuyin_ruijin, 7);
}
 
/**   
 * Main主循环函数
 */

byte LastReceive[30];
void loop() 
{  
   bool isDetect = false;
   byte ReceiveData[30]; // 接收数据数组
   //byte size = sizeof(ReceiveData);
   byte count = 0;
   memset(ReceiveData, 0, sizeof(ReceiveData));

  //读取从设备A传入的数据，并在串口监视器中显示  
  if(softSerial1.available()>0)
  { 
    isDetect = true;
    digitalWrite(13, HIGH);//打开板载led灯
    memset(ReceiveData, 0, sizeof(ReceiveData));
    byte IndexMark = 0;
    while (softSerial1.available())
    {
      delay(1);
      ReceiveData[IndexMark] = softSerial1.read();
      IndexMark++;
    }
  }
  digitalWrite(13, LOW);//关闭板载LED
  if(!isDetect) return;
  
  if(memcmp(LastReceive, ReceiveData, sizeof(ReceiveData)) == 0) return;
  dump_byte_array(ReceiveData, sizeof(ReceiveData));

  if(isDetect){
    digitalWrite(LED, HIGH);
    delay(2000);  //亮灯1000毫秒，延时时间需要适当修改
    digitalWrite(LED, LOW);//关闭LED 
  }
  memcpy(LastReceive, ReceiveData, sizeof(ReceiveData));
}

/**
 * hex array output
 * 发送到语音模块
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 11; i < bufferSize - 2; i++) {
        Serial.write(buffer[i]);
    }
}

/**
 * serial 1 hex array output
 * 发送到软串口
 */
void dump_byte_array1(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        softSerial1.write(buffer[i]);
    }
}
