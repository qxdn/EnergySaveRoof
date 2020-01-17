package com.qianxu.mqttclients;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;

public class MainActivity extends BasicActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main_menu,menu);
        return true;
    }

    //目录点击 activity跳转
    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()){
            case R.id.Setting:
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
        return true;
    }
}
