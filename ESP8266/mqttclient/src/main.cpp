#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

#define DEBUG Serial
#define HARDSERIAL Serial
//默认端口1883
#define MQTTPORT 1883

const char *mqtt_server = "101.133.235.188";
const char *getSunTopic = "getSunMsg";
const char *angleTopic = "angle";
const char *sunMsgTopic = "sunMsg";

WiFiClient espclient;
PubSubClient client(espclient);

Ticker timer;

void requireSun(){
  //TODO:填充数据
  //格式     "lat,lon,alt"  纬度，经度，海拔 其中alt必须为int类型，三个参数以英文,分隔
  //参考如下
  client.publish(getSunTopic,"30.2,120.0,10");
}

void angleControl(String angle)
{
  //TODO: 以下替换成USMART控制角度
  HARDSERIAL.println(angle);
}

void autoControl(float solar_elevation_angle, float solar_azimuth_angle)
{
  //TODO:替换USMART 服务器不提供坡度
  HARDSERIAL.printf("%.2f,%.2f\r\n", solar_elevation_angle, solar_azimuth_angle);
}

void handleSunMsg(String Msg)
{
  float solar_elevation_angle;
  float solar_azimuth_angle;
  int index = Msg.indexOf(',');
  String a = Msg.substring(0, index);
  String b = Msg.substring(index + 1);
  solar_elevation_angle = a.toFloat();
  solar_azimuth_angle = b.toFloat();
  autoControl(solar_elevation_angle, solar_azimuth_angle);
}
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

String char2String(char *chars)
{
  return String("") + chars;
}

/*
 * mqtt回调函数
 * 信息接收
 */
void callback(char *topic, byte *payload, unsigned int length)
{
  // DEBUG.print("Message arrived [");
  // DEBUG.print(topic);
  // DEBUG.print("] ");
  // for (int i = 0; i < length; i++)
  // {
  //   DEBUG.print((char)payload[i]);
  // } //串口打印出收到的信息
  // DEBUG.println();
  int code = 0;
  payload[length] = '\0';
  String message = char2String((char *)payload);
  //TODO:之后添加其他查询支持
  if (0 == strcmp(topic, angleTopic))
    code = 1;
  if (0 == strcmp(topic, sunMsgTopic))
    code = 2;
  switch (code)
  {
  case 1:
    angleControl(message);
    break;
  case 2:
    handleSunMsg(message);
    break;
  case 0:
  default:
    break;
  }
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
      //FIXME:记得更改
      client.subscribe(angleTopic); //这个是你让板子订阅的主题（接受该主题的消息）
      client.subscribe(sunMsgTopic);
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
  timer.attach_ms(1000,requireSun);
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
  // if (DEBUG.available())
  // {
  //   String msg = DEBUG.readString();
  //   msg.replace("\r\n", "");
  //   client.publish(pubTopic, msg.c_str());
  // }
  //digitalWrite(LED_BUILTIN, LOW);
  //delay(200);
}