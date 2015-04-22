#ifndef JD_JNI_UTIL_H__
#define JD_JNI_UTIL_H__

#include <jni.h>
#include <string>

using namespace std;

jmethodID getMethodID(JNIEnv* env, jobject& obj, string& methodName, string& signature);

void callbackJavaMethod(JNIEnv* env, jobject& obj, string methodName, string signature);

void callbackJavaMethod(JNIEnv* env, jobject& obj, string methodName, string signature, string param);

typedef struct stJniMethodInfo
{
    JNIEnv *    env;
    jclass      classID;
    jmethodID   methodID;
} JniMethodInfo;

JNIEnv* getJNIEnv(JavaVM* jvm);

// get class and make it a global reference, release it at endJni().
jclass getClassID(JNIEnv *pEnv, const char* className);

bool getStaticMethodInfo(JavaVM* jvm, JniMethodInfo &methodinfo, const char* className, const char *methodName, const char *paramCode);

void callback(JavaVM* jvm, const char* value);

#endif  // JD_JNI_UTIL_H__