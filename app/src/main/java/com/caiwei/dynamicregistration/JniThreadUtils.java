package com.caiwei.dynamicregistration;

import android.util.Log;

public class JniThreadUtils {
    private static String TAG = "JniThreadUtils";

    public static native void newJniThread();

    public static native void setJniEnv();

    public static void formJni(int i) {
        Log.d(TAG, "form jni : " + i);
    }

    public void form_JNI_Again(int i) {
        Log.d(TAG, "form_JNI_Again : " + i);
    }

    // Used to load the 'native-lib' library on application startup.
    /*
     ClassLoader会根据平台的不同把native-lib补充完整为libnative-lib.so或libnative-lib.dll，并加载
    */
    static {
        System.loadLibrary("native-lib");
    }
}
