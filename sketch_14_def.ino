//12,OK,print加ln可以通过
//13  取自然数
//14  定义函数
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
 int ID=0;

// 定义DS18B20数据口连接arduino的3号IO上
#define ONE_WIRE_BUS 3

// 初始连接在单总线上的单总线设备
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
  SoftwareSerial mySerial(10, 11); // RX, TX ESP8266就连在这个上面

void setup(void)
{
  // 设置串口通信波特率
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");
 mySerial.begin(9600);

 //控制ESP8266
 mySerial.println("AT+CWMODE=2"); 
 delay(500); 
  mySerial.println("AT+RST"); 
  delay(500);
  sensors.begin(); // 初始库
  }
 
void loop(void)
{ 
  
   sensors.requestTemperatures(); // 发送命令获取温度
   float IDb2=sensors.getTempCByIndex(0);
  String IDa ="AT+CWSAP_DEF=\"";
  int IDb=IDb2;
  String IDc ="c\",\"12K456u8\",11,3";
  String ID=IDa+IDb+IDc;
  mySerial.println(ID);
  
  while(mySerial.available()){
    Serial.write(mySerial.read());
     }
  delay(6000);
  }
  
  
    
