package com.caiwei.dynamicregistration;

public class FileUtils {
    public static native void diff(String path, String pattern_Path, int file_num);

    public static native void patch(String merger_path, String pattern_Path, int file_num);

    // Used to load the 'native-lib' library on application startup.
    /*
     ClassLoader会根据平台的不同把native-lib补充完整为libnative-lib.so或libnative-lib.dll，并加载
    */
    static {
        System.loadLibrary("native-lib");
    }
}
