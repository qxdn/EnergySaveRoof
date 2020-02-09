package com.qianxu.mqtt;

import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.InputStreamReader;
import java.util.Properties;

public class MyMqttClientImpl implements MyMqttClient {

    private Logger logger= LoggerFactory.getLogger(this.getClass());

    private MqttClient client;
    private MqttConnectOptions options_sub;

    private String sub_topic;
    private String pub_topic;

    @Override
    public void init(MqttCallback mqttCallback) {
        logger.debug("初始化Mqtt相关参数");
        Properties pro = new Properties();
        try {
            pro.load(new InputStreamReader(this.getClass().getResourceAsStream("/mqtt.properties"), "UTF-8"));
            // HOST_MQ为主机名，clientid即连接MQTT的客户端ID，一般以唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(pro.getProperty("mqtt.tcp.host").trim(), pro.getProperty("mqtt.clientid").trim(), new MemoryPersistence());
            // MQTT的连接设置
            options_sub = new MqttConnectOptions();
            // 设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，设置为true表示每次连接到服务器都以新的身份连接
            options_sub.setCleanSession(false);
            // 设置连接的用户名
            options_sub.setUserName(pro.getProperty("mqtt.username").trim());
            // 设置连接的密码
            options_sub.setPassword(pro.getProperty("mqtt.password").trim().toCharArray());
            // 设置会话心跳时间 单位为秒 服务器会每隔90秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options_sub.setKeepAliveInterval(90);

            //设置回调
            client.setCallback(mqttCallback);

            this.sub_topic=pro.getProperty("mqtt.subtopic").trim();
            this.pub_topic=pro.getProperty("mqtt.pubtopic").trim();

            //连接
            client.connect(options_sub);

            //订阅
            client.subscribe(sub_topic,0);

        }catch (Exception e){
            logger.error(e.toString());
            e.printStackTrace();
        }

    }

    @Override
    public void sendMessage(String msg) {
        sendMessage(pub_topic,msg);
    }

    @Override
    public void sendMessage(String topic, String msg) {
        try {
            MqttMessage message = new MqttMessage();
            message.setQos(1);
            message.setRetained(true);
            message.setPayload(msg.getBytes());
            MqttTopic mqttTopic = client.getTopic(topic);
            MqttDeliveryToken token = mqttTopic.publish(message);//发布主题
            token.waitForCompletion();
        }catch (MqttPersistenceException e) {
            logger.error(e.toString());
        } catch (MqttException e) {
            logger.error(e.toString());
        }
    }

    @Override
    public void subscribe(String[] topic, int[] qos) {
        try {
            client.subscribe(topic, qos);
        } catch (MqttException e) {
            logger.error(e.toString());
        }// 订阅主题
    }
    @Override
    public void subscribe(String topic, int qos) {
        try {
            client.subscribe(topic, qos);
        } catch (MqttException e) {
            logger.error(e.toString());
        }// 订阅主题
    }

    @Override
    public void subscribe() {
        subscribe(sub_topic,0);
    }

    @Override
    protected void finalize() throws Throwable {
        super.finalize();
        if(client!=null)
            client.close();
    }
}
