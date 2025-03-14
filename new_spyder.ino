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
#include"daka.h"
#include"yvyin.h"

//TODO:出场默认但是初赛点
//TODO:出场默认但是初赛点
//TODO:出场默认但是初赛点

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

/**
 * 初始化函数
 */
const int IN1 = 9, IN2 = 10, STBY = 11, PWM = 8;
void setup() {
    pinMode(13, OUTPUT);
    pinMode( LED, OUTPUT);  //设置管脚为输出
    pinMode( IN1, OUTPUT); 
    pinMode( IN2, OUTPUT); 
    pinMode( PWM, OUTPUT);
    pinMode( STBY, OUTPUT); 
    digitalWrite(LED, LOW);//关闭LED
    
    digitalWrite(STBY, HIGH);
    digitalWrite(PWM, HIGH);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    
    
    Serial.begin(9600); // 波特率9600
    //while (!Serial);    // 等待打开串口
    
   //初始化软串口通信；
   softSerial1.begin(9600);
   //监听软串口通信
   softSerial1.listen();
   delay(30);
   //读卡器自动读数据模式
   dump_byte_array1(auto_read_config, 15);
   delay(30);
   //读卡器秘钥设置，系统默认秘钥A：FFFFFFFFFFFF  秘钥B：FFFFFFFFFFFF  
   dump_byte_array1(key_Setting, 23);
   //dump_byte_array(yuyin_voiceSetting, 6);
  
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
  delay(3000);
  dump_byte_array(yuyin_ruijin, 7);
}

/**
 * Main主循环函数
 */
void loop() {    
   digitalWrite(13, HIGH);
   byte ReceiveData[18]; // 接收数据数组
   byte size = sizeof(ReceiveData);
   byte count = 0;
   memset(ReceiveData, 0, sizeof(ReceiveData));

  //读取从设备A传入的数据，并在串口监视器中显示  
  if(softSerial1.available()>0)
  { 
    digitalWrite(LED, HIGH);//打开LED  
    memset(ReceiveData, 0, sizeof(ReceiveData));
    byte IndexMark = 0;
    //Serial.println(IndexMark);
    while (softSerial1.available())
    {
      delay(1);
      ReceiveData[IndexMark] = softSerial1.read();
      Serial.print(ReceiveData[IndexMark]);
      IndexMark++;
    }
    Serial.print('\n');    
  }

    //判断从标签里读到的信息是什么
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
         
        dump_byte_array(yuyin_ruijin, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_tupodisandaofangxian, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_xuezhanxiangjiang, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_qiangduwujiang, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_zhanlingzunyi, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_siduchishui, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_qiaodujinshajiang, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_qiangdudaduhe, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_feiduoludingqiao, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_paxueshan, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_maogonghuishi, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_guocaodi, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_jizhanlazikou, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_huiningdahuishi, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
        }
            
    //吴起镇会议
    count = 0;
    for (byte i = 0; i < 16; i++) {
        if(ReceiveData[i+11] == wuqizhenhuiyi[i])
        {
          count++;          
          }       
        } 
    if(count==16)
      {
        dump_byte_array(yuyin_wuqizhenhuiyi, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
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
        dump_byte_array(yuyin_yanan, 7);
        //digitalWrite(LED, HIGH);//打开LED
        //delay(500);  //延时时间需要适当修改
        }
                     
    


    delay(500);  //亮灯500毫秒，延时时间需要适当修改
    digitalWrite(LED, LOW);//关闭LED 
    digitalWrite(13, LOW); 
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
