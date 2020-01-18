package com.qianxu.mqttclients;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class MQTTService extends Service implements MQTTServiceInterface {

    private String host="";
    private String userName="";
    private String password="";
    private String topic="";
    private String clientId="androidId";


    public MQTTService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }


    @Override
    public void sendMessage(String Topic, String msg) {

    }
}
