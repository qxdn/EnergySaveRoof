package com.qianxu.mqttclients;


import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;

import android.os.IBinder;
import android.util.Log;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.view.GravityCompat;
import androidx.drawerlayout.widget.DrawerLayout;

import com.google.android.material.navigation.NavigationView;

import org.eclipse.paho.android.service.MqttService;

public class MainActivity extends BasicActivity {

    private TextView light,water,pm;
    private EditText angle;
    private Button sendButton,connectButton;
    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ImageView home;


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


    private void findView(){
        drawerLayout=(DrawerLayout)findViewById(R.id.drawer_layout);
        navigationView=(NavigationView)findViewById(R.id.nav);
        home=(ImageView)findViewById(R.id.home);
        light=(TextView)findViewById(R.id.light);
        water = (TextView) findViewById(R.id.water);
        pm=(TextView)findViewById(R.id.PM);
        angle=(EditText)findViewById(R.id.angle);
        sendButton=(Button)findViewById(R.id.sendButton);
        connectButton=(Button)findViewById(R.id.connect);
    }

    private void setListener(){
        connectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(!mqttService.isConnected()){
                    mqttService.Connect();
                }
            }
        });
        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO:
                if(mqttService.isConnected()){
                    mqttService.Publish("helloworld");
                }
            }
        });
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


    }

    @Override
    protected void onResume() {
        super.onResume();
        ReadData();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(connection);
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
}
