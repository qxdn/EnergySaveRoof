package com.qianxu.mqttclients;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;

public class MQTTService extends Service {
    public MQTTService() {
    }

    @Override
    public IBinder onBind(Intent intent) {
        // TODO: Return the communication channel to the service.
        throw new UnsupportedOperationException("Not yet implemented");
    }
}
