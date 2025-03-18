/**
 * 淘宝店铺：蜘蛛电子
 * https://zhizhudianzi.taobao.com
 * 版权所有 未经允许 禁止传播
 * 
 * 装置接线说明:
 * --------------------- 
 * 读卡器      Arduino
 * --------------------- 
 * 5V           5V
 * GND          GND
 * RXD          D4 
 * TXD          D5  
 * 
 * --------------------- 
 * 喇叭         Arduino
 * --------------------- 
 * 5V           5V
 * GND          GND
 * RXD          TX 
 * TXD          RX 
 */
#define LED 2   // LED引脚配置
#include<SoftwareSerial.h>
#include"duka.h"
#include"yvyin.h"

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
 
void loop() 
{  
   bool isDetect = false;
   byte ReceiveData[18]; // 接收数据数组
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
    //Serial.println(IndexMark);
    while (softSerial1.available())
    {
      delay(1);
      ReceiveData[IndexMark] = softSerial1.read();
      IndexMark++;
    }
  }
    digitalWrite(13, LOW);//关闭板载LED
    if(!isDetect) return;
    //判断从标签里读到的信息是什么
    //延安
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == yanan[i])
        {
          count++;          
          }      
        }
    if(count==16)
      {
        if(!status[15]) return;
        status[15] = false;
        dump_byte_array(yuyin_yanan, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
      }
    //瑞金
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == ruijin[i])
        {
          count++;          
          }       
        }
    if(count==16)
      {
        if(!status[0]) return;
        status[0] = false; 
        dump_byte_array(yuyin_ruijin, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
                 
    //突破第三道封锁线
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == tupodisandaofangxian[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[1]) return;
        status[1] = false;
        dump_byte_array(yuyin_tupodisandaofangxian, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
        
    //血战湘江
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == xuezhanxiangjiang[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[2]) return;
        status[2] = false;
        dump_byte_array(yuyin_xuezhanxiangjiang, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
        
    //占领遵义
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == zhanlingzunyi[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[4]) return;
        status[4] = false;
        dump_byte_array(yuyin_zhanlingzunyi, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    //巧渡金沙江
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == qiaodujinshajiang[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[6]) return;
        status[6] = false;
        dump_byte_array(yuyin_qiaodujinshajiang, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    //飞夺泸定桥
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == feiduoludingqiao[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[8]) return;
        status[8] = false;
        dump_byte_array(yuyin_feiduoludingqiao, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    //爬雪山
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == paxueshan[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[9]) return;
        status[9] = false;
        dump_byte_array(yuyin_paxueshan, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    //过草地
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == guocaodi[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[11]) return;
        status[11] = false;
        dump_byte_array(yuyin_guocaodi, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }

    //会宁大会师
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == huiningdahuishi[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[13]) return;
        status[13] = false;
        dump_byte_array(yuyin_huiningdahuishi, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }

    //延安
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == yanan[i])
        {
          count++;          
          }       
        }
    if(count==16)
      {
        if(!status[15]) return;
        status[15] = false;
        dump_byte_array(yuyin_yanan, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
      }
      
      //强渡乌江
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == qiangduwujiang[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[3]) return;
        status[3] = false;
        dump_byte_array(yuyin_qiangduwujiang, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    //四渡赤水
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == siduchishui[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[5]) return;
        status[5] = false;
        dump_byte_array(yuyin_siduchishui, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
            
    //强渡大渡河
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == qiangdudaduhe[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[7]) return;
        status[7] = false;
        dump_byte_array(yuyin_qiangdudaduhe, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    
    //懋功会师
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == maogonghuishi[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[10]) return;
        status[10] = false;  
        dump_byte_array(yuyin_maogonghuishi, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    
    //激战腊子口
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == jizhanlazikou[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        if(!status[12]) return;
        status[12] = false;
        dump_byte_array(yuyin_jizhanlazikou, 7);
        digitalWrite(LED, HIGH);//打开LED
        delay(500);  //延时时间需要适当修改
        digitalWrite(LED, LOW);
        return;
        }
    
    if(isDetect){
      digitalWrite(LED, HIGH);
      delay(2000);  //亮灯500毫秒，延时时间需要适当修改
      digitalWrite(LED, LOW);//关闭LED 
    } 
}

/**
 * hex array output
 * 发送到语音模块
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
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
