#include "com_jeidee_glooxforandroid_MsgClient.h"

#include "msg_client.h"
#include "thread_lock.h"

using namespace jd;

#include <android/log.h>

#define  LOG_TAG    "jd::MsgClient"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

static JavaVM* g_JVM;

jint JNI_OnLoad(JavaVM *jvm, void *reserved)
{
    LOGD("JNI_OnLoad");
    g_JVM = jvm;

    JNIEnv* env;
    if (jvm->GetEnv((void **)&env, JNI_VERSION_1_4) != JNI_OK) {
        LOGD("GETENVFAILEDONLOAD");
        return -1;
    }
    return JNI_VERSION_1_4;
}

string toString(JNIEnv* env, jstring in) {
    char* nativeStr;
    const char* temp = env->GetStringUTFChars(in, 0);
    nativeStr = strdup(temp);
    env->ReleaseStringUTFChars(in, temp);

    LOGD("toString() %s", nativeStr);
    return string(nativeStr);
}

/*
 * Class:     com_jeidee_glooxforandroid_MsgClient
 * Method:    _newInstance
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1newInstance
  (JNIEnv *env, jobject obj) {
    MsgClient* clt = new MsgClient(g_JVM, env, obj);
    return (long)clt;
}

/*
 * Class:     com_jeidee_glooxforandroid_MsgClient
 * Method:    _setLoginInfo
 * Signature: (JLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1setLoginInfo
  (JNIEnv *env, jobject, jlong clientPtr, jstring jid, jstring pwd, jstring host, jint port) {

    MsgClient* clt = (MsgClient*)(clientPtr);
    if (!clt) return;

    return clt->setLoginInfo(toString(env, jid), toString(env, pwd), toString(env, host), port);
}

/*
 * Class:     com_jeidee_glooxforandroid_MsgClient
 * Method:    _connect
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1connect
  (JNIEnv *env, jobject, jlong clientPtr) {
    MsgClient* clt = (MsgClient*)(clientPtr);
    if (!clt) return false;

    return clt->connect();
}

/*
 * Class:     com_jeidee_glooxforandroid_MsgClient
 * Method:    _disConnect
 * Signature: (J)Z
 */
JNIEXPORT jboolean JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1disConnect
  (JNIEnv *, jobject, jlong clientPtr) {
    MsgClient* clt = (MsgClient*)(clientPtr);
    if (!clt) return false;

    return clt->disConnect();
 }

/*
 * Class:     com_jeidee_glooxforandroid_MsgClient
 * Method:    _deleteInstance
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1deleteInstance
  (JNIEnv *, jobject, jlong clientPtr) {
    if (!clientPtr) return;
    delete (MsgClient *)(clientPtr);
 }

 /*
  * Class:     com_jeidee_glooxforandroid_MsgClient
  * Method:    _recv
  * Signature: (J)V
  */
 JNIEXPORT void JNICALL Java_com_jeidee_glooxforandroid_MsgClient__1recv
   (JNIEnv *env, jobject obj, jlong clientPtr) {
    MsgClient* clt = (MsgClient*)(clientPtr);
    if (!clt) return;

    clt->recv();

}

