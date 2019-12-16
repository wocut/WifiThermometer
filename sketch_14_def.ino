

//12,OK,print加ln可以通过
//13  取自然数
//14  定义函数
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
 int ID=0;
int second=0,minute=0; 
  bool send=0;
  bool timeflag=0;
  
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
  //Serial.println(mySerial.read());
 mySerial.begin(9600);

 //控制ESP8266
 mySerial.println("AT+CWMODE=2"); 
 delay(500); 
  mySerial.println("AT+RST"); 
  delay(500);
  sensors.begin(); // 初始库
  }
    int MenberTemp=-100;
void loop(void)
{ 

   sensors.requestTemperatures(); // 发送命令获取温度
   float RealTemp=sensors.getTempCByIndex(0);
  String IDa ="AT+CWSAP_DEF=\"";
  int IDb;

  if (abs(RealTemp-MenberTemp)>1)    //如果温度比之前相差超过2度
      {
       IDb=RealTemp;       //更新温度
       MenberTemp=RealTemp;//更新临时记录的温度
       send=1;        //启动wifi名称更新程序
      }
  else
      {
        IDb=MenberTemp;
        send=0;        //关闭wifi名称更新程序
      }

  String IDc ="c\",\"12K456u8\",11,3";
  //String IDc ="c\"";
  String ID=IDa+IDb+IDc;

  second++;
  if (second>=60p[[ )
    {
      second=0;
      minute++;
    }
  if (minute>=10)
    {
      timeflag=1;
      minute=0;
    }

  if ((send&&timeflag)==1)                    //判断
  {
      mySerial.println(ID);     //wifi名称更新程序

      //delay(1000);
      send=0;              //关闭wifi名称更新程序
      timeflag=0; 
      //Serial.println(ID);
  }

      Serial.print(minute );
      Serial.print(second );

      Serial.print(timeflag );
      Serial.print(send );
      Serial.print("RealTemp=");
      Serial.print(RealTemp);
      Serial.print("MenberTemp=");
      Serial.println(MenberTemp);
  //mySerial.println(ID);
  //mySerial.println(IDb);
  //Serial.println(ID);
  
  while(mySerial.available()){
    
    Serial.write(mySerial.read());
     }
     
   

  delay(1000);

  }
