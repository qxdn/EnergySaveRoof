package com.qianxu;

import com.qianxu.mqtt.MyMqttClient;
import com.qianxu.mqtt.MyMqttClientImpl;
import com.qianxu.weather.Weather;
import com.qianxu.weather.WeatherImpl;

import org.apache.http.HttpEntity;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClientBuilder;
import org.apache.http.util.EntityUtils;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Hello world!
 *
 */
public class App {
    public static void main(String[] args) {
        Logger log = LoggerFactory.getLogger(App.class);

       Weather weather = new WeatherImpl();
        weather.init();
        System.out.println(weather.getWeather("武汉"));
        //System.out.println(weather.getSolar(30.59,114.30,24));
//
//        MyMqttClient client=new MyMqttClientImpl();
//        client.init(new MqttCallback() {
//            @Override
//            public void connectionLost(Throwable cause) {
//                log.debug("连接关闭");
//            }
//
//            @Override
//            public void messageArrived(String topic, MqttMessage message) throws Exception {
//                System.out.println("topic:"+topic+",msg:"+message.toString());
//            }
//
//            @Override
//            public void deliveryComplete(IMqttDeliveryToken token) {
//
//            }
//        });
//        client.sendMessage("sas");
//        while (true){
//
//        }
    }
}
