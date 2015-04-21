/**
 * Created by jeidee on 15. 4. 21..
 */
package com.jeidee.glooxforandroid;


import android.util.Log;

public class NativeCall {
    static {
        System.loadLibrary("my_lib");
    }

    public native void connectTest();

    public void testCallback(String v) {
        Log.d("jni", "Callback from jni " + v);
    }

}
