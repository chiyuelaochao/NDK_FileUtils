#include <jni.h>

#include <android/log.h>
#include <assert.h>
#include <malloc.h>
#include <pthread.h>

#define TAG "CaiWei_JNI"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
# define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
JavaVM *g_jvm = NULL;
jobject g_obj = NULL;

JNIEXPORT void JNICALL
Java_com_caiwei_dynamicregistration_RegisterUtils_register(JNIEnv *env, jclass type, jstring path_,
                                                           jstring pattern_Path_, jint file_num) {
    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    const char *pattern_Path = (*env)->GetStringUTFChars(env, pattern_Path_, 0);

    // TODO
    LOGI("JNI Java_com_caiwei_dynamicregistration_RegisterUtils_register running");

    (*env)->ReleaseStringUTFChars(env, path_, path);
    (*env)->ReleaseStringUTFChars(env, pattern_Path_, pattern_Path);
}

JNIEXPORT void JNICALL native_register_1(JNIEnv *env, jclass type, jstring path_,
                                         jstring pattern_Path_, jint file_num) {
    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    const char *pattern_Path = (*env)->GetStringUTFChars(env, pattern_Path_, 0);

    // TODO
    LOGI("JNI native_register_1 Begin 动态注册方法 running");

    (*env)->ReleaseStringUTFChars(env, path_, path);
    (*env)->ReleaseStringUTFChars(env, pattern_Path_, pattern_Path);
}

JNIEXPORT void JNICALL native_register_2(JNIEnv *env, jclass type, jstring path_,
                                         jstring pattern_Path_, jint file_num) {
    const char *path = (*env)->GetStringUTFChars(env, path_, 0);
    const char *pattern_Path = (*env)->GetStringUTFChars(env, pattern_Path_, 0);

    // TODO
    LOGI("JNI native_register_2 Begin 动态注册方法 running");

    (*env)->ReleaseStringUTFChars(env, path_, path);
    (*env)->ReleaseStringUTFChars(env, pattern_Path_, pattern_Path);
}

//获取文件大小
long get_file_size(const char *path) {
    FILE *fp = fopen(path, "rb"); //打开一个文件， 文件必须存在，只运行读
    fseek(fp, 0, SEEK_END);
    long ret = ftell(fp);
    fclose(fp);
    return ret;
}

JNIEXPORT void JNICALL native_diff
        (JNIEnv *env, jclass clazz, jstring path, jstring pattern_Path, jint file_num) {
    LOGI("JNI native diff begin");

    const char *path_Str = (*env)->GetStringUTFChars(env, path, NULL);
    const char *pattern_Path_str = (*env)->GetStringUTFChars(env, pattern_Path, NULL);

    //申请二维字符数据, 存放子文件名
    char **patches = (char **) malloc(sizeof(char *) * file_num);

    int i = 0;
    for (; i < file_num; i++) {
        //每个文件名申请地址
        LOGI("char = %d char * = %d", sizeof(char), sizeof(char *));
        patches[i] = (char *) malloc(sizeof(char) * 100);
        // 需要分割的文件 Vibrato.mp4
        // 每个子文件名称 Vibrato_n.mp4
        sprintf(patches[i], pattern_Path_str, i);// 格式化文件名
        LOGI("patch path : %s", patches[i]);
    }

    int fileSize = get_file_size(path_Str);
    FILE *fpr = fopen(path_Str, "rb");
    /*
     * 1.判断文件大小能够被 file_num整除
     * 2.能整除就平分
     * 3.不能整除就先分 file_num -1
     * */

    if (fileSize % file_num == 0) {

        int part = fileSize / file_num;
        for (int i = 0; i < file_num; i++) {
            FILE *fpw = fopen(patches[i], "wb");//文件已经存在 就删除，只运行写
            for (int j = 0; j < part; j++) {
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }

    } else {
        int part = fileSize / (file_num - 1);
        for (int i = 0; i < file_num - 1; i++) {
            FILE *fpw = fopen(patches[i], "wb");//文件已经存在 就删除，只运行写
            for (int j = 0; j < part; j++) {
                fputc(fgetc(fpr), fpw);
            }
            fclose(fpw);
        }

        FILE *fpw = fopen(patches[file_num - 1], "wb");

        for (int i = 0; i < fileSize % (file_num - 1); i++) {
            fputc(fgetc(fpr), fpw);
        }
        fclose(fpw);
    }

    fclose(fpr);

    for (int i = 0; i < file_num; i++) {
        free(patches[i]);
    }
    free(patches);
    (*env)->ReleaseStringUTFChars(env, path, path_Str);
    (*env)->ReleaseStringUTFChars(env, pattern_Path, pattern_Path_str);
}

JNIEXPORT void JNICALL native_patch
        (JNIEnv *env, jclass clazz, jstring merge_path, jstring pattern_Path, jint file_num) {
    LOGI("JNI native patch begin");
    const char *path_Str = (*env)->GetStringUTFChars(env, merge_path, NULL);
    const char *pattern_Path_str = (*env)->GetStringUTFChars(env, pattern_Path, NULL);

    //申请二维字符数据, 存放子文件名
    char **patches = (char **) malloc(sizeof(char *) * file_num);

    int i = 0;
    for (; i < file_num; i++) {
        //每个文件名申请地址
//        LOGI("char = %d char * = %d", sizeof(char), sizeof(char *));
        patches[i] = (char *) malloc(sizeof(char) * 100);
        // 需要分割的文件 Vibrato.mp4
        // 每个子文件名称 Vibrato_n.mp4
        sprintf(patches[i], pattern_Path_str, i);// 格式化文件名
        LOGI("patch path : %s", patches[i]);
    }

    FILE *fpw = fopen(path_Str, "wb");

    for (int i = 0; i < file_num; i++) {
        int filesize = get_file_size(patches[i]);
        FILE *fpr = fopen(patches[i], "rb");
        for (int j = 0; j < filesize; j++) {
            fputc(fgetc(fpr), fpw);
        }
        fclose(fpr);
    }
    fclose(fpw);

    for (int i = 0; i < file_num; i++) {
        free(patches[i]); //每一个malloc 对应一个free
    }
    free(patches);
    (*env)->ReleaseStringUTFChars(env, merge_path, path_Str);
    (*env)->ReleaseStringUTFChars(env, pattern_Path, pattern_Path_str);
}

void *thread_fun(void *arg) {

    JNIEnv *env;
    jclass cls;
    jmethodID mid, mid1;

    if ((*g_jvm)->AttachCurrentThread(g_jvm, &env, NULL) != JNI_OK) {
        LOGI("%s AttachCurrentThread error failed ", __FUNCTION__);
        return NULL;
    }

    cls = (*env)->GetObjectClass(env, g_obj);
    if (cls == NULL) {
        LOGI("findClass error....");
        goto error;
    }
    LOGI("call back begin");
    mid = (*env)->GetStaticMethodID(env, cls, "formJni", "(I)V");
    if (mid == NULL) {
        LOGI("GetStaticMethodID error....");
        goto error;
    }

    (*env)->CallStaticVoidMethod(env, cls, mid, (int) arg);

    mid1 = (*env)->GetMethodID(env, cls, "form_JNI_Again", "(I)V");
    if (mid1 == NULL) {
        LOGI("GetMethodID error....");
        goto error;
    }
    (*env)->CallVoidMethod(env, g_obj, mid1, (int) arg);

    error:
    if ((*g_jvm)->DetachCurrentThread(g_jvm) != JNI_OK) {
        LOGI("%s DetachCurrentThread error failed ", __FUNCTION__);
    }
    pthread_exit(0);
}

JNIEXPORT void JNICALL native_newThread(JNIEnv *env, jclass clazz) {
    LOGI("newThread begin");
    int i;
    pthread_t pt[5];
    for (i = 0; i < 5; i++) {
        pthread_create(&pt[i], NULL, &thread_fun, (void *) i);
    }
}

JNIEXPORT void JNICALL native_setJniEnv(JNIEnv *env, jobject obj) {
    LOGI("native_setJniEnv");
    //保存JVM
    (*env)->GetJavaVM(env, &g_jvm);
    //保持activity对象
    g_obj = (*env)->NewGlobalRef(env, obj);
}

static const JNINativeMethod gMethods_register[] = {
        {"register_1", "(Ljava/lang/String;Ljava/lang/String;I)V", (void *) native_register_1},
        {"register_2", "(Ljava/lang/String;Ljava/lang/String;I)V", (void *) native_register_2}
};

static const JNINativeMethod gMethods_file[] = {
        {"diff",  "(Ljava/lang/String;Ljava/lang/String;I)V", (void *) native_diff},
        {"patch", "(Ljava/lang/String;Ljava/lang/String;I)V", (void *) native_patch}
};

static const JNINativeMethod gMethods_thread[] = {
        {"newJniThread", "()V", (void *) native_newThread},
        {"setJniEnv",    "()V", (void *) native_setJniEnv}
};

//注册RegisterUtils里面native方法
static int registerRegisterUtilsNatives(JNIEnv *engv) {
    LOGI("registerRegisterUtilsNatives begin");
    jclass clazz;
    clazz = (*engv)->FindClass(engv, "com/caiwei/dynamicregistration/RegisterUtils");

    if (clazz == NULL) {
        LOGI("clazz is null");
        return JNI_FALSE;
    }

    if ((*engv)->RegisterNatives(engv, clazz, gMethods_register, NELEM(gMethods_register)) < 0) {
        LOGI("RegisterNatives error");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

//注册FileUtils中的native方法
static int registerFileUtilsNatives(JNIEnv *engv) {
    LOGI("registerFileUtilsNatives begin");
    jclass clazz;
    clazz = (*engv)->FindClass(engv, "com/caiwei/dynamicregistration/FileUtils");

    if (clazz == NULL) {
        LOGI("clazz is null");
        return JNI_FALSE;
    }

    if ((*engv)->RegisterNatives(engv, clazz, gMethods_file, NELEM(gMethods_file)) < 0) {
        LOGI("registerFileUtilsNatives error");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

//注册JniThreadUtils中的native方法
static int registerJniThreadUtilsNatives(JNIEnv *engv) {
    LOGI("registerFileUtilsNatives begin");
    jclass clazz;
    clazz = (*engv)->FindClass(engv, "com/caiwei/dynamicregistration/JniThreadUtils");

    if (clazz == NULL) {
        LOGI("clazz is null");
        return JNI_FALSE;
    }

    if ((*engv)->RegisterNatives(engv, clazz, gMethods_thread, NELEM(gMethods_thread)) < 0) {
        LOGI("registerFileUtilsNatives error");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

/*
 * run first
 * */
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("jni_OnLoad begin");
    JNIEnv *env = NULL;
    jint result = -1;

    if ((*vm)->GetEnv(vm, (void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("ERROR: GetEnv failed\n");
        return -1;
    }
    assert(env != NULL);

    registerRegisterUtilsNatives(env);
    registerFileUtilsNatives(env);
    registerJniThreadUtilsNatives(env);

    return JNI_VERSION_1_4;
}