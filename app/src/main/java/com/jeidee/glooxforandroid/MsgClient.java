package com.jeidee.glooxforandroid;

/**
 * Created by jeidee on 15. 4. 21..
 *
 * Purpose      : 싱글턴 객체로 XMPP client wrapper 기능 수행
 *  XMPP client 라이브러리는 gloox를 사용하며, jni를 사용해 interface한다.
 */

import android.util.Log;
import android.os.Handler;

public class MsgClient extends Thread {
    private enum ConnectState {
        disconnected,
        connecting,
        connected
    }
    static MsgClient g_instance = null;

    private Handler m_handler;
    private IMsgClientEvent m_msgClientEvent;
    private long m_clientPtr;
    private boolean m_recvThreadRunning = false;
    private ConnectState m_connectState = ConnectState.disconnected;

    private StoredLoginInfo m_loginInfo;
    private BackoffManager m_backoff;

    /*
     *  싱글턴 구현
     */
    // singleton
    public static MsgClient Instance() {
        if (g_instance == null) {
            g_instance = new MsgClient();
        }

        return g_instance;
    }

    // 싱글턴 외의 인스턴스 생성할 수 없음
    private MsgClient() {}

    // 싱글턴 명시적 초기화 함수
    public void init() {
        m_clientPtr = _newInstance();
        Log.d("MsgClient", String.format("m_clientPtr : %d", m_clientPtr));

        m_loginInfo = new StoredLoginInfo();
        m_loginInfo.init();

        m_backoff = new BackoffManager();
        m_backoff.init(10, 2, true);

        m_connectState = ConnectState.disconnected;
    }

    // 싱글턴 명시적 해제 함수
    // 싱글턴 객체를 리셋해야할 필요가 있을때 사용
    public void deInit() {
        _deleteInstance(m_clientPtr);
    }

    /*
     * 외부 인터페이스
     */

    public boolean connected() {
        if (m_connectState == ConnectState.connected)
            return true;
        else
            return false;
    }

    public void setHandler(Handler handler, IMsgClientEvent msgClientEvent) {
        m_handler = handler;
        m_msgClientEvent = msgClientEvent;
    }

    public void setLoginInfo(String jid, String pwd, String host, int port) {
        m_loginInfo.setNewInfo(jid, pwd, host, port);
        _setLoginInfo(m_clientPtr, jid, pwd, host, port);
    }

    public boolean connect() {
       return _connect(m_clientPtr);
    }

    public boolean disConnect() {
        return _disConnect(m_clientPtr);
    }

    // 메세지 수신
    // 외부에 수신 처리 쓰레드가 있을 경우 사용할 수 있다.
    public void recv() {
        _recv(m_clientPtr);
    }

    /*
     * 쓰레드 관련 인터페이스
     */
    public void stopRecv() {
        m_recvThreadRunning = false;
    }

    // 자동 재접속 구현
    //
    @Override
    public void run() {
        super.run();

        m_recvThreadRunning = true;

        while (m_recvThreadRunning) {
            try {
                Thread.sleep(10);

                if (m_connectState != ConnectState.disconnected) {
                    recv();
                    continue;
                }

                // 연결 재시도
                while (true) {
                    m_connectState = ConnectState.connecting;

                    if (connect()) {
                        break;
                    }

                    try {
                        if (m_backoff.retry() == false) {
                            Log.e("MsgClient", "최대 연결 재시도 횟수를 초과했습니다. 연결에 실패했습니다.");
                            m_recvThreadRunning = false;
                            return;
                        }
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
            } catch (InterruptedException e) {
                Log.d("MsgClient", e.toString());
                e.printStackTrace();
            }
        }

        if (m_connectState == ConnectState.connected)
            disConnect();
    }

    /*
     * gloox event handler
     */

    public void onConnect() {
        m_connectState = ConnectState.connected;
        m_handler.post(new Runnable() {
            public void run() {
                m_msgClientEvent.onConnect();
            }
        });
    }

    public void onDisconnect(final int e) {
        m_connectState = ConnectState.disconnected;

        m_handler.post(new Runnable() {
            public void run() {
                m_msgClientEvent.onDisconnect(e);
            }
        });
    }


    /*
     * JNI 관련 코드
     */
    static {
        System.loadLibrary("msg_lib");
    }

    // native 함수 선언
    public native long _newInstance();
    public native void _setLoginInfo(long clientPtr, String jid, String pwd, String host, int port);
    public native boolean _connect(long clientPtr);
    public native boolean _disConnect(long clientPtr);
    public native void _recv(long clientPtr);
    public native void _deleteInstance(long clientPtr);

    /*
     * JNI 콜백 정적 함수
     */
    public static void callbackConnect() {
        MsgClient.Instance().onConnect();
    }
    public static void callbackDisconnect(int e) {
        MsgClient.Instance().onDisconnect(e);
    }

    /*
     * 내부 함수
     */
}
