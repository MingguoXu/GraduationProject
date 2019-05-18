package com.xuzong.graduationdesign;

import android.os.Bundle;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.LinearLayout;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by xuzong on 2019/1/11.
 */

public class MyAnimation extends BaseActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.my_animation);
        final LinearLayout tv_lin= (LinearLayout) findViewById(R.id.text_lin);//要显示的字体
        final LinearLayout tv_hide_lin= (LinearLayout) findViewById(R.id.text_hide_lin);//所谓的布
        ImageView logo= (ImageView) findViewById(R.id.image);//图片
        Animation animation = AnimationUtils.loadAnimation(this,R.anim.splash);
        logo.startAnimation(animation);//开始执行动画
        animation.setAnimationListener(new Animation.AnimationListener() {
            @Override
            public void onAnimationStart(Animation animation) {

            }
            @Override
            public void onAnimationEnd(Animation animation) {
                //第一个动画执行完后执行第二个动画就是那个字体显示那部分
                animation= AnimationUtils.loadAnimation(MyAnimation.this,R.anim.text_splash_position);
                tv_lin.startAnimation(animation);
                animation=AnimationUtils.loadAnimation(MyAnimation.this,R.anim.text_canvas);
                tv_hide_lin.startAnimation(animation);
                TimerTask task = new TimerTask() {
                    @Override
                    public void run() {
                        finish();
                    }
                };
                Timer timer = new Timer();
                timer.schedule(task, 1600);//1.6秒后执行TimeTask的run方法
            }

            @Override
            public void onAnimationRepeat(Animation animation) {

            }
        });
    }
}
    