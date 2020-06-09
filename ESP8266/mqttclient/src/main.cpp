#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <ArduinoJson.h>

#define DEBUG Serial
#define HARDSERIAL Serial
//默认端口1883
#define MQTTPORT 1883

const char *mqtt_server = "101.133.235.188";
const char *getSunTopic = "getSunMsg";
const char *elecTopic = "electric";
const char *getElecTopic = "getElectric";
const char *roofTopic = "roof";
const char *getRoofTopic = "getRoof";
const char *willTopic = "hardwareState";
const char *angleTopic = "angle";
const char *sunMsgTopic = "sunMsg";
const char *emergencyTopic = "emergency";
const char *windowsTopic = "windows";
const char *smartTopic = "smart";
String clinetId = "esp8266client";

WiFiClient espclient;
PubSubClient client(espclient);

Ticker timer1;
Ticker timer2;

void requireSun()
{
  //TODO:填充数据  2分钟一次  不提供关闭
  //格式     "lat,lon,alt"  纬度，经度，海拔 其中alt必须为int类型，三个参数以英文,分隔
  //参考如下 更改"30.2,120.0,10"
  client.publish(getSunTopic, "30.2,120.0,10");
}

void returnState()
{
  //TODO:填充数据  60s一次  不提供关闭
  //worked  正常工作：true 异常工作:false
  //smarted   智能控制:true   非智能控制false
  StaticJsonDocument<200> doc;
  doc["worked"] = true;
  doc["smarted"] = true;
  char JSONmessageBuffer[100];
  serializeJson(doc, JSONmessageBuffer);
  client.publish(willTopic, JSONmessageBuffer, true);
}

void returnElec()
{
  //TODO: 对应app第三面  发电参数  各个参数见下 填充数据
  StaticJsonDocument<500> doc;
  //日均发电
  doc["averageDayElectric"] = 13.3f;
  //全天发电
  doc["allDayElectric"] = 20.3f;
  //近一周发电
  doc["weeklyElectric"] = 20.3f;
  //上小时发电
  doc["lastHourElectric"] = 0.31f;
  //一周内每天发电  7个
  JsonArray array = doc.createNestedArray("aWeekElectrics");
  // for (int i = 0; i < 7; i++)
  // {
  //   array.add((float)(random(20) + 50));
  // }
  array.add(0.95f);
  array.add(0.89f);
  array.add(1.21f);
  array.add(1.09f);
  array.add(1.67f);
  array.add(1.79f);
  array.add(1.71f);
  char JSONmessageBuffer[500];
  serializeJson(doc, JSONmessageBuffer);
  client.publish_P(elecTopic, JSONmessageBuffer, false);
}

void returnRoof()
{
  //TODO: app第二面  屋顶参数  各个参数见下 填充数据
  StaticJsonDocument<500> doc;
  //屋顶特性
  doc["electricState"] = "正常";
  //工作时间
  doc["runtime"] = "6小时55分";
  JsonArray array = doc.createNestedArray("windowsStates");
  //5扇窗户 各个的状态
  for (int i = 0; i < 5; i++)
  {
    JsonObject object = array.createNestedObject();
    //开关状态 true打开  false 关闭
    object["switchState"] = true;
    //是否正常
    object["workState"] = true;
  }
  char JSONmessageBuffer[500];
  serializeJson(doc, JSONmessageBuffer);
  client.publish_P(roofTopic, JSONmessageBuffer, false);
}

void emergency()
{
  //TODO: app第一面 一键应急
  HARDSERIAL.println("一键应急");
  //最好可以返回当前状态
  //returnState();
}

void openWindows(int index, int open)
{
  //TODO: app第三面 打开窗户  index 0-4窗户 open 1:打开  0:关闭
  HARDSERIAL.printf("%d,%d\r\n", index, open);
  //最好可以返回当前状态
  //returnState();
}

void angleControl(String angle)
{
  //TODO: app第三面 开发者功能 以下替换成USMART控制角度
  HARDSERIAL.println(angle);
}
void autoControl(float solar_elevation_angle, float solar_azimuth_angle)
{
  //TODO:替换USMART 自动控制  服务器不提供坡度
  HARDSERIAL.printf("%.2f,%.2f\r\n", solar_elevation_angle, solar_azimuth_angle);
}

void smartControl(int isSmart)
{
  //TODO: 智能控制 对应第一面智能控制   isSmart为1则智能控制 0为手动
  HARDSERIAL.println(isSmart);
  //最好可以返回当前状态
  //returnState();
}


/******************************************************************/

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

void handleWindows(String Msg)
{
  int index = Msg.indexOf(',');
  String a = Msg.substring(0, index);
  String b = Msg.substring(index + 1);
  int windowsIndex = a.toInt();
  int isopen;
  if (b == "false")
  {
    isopen = 0;
  }
  else
  {
    isopen = 1;
  }
  openWindows(windowsIndex, isopen);
}

void handleSmart(String msg)
{
  int isSmart;
  if (msg == "false")
  {
    isSmart = 0;
  }
  else
  {
    isSmart = 1;
  }
  smartControl(isSmart);
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
  int code = 0;
  payload[length] = '\0';
  String message = char2String((char *)payload);
  //TODO:之后添加其他查询支持  可以更换ascii switch
  if (0 == strcmp(topic, angleTopic))
    code = 1;
  else if (0 == strcmp(topic, sunMsgTopic))
    code = 2;
  else if (0 == strcmp(topic, getElecTopic))
    code = 3;
  else if (0 == strcmp(topic, getRoofTopic))
    code = 4;
  else if (0 == strcmp(topic, emergencyTopic))
    code = 5;
  else if (0 == strcmp(topic, windowsTopic))
    code = 6;
  else if (0 == strcmp(topic, smartTopic))
    code = 7;

  switch (code)
  {
  case 1:
    angleControl(message);
    break;
  case 2:
    handleSunMsg(message);
    break;
  case 3:
    returnElec();
    break;
  case 4:
    returnRoof();
    break;
  case 5:
    emergency();
    break;
  case 6:
    handleWindows(message);
    break;
  case 7:
    handleSmart(message);
    break;
  case 0:
  default:
    break;
  }
}

void reconnect()
{
  //FIXME:可能导致拍视频的问题,也就是不差硬件的时候拍视频会显示不正常工作，一般插了硬件就行
  //设置lastwill
  StaticJsonDocument<200> doc;
  doc["worked"] = false;
  doc["smarted"] = false;
  char JSONmessageBuffer[100];
  serializeJson(doc, JSONmessageBuffer);
  // Loop until we're reconnected
  while (!client.connected())
  {
    DEBUG.print("Attempting MQTT connection...");
    //尝试连接
    if (client.connect(clinetId.c_str(), willTopic, 0, true, JSONmessageBuffer))
    {
      DEBUG.println("connected");
      doc["worked"] = true;
      doc["smarted"] = true;
      serializeJson(doc, JSONmessageBuffer);
      client.publish(willTopic, JSONmessageBuffer, true);
      // 连接后，发布公告......
      //client.publish(pubTopic, "hello world"); //链接成功后 会发布这个主题和语句
      // ......并订阅
      client.subscribe(angleTopic); //这个是你让板子订阅的主题（接受该主题的消息）
      client.subscribe(sunMsgTopic);
      client.subscribe(getElecTopic);
      client.subscribe(getRoofTopic);
      client.subscribe(emergencyTopic);
      client.subscribe(windowsTopic);
      client.subscribe(smartTopic);
      //获取一次天气
      requireSun();
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
  HARDSERIAL.begin(115200);
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
  //TODO: 时间控制 单位秒
  //获取天气时间
  timer1.attach(600, requireSun);
  //返回是否正常工作
  timer2.attach(60, returnState);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
}