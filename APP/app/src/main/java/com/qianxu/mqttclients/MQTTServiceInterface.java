package com.qianxu.mqttclients;


public interface MQTTServiceInterface {
    public void Init(MqttSetting mqttSetting);
    public boolean isConnected();
    public void Connect();
    public void Publish(String msg);
    public void Close();
    public void updateSetting(MqttSetting mqttSetting);
    public void disconnect();
}
