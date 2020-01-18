#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DEBUG Serial
//默认端口1883
#define MQTTPORT 1883

const char *mqtt_server = "192.168.124.8";
const char *pubTopic="outTopic";
const char *subTopic="inTopic";


WiFiClient espclient;
PubSubClient client(espclient);
/*
 *自动连接
 */
bool autoConfig()
{
  WiFi.begin();
  for (int i = 0; i < 20; i++)
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
      DEBUG.println("AutoConfig Success");
      DEBUG.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      DEBUG.printf("PassWord:%s\r\n", WiFi.psk().c_str());
      delay(2000);
      DEBUG.print("IP:");
      DEBUG.println(WiFi.localIP());
      WiFi.printDiag(DEBUG);
      return true;
    }
    else
    {
      DEBUG.print("AutoConfig Waiting......");
      DEBUG.println(wstatus);
      delay(1000);
    }
  }
  DEBUG.println("AutoConfig Faild!");
  return false;
}
/*
 *smartconfig
 */
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  DEBUG.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {
    DEBUG.print(".");
    if (WiFi.smartConfigDone())
    {
      DEBUG.println("SmartConfig Success");
      DEBUG.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      DEBUG.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.setAutoConnect(true); // 设置自动连接
      break;
    }
    delay(1000); // 这个地方一定要加延时，否则极易崩溃重启
  }
}

/*
 * mqtt回调函数
 * 信息接收
 */
void callback(char *topic, byte *payload, unsigned int length)
{
  DEBUG.print("Message arrived [");
  DEBUG.print(topic);
  DEBUG.print("] ");
  for (int i = 0; i < length; i++)
  {
    DEBUG.print((char)payload[i]);
  } //串口打印出收到的信息
  DEBUG.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    DEBUG.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";      //该板子的链接名称
    clientId += String(random(0xffff), HEX); //产生一个随机数字 以免多块板子重名
    //尝试连接
    if (client.connect(clientId.c_str()))
    {
      DEBUG.println("connected");
      // 连接后，发布公告......
      //client.publish(pubTopic, "hello world"); //链接成功后 会发布这个主题和语句
      // ......并订阅
      client.subscribe(subTopic); //这个是你让板子订阅的主题（接受该主题的消息）
    }
    else
    {
      DEBUG.print("failed, rc=");
      DEBUG.print(client.state());
      DEBUG.println(" try again in 5 seconds");
      // 如果链接失败 等待五分钟重新链接
      delay(5000);
    }
  }
}

void setup()
{
  //Log串口初始化
  DEBUG.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  if (!autoConfig())
  {
    //自动连接失败启动smartconfig
    DEBUG.println("Start module");
    smartConfig();
  }
  client.setServer(mqtt_server, 1883);
  //接收回调
  client.setCallback(callback);
}

void loop()
{
  //digitalWrite(LED_BUILTIN, HIGH);
  // delay(200);
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (DEBUG.available())
  {
    String msg = DEBUG.readString();
    msg.replace("\r\n","");
    client.publish(pubTopic, msg.c_str());
  }
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(200);
}