

//12,OK,print加ln可以通过
//13  取自然数
//14  定义函数
/*
 * 17
 * 18 把代码优化了一下
 */
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SoftwareSerial.h>
 //int ID=0;
int second=0,minute=0; 
  bool send=1;
  bool timeflag=1;
  
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
  //Serial.println("Dallas Temperature IC Control Library Demo");
  //Serial.println(mySerial.read());
  
 mySerial.begin(9600);//因单片机无法支持115200
                      //需要将esp8266的波特率改为9600
                      //发送AT指令AT+CIOBAUD=9600 若成功则返回ok，否则返回error
 //控制ESP8266
 mySerial.println("AT+CWMODE=2"); 
 delay(500); 
  mySerial.println("AT+RST"); 
  delay(500);
  sensors.begin(); // 初始库
  }






void loop(void)
{ 
   second++;
  if (second>=60)
    {
      second=0;
      minute++;
    }
  if (minute>2)    //定时2分钟
    {
      timeflag=1;
      minute=0;
    }


  if ((send&&timeflag)==1)                    //判断
  {
      
     sensors.requestTemperatures(); // 发送命令获取温度
   int RealTemp=sensors.getTempCByIndex(0);
   //int RealTemp=25;
  String IDa ="AT+CWSAP=\"";
  int IDb;
  int MenberTemp=-100;
  if (abs(RealTemp-MenberTemp)>1)    //如果温度比之前相差超过2度
      {
       IDb=RealTemp;       //更新温度
       MenberTemp=RealTemp;//更新临时记录的温度
       send=1;        //启动wifi名称更新程序   
       
      }
  else
      {
        IDb=MenberTemp;
        //send=0;        //关闭wifi名称更新程序
        //send=1;        //临时试验新UNO
      }

  String IDc ="c\",\"12312312\",11,0";
  //String IDc ="c\"";
  String ID=IDa+IDb+IDc;
   //String ID="AT+CWSAP=\"baidc\",\"12K456u8\",11,3";   //临时试验新UNO 

      mySerial.println(ID);     //wifi名称更新程序
      
      
      //delay(1000);
      //mySerial.println("AT+RST");     //重启
      //delay(1000);
      send=0;              //关闭wifi名称更新程序
      timeflag=0; 
 
      
      //Serial.println(ID);
  }

//      Serial.print("M:");
//      Serial.print(minute );
//      Serial.print("S:");
//      Serial.print(second );
//      Serial.print("time:");
//      Serial.print(timeflag );
//      Serial.print("temperture:");
//      Serial.print(send );
//      Serial.print("RealTemp=");
//      Serial.print(RealTemp);
//      Serial.print("MenberTemp=");
//      Serial.println(MenberTemp);
//  mySerial.println(ID);
//  mySerial.println(IDb);
//  Serial.println(ID);

/*
 * 串口打印程序
 while(mySerial.available()){
    Serial.write(mySerial.read());
     }
  delay(1000);
  }
 */
  
}
