#include <Arduino.h>
#include <ESP8266WiFi.h>

#define DEBUG Serial


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
  DEBUG.println("AutoConfig Faild!" );
  return false;
}
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
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    delay(1000); // 这个地方一定要加延时，否则极易崩溃重启
  }
}

void setup()
{
  //Log串口初始化
  DEBUG.begin(115200);
  if (!autoConfig())
  {
    //自动连接失败启动smartconfig
    DEBUG.println("Start module");
    smartConfig();
  }
}

void loop()
{
}