#include <jni.h>
#include <string>

#include <android/log.h>

#define TAG "zhaoyifei"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, TAG, __VA_ARGS__)
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))


JNIEXPORT jstring JNICALL helloFromJNI(JNIEnv* env, jobject obj)
{
    LOGI("动态加载完成啦");
    return env->NewStringUTF("hello world returned.");
}


//Java层传递复杂对象给Native
JNIEXPORT void JNICALL printStuInfoAtNative(JNIEnv* env, jobject obj, jobject stu_obj){
    jclass stu_cls = env -> GetObjectClass(stu_obj);//获得student类引用
    if (NULL == stu_cls) {
        LOGI("GetObjectClass failed");
    }
    jfieldID ageFieldId = env->GetFieldID(stu_cls, "age", "I"); //获得属性ID
    jfieldID nameFieldId = env->GetFieldID(stu_cls, "name", "Ljava/lang/String;"); //获得属性ID
    jint age = env->GetIntField(stu_obj, ageFieldId); //获得属性值
    jstring  name = (jstring)env->GetObjectField(stu_obj, nameFieldId); //获得属性值
    const char * c_name = env->GetStringUTFChars(name, NULL); //转换为char*
    LOGI("name: %s, age: %d" , c_name, (int)age);
    env->ReleaseStringUTFChars(name, c_name); //释放引用
    return;
}


static JNINativeMethod gMethods[] = {
        { "helloFromJNI", "()Ljava/lang/String;", (void *)helloFromJNI },
        {"printStuInfoAtNative", "(Lcom/example/github/jnidemo/MainActivity$Student;)V", (void *)printStuInfoAtNative },
};

static int registerNatives(JNIEnv* env) {
    LOGI("registerNatives begin");
    jclass  clazz;
    clazz = env -> FindClass("com/example/github/jnidemo/MainActivity");

    if (clazz == NULL) {
        LOGI("clazz is null");
        return JNI_FALSE;
    }

    if (env ->RegisterNatives(clazz, gMethods, NELEM(gMethods)) != JNI_OK) {
        LOGI("RegisterNatives error");
        return JNI_FALSE;
    }

    return JNI_TRUE;
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {

    LOGI("jni_OnLoad begin");

    JNIEnv *env = NULL;
    jint result = -1;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {
        LOGI("ERROR: GetEnv failed\n");
        return result;
    }
    if (registerNatives(env) < 0) {
        LOGI("ERROR: registerNatives failed\n");
        return result;
    }

    return JNI_VERSION_1_4;
}

void JNI_OnUnload(JavaVM* vm, void *reserved)
{
    return;
}


//静态注册生成的Native方法，方法名较长
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_github_jnidemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

