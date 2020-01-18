package com.qianxu.mqttclients;


import android.content.Intent;
import android.os.Bundle;

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

public class MainActivity extends BasicActivity {

    private TextView light,water,pm;
    private EditText angle;
    private Button sendButton;
    private DrawerLayout drawerLayout;
    private NavigationView navigationView;
    private ImageView home;

    private void findView(){
        drawerLayout=(DrawerLayout)findViewById(R.id.drawer_layout);
        navigationView=(NavigationView)findViewById(R.id.nav);
        home=(ImageView)findViewById(R.id.home);
        light=(TextView)findViewById(R.id.light);
        water = (TextView) findViewById(R.id.water);
        pm=(TextView)findViewById(R.id.PM);
        angle=(EditText)findViewById(R.id.angle);
        sendButton=(Button)findViewById(R.id.sendButton);
    }

    private void setListener(){
        sendButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //TODO:
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

        //找到View
        findView();
        //setListener
        setListener();
    }


}
