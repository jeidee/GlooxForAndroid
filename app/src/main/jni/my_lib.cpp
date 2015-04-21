#include "com_jeidee_glooxforandroid_NativeCall.h"

#include "message_test.h"

JNIEXPORT void JNICALL Java_com_jeidee_glooxforandroid_NativeCall_connectTest (JNIEnv *env, jobject obj) {
//    static jmethodID cb = NULL;
//    jclass cls = env->GetObjectClass(obj);
//
//    if (cb == NULL) {
//        cb = env->GetMethodID(cls, "testCallback", "(Ljava/lang/String;)V");
//        if (cb == NULL) return;
//    }
//
//    env->CallVoidMethod(obj, cb, env->NewStringUTF("[C->J] testCallback callback"));

    MessageTest *r = new MessageTest(env, obj);
    r->start();
    delete( r );

}