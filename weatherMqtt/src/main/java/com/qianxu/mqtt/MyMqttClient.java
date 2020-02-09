package com.qianxu.mqtt;


import org.eclipse.paho.client.mqttv3.MqttCallback;

public interface MyMqttClient{
    public void init(MqttCallback mqttCallback);
    public void sendMessage(String msg);
    public void sendMessage(String topic,String msg);
    public void subscribe(String[] topic,int[] qos);
    public void subscribe(String topic,int qos);
    public void subscribe();
}