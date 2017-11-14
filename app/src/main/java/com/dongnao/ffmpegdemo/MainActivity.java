package com.dongnao.ffmpegdemo;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static{

        System.loadLibrary("avcodec-56");
        System.loadLibrary("avdevice-56");
        System.loadLibrary("avfilter-5");
        System.loadLibrary("avformat-56");
        System.loadLibrary("avutil-54");
        System.loadLibrary("postproc-53");
        System.loadLibrary("swresample-1");
        System.loadLibrary("swscale-3");
        System.loadLibrary("native-lib");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ((TextView)findViewById(R.id.tv)).setText(stringFromJNI());
    }

    /**
     */
    public native String stringFromJNI();

    /**
     * 将mp4文件转换为yuv（人是对色度不敏感，对亮度敏感的）, 中"Y"表示明亮度，而"U"和"V"表示的则是色度，作用是描述影像色彩及饱和度
     * @param inputPath
     * @param outPath
     */
    public native void convert(String inputPath, String outPath);
}
