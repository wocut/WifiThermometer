
/*
 * 9_把每分钟上传一次改为10分钟一次
 * 8_=7_iphone+7_1
 * 7_iphone 成功更新阿里云上的数据
 * 
 */
//DallasTemperature
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2 // DS18B20 on NodeMCU pin D4 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

//aliyun_mqtt
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "aliyun_mqtt.h"
 
// GPIO 13, D7 on the Node MCU v3
//#define SENSOR_PIN 13
 
#define WIFI_SSID        "${{ secrets.WIFI_SSID  }}"//替换自己的WIFI
#define WIFI_PASSWD      "${{ secrets.WIFI_PASSWD }}"//替换自己的WIFI


 /*
  * 
  * {

}
  */
#define PRODUCT_KEY      "${{ secrets.ProductKey }}" //替换自己的PRODUCT_KEY
#define DEVICE_NAME      "${{ secrets.DeviceName }}" //替换自己的DEVICE_NAME
#define DEVICE_SECRET    "${{ secrets.DeviceSecret }}"//替换自己的DEVICE_SECRET
 
#define DEV_VERSION       "S-TH-WIFI-v1.0-20190220"        //固件版本信息
 
#define ALINK_BODY_FORMAT         "{\"id\":\"123\",\"version\":\"1.0\",\"method\":\"%s\",\"params\":%s}"
#define ALINK_TOPIC_PROP_POST     "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post"
#define ALINK_TOPIC_PROP_POSTRSP  "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/event/property/post_reply"
#define ALINK_TOPIC_PROP_SET      "/sys/" PRODUCT_KEY "/" DEVICE_NAME "/thing/service/property/set"
#define ALINK_METHOD_PROP_POST    "thing.event.property.post"
#define ALINK_TOPIC_DEV_INFO      "/ota/device/inform/" PRODUCT_KEY "/" DEVICE_NAME ""    
#define ALINK_VERSION_FROMA      "{\"id\": 123,\"params\": {\"version\": \"%s\"}}"
unsigned long lastMs = 0;



WiFiClient   espClient;
PubSubClient mqttClient(espClient);
 
void init_wifi(const char *ssid, const char *password)
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi does not connect, try again ...");
        delay(500);
    }
 
    Serial.println("Wifi is connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}
 
void mqtt_callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    payload[length] = '\0';
    Serial.println((char *)payload);
 
    if (strstr(topic, ALINK_TOPIC_PROP_SET))
    {
        StaticJsonBuffer<100> jsonBuffer;
        JsonObject &root = jsonBuffer.parseObject(payload);
        if (!root.success())
        {
            Serial.println("parseObject() failed");
            return;
        }
    }
}
void mqtt_version_post()
{
    char param[512];
    char jsonBuf[1024];
 long CHu=random(15,20);
 
   
    //sprintf(param, "{\"MotionAlarmState\":%d}", digitalRead(13));
    sprintf(param, "{\"id\": 123,\"params\": {\"version\": \"%s\"}}", DEV_VERSION);
   // sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(param);
    mqttClient.publish(ALINK_TOPIC_DEV_INFO, param);
}
void mqtt_check_connect()
{
    while (!mqttClient.connected())//mqttδ����
    {
        while (connect_aliyun_mqtt(mqttClient, PRODUCT_KEY, DEVICE_NAME, DEVICE_SECRET))
        {
            Serial.println("MQTT connect succeed!");
            //client.subscribe(ALINK_TOPIC_PROP_POSTRSP);
            mqttClient.subscribe(ALINK_TOPIC_PROP_SET);
            
            Serial.println("subscribe done");
            mqtt_version_post();
        }
    }
    
}
 
void mqtt_interval_post(int AA)
{
    char param[512];
    char jsonBuf[1024];
 long CHu=random(15,30);
    //sprintf(param, "{\"Data\":\"%d\"}", CHu);
    //sprintf(param, "{\"CurrentHumidity\":\"%d\",\"CurrentTemperature\":12,\"GeoLocation\":{\"CoordinateSystem\":2,\"Latitude\":2,\"Longitude\":123,\"Altitude\":1}}", CHu);
    sprintf(param, "{\"CurrentTemperature\":%d}", AA);
    sprintf(jsonBuf, ALINK_BODY_FORMAT, ALINK_METHOD_PROP_POST, param);
    Serial.println(jsonBuf);
    mqttClient.publish(ALINK_TOPIC_PROP_POST, jsonBuf);
}
 
 
void setup()
{
 int temp_0;
    //pinMode(SENSOR_PIN, INPUT);
    /* initialize serial for debugging */
    Serial.begin(115200);
   DS18B20.begin();
   
   Serial.println("Testing Dual Sensor data");
   
    Serial.println("Demo Start");
 
    init_wifi(WIFI_SSID, WIFI_PASSWD);
 
    mqttClient.setCallback(mqtt_callback);
    mqtt_interval_post(temp_0);
}
 
// the loop function runs over and over again forever
void loop()
{
    int temp_0;
    DS18B20.requestTemperatures(); 
  temp_0 = DS18B20.getTempCByIndex(0); // Sensor 0 will capture Temp in Celcius
    if (millis() - lastMs >= 60000)
    {
        lastMs = millis();
        mqtt_check_connect();
        /* Post */        
        mqtt_interval_post(temp_0);
    }
 
    mqttClient.loop();
 
    unsigned int WAIT_MS = 10000;

        Serial.println("Motion detected!");
    
    delay(WAIT_MS); // ms
    Serial.println(millis() / WAIT_MS);
}