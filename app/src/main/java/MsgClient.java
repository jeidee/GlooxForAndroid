/**
 * Created by jeidee on 15. 4. 21..
 */
package com.jeidee.glooxforandroid;


import android.util.Log;

public class MsgClient extends Thread {
    static MsgClient g_instance = null;

    public static MsgClient Instance() {
        if (g_instance == null) {
            g_instance = new MsgClient();
        }

        return g_instance;
    }

    private IMsgClientEvent m_msgClientEvent;
    private long m_clientPtr;
    private boolean m_recvThreadRunning = false;

    private MsgClient() {}

    public void setMsgClientEvent (IMsgClientEvent msgClientEvent) {
        m_msgClientEvent = msgClientEvent;
    }

    public IMsgClientEvent getMsgClientEvent() {
        return m_msgClientEvent;
    }

    /*
     * MsgClient interfaces for java
     */

    public void init() {
        m_clientPtr = _newInstance();
        Log.d("MsgClient", String.format("m_clientPtr : %d", m_clientPtr));
    }

    public void deInit() {
        _deleteInstance(m_clientPtr);
    }

    public boolean connect(String jid, String pwd, String host, int port) {
        return _connect(m_clientPtr, jid, pwd, host, port);
    }

    public boolean disConnect() {
        return _disConnect(m_clientPtr);
    }

    public void recv() {
        _recv(m_clientPtr);
    }

    /*
     * for Threads
     */
    public void stopRecv() {
        m_recvThreadRunning = false;
    }

    @Override
    public void run() {
        m_recvThreadRunning = true;
        super.run();

        while (m_recvThreadRunning) {
            try {
                recv();
                Thread.sleep(10);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

    }


    /*
     * jni native functions
     */
    static {
        System.loadLibrary("msg_lib");
    }

    public native long _newInstance();
    public native boolean _connect(long clientPtr, String jid, String pwd, String host, int port);
    public native boolean _disConnect(long clientPtr);
    public native void _recv(long clientPtr);
    public native void _deleteInstance(long clientPtr);

    /*
     * callback from jni
     */
    public static void onConnect() {
        MsgClient.Instance().getMsgClientEvent().onConnect();
    }
    public static void onDisconnect(int e) {
        MsgClient.Instance().getMsgClientEvent().onDisconnect(e);
    }
    public static void onTest(String V) {
        Log.d("MsgClient", V);
    }

}
