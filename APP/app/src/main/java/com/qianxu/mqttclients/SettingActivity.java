package com.qianxu.mqttclients;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SettingActivity extends BasicActivity implements TextWatcher {

    private EditText host,port,PubTopic,SubTopic,ClientId,username,password;
    private boolean isChanged=false;
    private Button saveButton;

    private void findView(){
        host=(EditText)findViewById(R.id.host);
        port=(EditText)findViewById(R.id.port);
        PubTopic=(EditText)findViewById(R.id.PubTopic);
        SubTopic=(EditText)findViewById(R.id.SubTopic);
        ClientId=(EditText)findViewById(R.id.ClientId);
        username=(EditText)findViewById(R.id.username);
        password=(EditText)findViewById(R.id.password);
        saveButton=(Button)findViewById(R.id.saveButton);
    }
    private void addListener(){
        host.addTextChangedListener(this);
        port.addTextChangedListener(this);
        PubTopic.addTextChangedListener(this);
        SubTopic.addTextChangedListener(this);
        ClientId.addTextChangedListener(this);
        username.addTextChangedListener(this);
        password.addTextChangedListener(this);
        saveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                WriteData();
                isChanged=false;
                Toast.makeText(getApplicationContext(),"保存成功",Toast.LENGTH_LONG).show();
            }
        });
    }
    private void WriteData(){
       SharedPreferences.Editor editor=getSharedPreferences("keyinfo",MODE_PRIVATE).edit();
       editor.putString("host",host.getText().toString());
       editor.putString("port",port.getText().toString());
       editor.putString("PubTopic",PubTopic.getText().toString());
       editor.putString("SubTopic",SubTopic.getText().toString());
       editor.putString("ClientId",ClientId.getText().toString());
       editor.putString("username",username.getText().toString());
       editor.putString("password",password.getText().toString());
       editor.apply();
    }
    private void ReadData(){
       SharedPreferences pref=getSharedPreferences("keyinfo",MODE_PRIVATE);
       host.setText(pref.getString("host",getString(R.string.defaultHost)));
       port.setText(pref.getString("port",getString(R.string.defaultPort)));
       PubTopic.setText(pref.getString("PubTopic",getString(R.string.defaultPubTopic)));
       SubTopic.setText(pref.getString("SubTopic",getString(R.string.defaultSubTopic)));
       ClientId.setText(pref.getString("ClientId",getString(R.string.defaultClientId)));
       username.setText(pref.getString("username",getString(R.string.defaultUserName)));
       password.setText(pref.getString("password",getString(R.string.defaultPassword)));
    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting);

        findView();

        addListener();

        ReadData();
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after){

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count){

    }

    @Override
    public void afterTextChanged(Editable s){
        isChanged=true;
    }

    @Override
    public void onBackPressed() {
        if(isChanged){
            AlertDialog.Builder dialog=new AlertDialog.Builder(SettingActivity.this);
            dialog.setTitle("尚未保存");
            dialog.setMessage("更改没有保存，是否退出");
            dialog.setCancelable(true);
            dialog.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    finish();
                }
            });
            dialog.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {

                }
            });
            dialog.show();
        }else {
        super.onBackPressed();
        }
    }
}
