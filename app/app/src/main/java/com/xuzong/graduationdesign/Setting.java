package com.xuzong.graduationdesign;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

/**
 * Created by xuzong on 2019/5/11.
 */

public class Setting extends BaseActivity {

    private static final String TAG = "SettingActivity";
    private EditText addrEdit;
    private EditText portEdit;
    private Button submit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting);

        addrEdit = (EditText) findViewById(R.id.addr);
        portEdit = (EditText) findViewById(R.id.port);
        submit = (Button) findViewById(R.id.submit);

        addrEdit.setText(MainActivity.addr);
        portEdit.setText("" + MainActivity.port);

        submit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MainActivity.addr = addrEdit.getText().toString();
                MainActivity.port = Integer.parseInt(portEdit.getText().toString());

                LogUtil.d(TAG, "地址和端口修改成功！");
                Toast.makeText(Setting.this, "地址和端口修改成功！", Toast.LENGTH_SHORT).show();
            }
        });
    }
}