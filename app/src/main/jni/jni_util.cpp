#include "jni_util.h"

#include <android/log.h>

#define  LOG_TAG    "jd::MsgClient"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


jmethodID getMethodID(JNIEnv* env, jobject& obj, string& methodName, string& signature) {
    jmethodID cb = NULL;
    jclass cls = env->GetObjectClass(obj);

    if (cb == NULL) {
        cb = env->GetMethodID(cls, methodName.c_str(), signature.c_str());
    }

    return cb;
}

void callbackJavaMethod(JNIEnv* env, jobject& obj, string methodName, string signature) {
    jmethodID cb = getMethodID(env, obj, methodName, signature);

    if (cb)
        env->CallVoidMethod(obj, cb);
}

void callbackJavaMethod(JNIEnv* env, jobject& obj, string methodName, string signature, string param) {
    jmethodID cb = getMethodID(env, obj, methodName, signature);

    if (cb)
        env->CallVoidMethod(obj, cb, env->NewStringUTF(param.c_str()));
}

JNIEnv* getJNIEnv(JavaVM* jvm)
{

    if (NULL == jvm) {
        LOGD("Failed to get JNIEnv. JniHelper::getJavaVM() is NULL");
        return NULL;
    }

    JNIEnv *env = NULL;
    // get jni environment
    jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

    switch (ret) {
        case JNI_OK :
            // Success!
            return env;

        case JNI_EDETACHED :
            // Thread not attached

            // TODO : If calling AttachCurrentThread() on a native thread
            // must call DetachCurrentThread() in future.
            // see: http://developer.android.com/guide/practices/design/jni.html

            if (jvm->AttachCurrentThread(&env, NULL) < 0)
            {
                LOGD("Failed to get the environment using AttachCurrentThread()");
                return NULL;
            } else {
                // Success : Attached and obtained JNIEnv!
                return env;
            }

        case JNI_EVERSION :
            // Cannot recover from this error
            LOGD("JNI interface version 1.4 not supported");
        default :
            LOGD("Failed to get the environment using GetEnv()");
            return NULL;
    }
}

// get class and make it a global reference, release it at endJni().
jclass getClassID(JNIEnv *pEnv, const char* className)
{
    jclass ret = pEnv->FindClass(className);
    if (! ret)
    {
        LOGD("Failed to find class of %s", className);
    }

    return ret;
}

bool getStaticMethodInfo(JavaVM* jvm, JniMethodInfo &methodinfo, const char* className, const char *methodName, const char *paramCode)
{
    jmethodID methodID = 0;
    JNIEnv *pEnv = 0;
    bool bRet = false;

    do
    {
        pEnv = getJNIEnv(jvm);
        if (! pEnv)
        {
            break;
        }

        jclass classID = getClassID(pEnv, className);

        methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
        if (! methodID)
        {
            LOGD("Failed to find static method id of %s", methodName);
            break;
        }

        methodinfo.classID = classID;
        methodinfo.env = pEnv;
        methodinfo.methodID = methodID;

        bRet = true;
    } while (0);

    return bRet;
}

void callback(JavaVM* jvm, const char* value){
    JniMethodInfo methodInfo;
    if (! getStaticMethodInfo(jvm, methodInfo, "com/jeidee/glooxforandroid/MsgClient", "onConnect", "()V"))
    {
        return;
    }

    jstring stringArg = methodInfo.env->NewStringUTF(value);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
    methodInfo.env->DeleteLocalRef(stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}
