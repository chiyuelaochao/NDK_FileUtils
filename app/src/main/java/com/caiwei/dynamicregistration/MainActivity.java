package com.caiwei.dynamicregistration;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ProgressBar;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private static String TAG = "MainActivity";
    private ProgressBar progressBar;
    /*
    *1. 编译
        xxx.c  ------> windows .obj  语法 Linux .o
    2. 链接
        .o ----->  log.so
    */

    /*
    编译器编译规则：
		Eclipse GUN  ----> Android.mk
		Android Studio LLVM ----> CMakeList.txt
	*/
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        progressBar = (ProgressBar) findViewById(R.id.progress);
        /*
         Classloader =  dalvik.system.PathClassLoader[
         DexPathList[
                        [zip file "/data/app/com.caiwei.dynamicregistration-2/base.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_dependencies_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_0_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_1_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_2_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_3_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_4_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_5_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_6_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_7_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_8_apk.apk",
                        zip file "/data/app/com.caiwei.dynamicregistration-2/split_lib_slice_9_apk.apk"],
                        nativeLibraryDirectories=[/data/app/com.caiwei.dynamicregistration-2/lib/arm, /vendor/lib, /system/lib]  //厂商library库和系统library库
                        ]
                    ]

          Classloader为null，则到/vendor/lib, /system/lib中来找
          JVM通过native方法(runtime.cc  runtime_nativeLoad)来加载so库
        */
        Log.e(TAG, "Classloader = " + this.getClassLoader().toString());

        //system libraryPath :/vendor/lib:/system/lib
        Log.e(TAG, "system libraryPath :" + System.getProperty("java.library.path"));
    }

    private static String SD_CARD_PATH = Environment.getExternalStorageDirectory().getAbsolutePath();

    public void register(View v) {
        Log.d(TAG, "register begin----");
        RegisterUtils.register("x", "s", 3);
        RegisterUtils.register_1("x", "s", 3);
        RegisterUtils.register_2("x", "s", 3);
        Log.d(TAG, "register end----");
    }

    public void diff(View v) {
        Log.d(TAG, "diff begin----");
        progressBar.setVisibility(View.VISIBLE);
        new Thread(new Runnable() {
            @Override
            public void run() {
                String path = SD_CARD_PATH + File.separatorChar + "Vibrato.mp4";
                String pattern_Path = SD_CARD_PATH + File.separatorChar + "Vibrato_%d.mp4";
                FileUtils.diff(path, pattern_Path, 4);
            }
        }).run();
        progressBar.setVisibility(View.GONE);
        Log.d(TAG, "diff end----");
    }

    public void patch(View v) {
        Log.d(TAG, "patch begin----");
        progressBar.setVisibility(View.VISIBLE);
        new Thread(new Runnable() {
            @Override
            public void run() {
                String path = SD_CARD_PATH + File.separatorChar + "Vibrato_merger.mp4";
                String pattern_Path = SD_CARD_PATH + File.separatorChar + "Vibrato_%d.mp4";
                FileUtils.patch(path, pattern_Path, 4);
            }
        }).run();
        progressBar.setVisibility(View.GONE);
        Log.d(TAG, "patch end----");
    }

    public void newJniThread(View v) {
        Log.d(TAG, "newJniThread begin----");
        JniThreadUtils.newJniThread();
        Log.d(TAG, "newJniThread end----");
    }

    public void setJniEnv(View v) {
        Log.d(TAG, "setJniEnv begin----");
        JniThreadUtils.setJniEnv();
        Log.d(TAG, "setJniEnv end----");
    }
}
