package com.example.lzqtest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;


import com.example.vrpdet.VRPDet;

public class MainActivity extends AppCompatActivity {
    private String TAG = "lzqtest";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        VRPDet.VACheck();
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setText("123456");//调用设置显示方法
    }

    private void setText(String s) {
        VRPDet.HookCheck();
        TextView tv_show = (TextView) findViewById(R.id.text);//获取UI界面布局
        tv_show.setText(s); //将传入的字符串在UI界面显示
    }
}