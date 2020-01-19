package com.qianxu.mqttclients;


import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;

import android.os.IBinder;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.widget.SwitchCompat;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import com.google.android.material.navigation.NavigationView;


public class MainActivity extends BasicActivity {

    private TextView light,water,pm,textIntent;
    private EditText angle;
    private Button sendButton;
    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ImageView home,imageIntent;
    private SwitchCompat switchConnect;

    private MqttSetting mqttSetting;

    private MQTTService mqttService;

    private ServiceConnection connection=new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            mqttService=((MQTTService.MQTTBinder)service).getService();
            //读取数据
            ReadData();
            mqttService.Init(mqttSetting);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }
    };


   private LocalBroadcastManager localBroadcastManager;
   private LocalReceiver localReceiver;

    private void findView(){
        drawerLayout=(DrawerLayout)findViewById(R.id.drawer_layout);
        navigationView=(NavigationView)findViewById(R.id.nav);

        home=(ImageView)findViewById(R.id.home);

        light=(TextView)findViewById(R.id.light);
        water = (TextView) findViewById(R.id.water);
        pm=(TextView)findViewById(R.id.PM);
        angle=(EditText)findViewById(R.id.angle);
        sendButton=(Button)findViewById(R.id.sendButton);

        textIntent=(TextView)findViewById(R.id.textIntent);
        imageIntent=(ImageView)findViewById(R.id.imageIntent);
        switchConnect=(SwitchCompat) findViewById(R.id.switchConnect);
    }

    private void setListener(){
        //发送按键
        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(mqttService.isConnected()){
                    //TODO:
                    mqttService.Publish(String.format("UsmartChangeMotorAngle(\"%s\")\r\n",angle.getText()));
                }else {
                    Toast.makeText(getApplicationContext(),"尚未连接",Toast.LENGTH_LONG).show();
                }
            }
        });
        //主菜单
        home.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                drawerLayout.openDrawer(GravityCompat.START);
            }
        });
        navigationView.setCheckedItem(R.id.setting);
        navigationView.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                switch (item.getItemId()){
                    case R.id.setting:
                        Intent intent=new Intent(MainActivity.this,SettingActivity.class);
                        startActivity(intent);
                        break;
                    case R.id.HowToUse:
                        Intent intent1=new Intent(MainActivity.this,HowToUseActivity.class);
                        startActivity(intent1);
                        break;
                    case R.id.about:
                        Intent intent2=new Intent(MainActivity.this,AboutActivity.class);
                        startActivity(intent2);
                        break;
                        default:break;
                }
                return false;
            }
        });
        switchConnect.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    mqttService.Connect();
                    imageIntent.setImageResource(R.drawable.connect);
                    textIntent.setText(R.string.isConnected);
                }else {
                    mqttService.disconnect();
                    imageIntent.setImageResource(R.drawable.notconnect);
                    textIntent.setText(R.string.isNotConnected);
                }
            }
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Intent bindIntent=new Intent(this,MQTTService.class);
        bindService(bindIntent,connection,BIND_AUTO_CREATE);

        //找到View
        findView();

        setListener();

        localBroadcastManager=LocalBroadcastManager.getInstance(this);
        IntentFilter intentFilter=new IntentFilter();
        intentFilter.addAction("com.qianxu.mqttService.messageArrived");
        intentFilter.addAction("com.qianxu.mqttService.connectLost");
        localReceiver=new LocalReceiver();
        localBroadcastManager.registerReceiver(localReceiver,intentFilter);
    }

    @Override
    protected void onResume() {
        super.onResume();
        ReadData();
        if(mqttService!=null) {
            mqttService.updateSetting(mqttSetting);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(connection);
        if(mqttService.isConnected()){
            mqttService.Close();
        }
    }

    private void ReadData(){
        String host,port,PubTopic,SubTopic,ClientId,username,password;
        SharedPreferences pref=getSharedPreferences("keyinfo",MODE_PRIVATE);
        host=new String(pref.getString("host",getString(R.string.defaultHost)));
        port=new String(pref.getString("port",getString(R.string.defaultPort)));
        PubTopic=new String(pref.getString("PubTopic",getString(R.string.defaultPubTopic)));
        SubTopic= new String(pref.getString("SubTopic",getString(R.string.defaultSubTopic)));
        ClientId=new String(pref.getString("ClientId",getString(R.string.defaultClientId)));
        username=new String(pref.getString("username",getString(R.string.defaultUserName)));
        password= new String(pref.getString("password",getString(R.string.defaultPassword)));
        mqttSetting=new MqttSetting();
        mqttSetting.setUrl(host,port);
        mqttSetting.setPubTopic(PubTopic);
        mqttSetting.setSubTopic(SubTopic);
        mqttSetting.setClientId(ClientId);
        mqttSetting.setUserName(username);
        mqttSetting.setPassword(password);
    }


    class LocalReceiver extends BroadcastReceiver{

        @Override
        public void onReceive(Context context, Intent intent) {
            //有消息来了
            if(intent.getAction().equalsIgnoreCase("com.qianxu.mqttService.messageArrived")){
                String topic=intent.getStringExtra("Topic");
                String message=intent.getStringExtra("message");
                //TODO:更新edittor
                Toast.makeText(getApplicationContext(),"Topic:"+topic+" message:"+message,Toast.LENGTH_LONG).show();
            }
            if(intent.getAction().equalsIgnoreCase("com.qianxu.mqttService.connectLost")){
               imageIntent.setImageResource(R.drawable.notconnect);
               textIntent.setText(R.string.isNotConnected);
               switchConnect.setChecked(false);
            }
        }
    }

}
