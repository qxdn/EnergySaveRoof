package com.qianxu.mqttclients;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import org.eclipse.paho.android.service.MqttAndroidClient;
import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;


public class MQTTService extends Service implements MQTTServiceInterface {

    private MqttConnectOptions mqttConnectOptions;
    private MqttAndroidClient client;
    private String subTopic;
    private String pubTopic;
    //本地广播
    private LocalBroadcastManager localBroadcastManager;

    public MQTTService() {
    }

    @Override
    public void onCreate() {
        super.onCreate();
        //注册本地广播
        localBroadcastManager=LocalBroadcastManager.getInstance(this);
    }

    //绑定
    public class MQTTBinder extends Binder{
        public MQTTService getService(){
            return MQTTService.this;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return new MQTTBinder();
    }


    //MQTT初始化
    @Override
    public void Init(MqttSetting mqttSetting) {
        //订阅的主题
        this.subTopic=mqttSetting.getSubTopic();
        this.pubTopic=mqttSetting.getPubTopic();
        //封装好Mqtt client
        client=new MqttAndroidClient(getApplicationContext(),mqttSetting.getUrl(),mqttSetting.getClientId());
        //设置回调函数
        //TODO:
        client.setCallback(new MqttCallback() {
            //连接断开
            @Override
            public void connectionLost(Throwable cause) {
                Intent intent=new Intent("com.qianxu.mqttService.connectLost");
                localBroadcastManager.sendBroadcast(intent);
            }

            //消息到来
            @Override
            public void messageArrived(String topic, MqttMessage message) throws Exception {
                Intent intent=new Intent("com.qianxu.mqttService.messageArrived");
                intent.putExtra("Topic",topic);
                intent.putExtra("message",new String(message.getPayload()));
                localBroadcastManager.sendBroadcast(intent);
            }

            @Override
            public void deliveryComplete(IMqttDeliveryToken token) {
                Toast.makeText(getApplicationContext(),"发送成功",Toast.LENGTH_LONG);
            }
        });
        //设置连接参数
        mqttConnectOptions=new MqttConnectOptions();
        //设置自动重连
        mqttConnectOptions.setAutomaticReconnect(true);
        // 设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录
        // 这里设置为true表示每次连接到服务器都以新的身份连接
        mqttConnectOptions.setCleanSession(true);
        //设置用户名
        mqttConnectOptions.setUserName(mqttSetting.getUserName());
        //设置密码
        mqttConnectOptions.setPassword(mqttSetting.getPassword().toCharArray());
        // 设置超时时间 单位为秒
        mqttConnectOptions.setConnectionTimeout(10);
        // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
        mqttConnectOptions.setKeepAliveInterval(20);
        //TODO:设置遗嘱
    }

    @Override
    public boolean isConnected() {
        if(client!=null){
            return client.isConnected();
        }else {
            return false;
        }
    }

    @Override
    public void Connect() {
        if (!client.isConnected()) {
            try {
                client.connect(mqttConnectOptions,null, iMqttActionListener);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     *  MQTT是否连接成功
     */
    private IMqttActionListener iMqttActionListener = new IMqttActionListener() {

        @Override
        public void onSuccess(IMqttToken arg0) {
            Log.i("MQTT", "mqtt connect success ");
            subscribe();
        }

        @Override
        public void onFailure(IMqttToken arg0, Throwable arg1) {
            Log.i("MQTT", "mqtt connect failed ");
        }
    };

    /**
     * 发布主题
     * @param msg
     */
    @Override
    public void Publish(String msg) {
        //设置优先级
        Integer qos = 0;
        //是否被服务器保留
        Boolean retained = false;
        try {
            if (client != null){
                client.publish(pubTopic, msg.getBytes(), qos.intValue(), retained.booleanValue());
            }
        } catch (MqttException e) {
            Log.e("MQTT","Publish Fail");
        }
    }

    /**
     * 关闭
     */
    @Override
    public void Close() {
        if(client!=null){
           client.close();
        }
    }

    /**
     * 更新设置
     * @param mqttSetting
     */
    @Override
    public void updateSetting(MqttSetting mqttSetting) {
        boolean connected=false;
        if(isConnected()){
            disconnect();
            connected=true;
        }
        Init(mqttSetting);
        if(connected) {
            Connect();
        }
    }

    /**
     * 订阅主题
     */
    private void subscribe() {
        try {
            client.subscribe(subTopic,1);
        } catch (MqttException ex) {
           Log.e("MQTT",ex.toString());
        }
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        if((client!=null)&&client.isConnected()){
            client.close();
        }
    }

    /**
     * 断开连接
     */
    @Override
    public void disconnect(){
        if(client!=null){
            try {
                client.disconnect();
            } catch (MqttException e) {
                e.printStackTrace();
            }
        }
    }

}
