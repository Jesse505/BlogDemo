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


// 比较两个数的大小
int compare(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

//Java层传递数组给Native
JNIEXPORT void JNICALL inputSortArray(JNIEnv* env, jobject obj, jintArray jintArray) {
    LOGI("inputSortArray begin");
    //获取数组中的元素
    jint *int_arr = env->GetIntArrayElements(jintArray, NULL);
    //获取数组的长度
    int length = env->GetArrayLength(jintArray);
    //快速排序
    qsort(int_arr, length, sizeof(int), compare);
    // 同步操作后的数组内存到java中
    // 最后一个参数mode的解释
    // 0：Java的数组更新同步，然后释放C/C++的数组内存
    // JNI_ABORT：Java的数组不会更新同步，但是释放C/C++的数组内存
    // JNI_COMMIT：Java的数组更新同步，不释放C/C++的数组内存（但是函数执行完了局部的变量还是会释放掉）
    env->ReleaseIntArrayElements(jintArray, int_arr, 0);
}


//Native层传递数组给Java层
JNIEXPORT jintArray JNICALL getArray(JNIEnv* env, jobject obj, jint len) {
    jintArray jintArray = env->NewIntArray(len);
    jint *elements = env->GetIntArrayElements(jintArray, NULL);
    for (int i = 0; i < len; ++i) {
        elements[i] = i;
    }
    env->ReleaseIntArrayElements(jintArray, elements, 0);
    return jintArray;
}


//Native层传递二维数组给Java层
JNIEXPORT jobjectArray JNICALL getTwoArray(JNIEnv* env, jobject obj, jint len) {
    //获得一维数组的类引用，即jintArray类型
    jclass intArrayClass = env->FindClass("[I");
    //构造指向jintArray一维数组的对象数组，数组长度为len
    jobjectArray objectIntArray = env->NewObjectArray(len, intArrayClass, NULL);
    for (int i = 0; i < len; ++i) {
        jintArray intArray = env->NewIntArray(len);
        //初始化一个容器，假设len<10
        jint temp[10];
        for (int j = 0; j < len; ++j) {
            temp[j] = j + i;
        }
        //设置一维数组的值
        env->SetIntArrayRegion(intArray, 0, len, temp);
        //设置对象数组的值
        env->SetObjectArrayElement(objectIntArray, i, intArray);
        //删除局部引用
        env->DeleteLocalRef(intArray);
    }
    return objectIntArray;
}


//Native层传递复杂对象给Java层
JNIEXPORT jobject JNICALL getStuInfo(JNIEnv* env, jobject obj) {
    LOGI("getStuInfo begin");
    jclass stu_cls = env->FindClass("com/example/github/jnidemo/MainActivity$Student");
    if (stu_cls == NULL) {
        return NULL;
    }
    //获得构造方法Id,函数名为<init>,返回类型为void,即V
    jmethodID constructMId = env->GetMethodID(stu_cls, "<init>", "(ILjava/lang/String;)V");
    if (constructMId == NULL) {
        LOGI("getStuInfo getConstruc failed");
        return NULL;
    }
    jstring name = env->NewStringUTF("赵一飞");
    //构造一个对象，调用构造方法并且传入相应参数
    return env->NewObject(stu_cls, constructMId, 26, name);
}

static JNINativeMethod gMethods[] = {
        { "helloFromJNI", "()Ljava/lang/String;", (void *)helloFromJNI },
        {"printStuInfoAtNative", "(Lcom/example/github/jnidemo/MainActivity$Student;)V", (void *)printStuInfoAtNative },
        {"inputSortArray", "([I)V", (void *)inputSortArray},
        {"getArray", "(I)[I", (void*)getArray},
        {"getTwoArray", "(I)[[I", (void*)getTwoArray},
        {"getStuInfo", "()Lcom/example/github/jnidemo/MainActivity$Student;", (void*)getStuInfo}
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


