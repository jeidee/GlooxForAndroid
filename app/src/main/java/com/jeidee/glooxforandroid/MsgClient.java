package com.jeidee.glooxforandroid;

/**
 * Created by jeidee on 15. 4. 21..
 *
 * Purpose      : 싱글턴 객체로 XMPP client wrapper 기능 수행
 *  XMPP client 라이브러리는 gloox를 사용하며, jni를 사용해 interface한다.
 */

import android.util.Log;
import android.os.Handler;
import java.util.ArrayList;

public class MsgClient extends Thread {

    private enum ConnectState {
        none,
        disconnected,
        connecting,
        connected
    }
    static MsgClient g_instance = null;

    private Handler m_handler;
    private IMsgClientEvent m_msgClientEvent;
    private long m_clientPtr;
    private boolean m_recvThreadRunning = false;
    private ConnectState m_connectState = ConnectState.none;

    private StoredLoginInfo m_loginInfo;
    private BackoffManager m_backoff;
    private long m_connectionTimeoutSeconds = 30;   // 연결타임아웃 : 기본값 = 30초

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
    // @todo: 초기값은 init() 파라미터로 추가할 것
    // @todo: StoreLoginInfo 인스턴스도 init() 파라미터로 받아야 한다.
    public void init() {

        // check already inited
        if (m_connectState != ConnectState.none)
            return;

        m_clientPtr = _newInstance();
        Log.d("MsgClient", String.format("m_clientPtr : %d", m_clientPtr));

        m_loginInfo = new StoredLoginInfo();
        m_loginInfo.init();
        setLoginInfo(m_loginInfo.jid(), m_loginInfo.pwd(), m_loginInfo.host(), m_loginInfo.port());

        m_backoff = new BackoffManager();
        m_backoff.init(10, 2, true);

        m_connectState = ConnectState.disconnected;
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

    public boolean running() {
        return m_recvThreadRunning;
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
        // 접속해제된 경우에만 연결할 수 있다.
        if (m_connectState != ConnectState.disconnected)
            return false;

        m_connectState = ConnectState.connecting;

        if (!_connect(m_clientPtr)) {
            Log.e("MsgClient", "_connect() failed.");
            return false;
        }

        long elapsedTime = 0;
        long startTime = System.currentTimeMillis();

        while (elapsedTime < m_connectionTimeoutSeconds * 1000) {
            elapsedTime = System.currentTimeMillis() - startTime;

            try {
                Thread.sleep(10);
                if (!recv()) {
                    Log.e("MsgClient", "connect() > recv() failed!");
                    return false;
                }

                if (connected()) {
                    return true;
                }

            } catch (InterruptedException e) {
                e.printStackTrace();
                Log.e("MsgClient", "connect() failed.\n" + e.toString());
                return false;
            }
        }

        Log.e("MsgClient", "connect() timeout!");
        return false;
    }

    public boolean disConnect() {
        // 접속된 경우에만 끊을 수 있다.
        if (m_connectState != ConnectState.connected)
            return false;

        return _disConnect(m_clientPtr);
    }

    // 메세지 수신
    // 외부에 수신 처리 쓰레드가 있을 경우 사용할 수 있다.
    public boolean recv() {
        // 접속 중이거나 접속이 완료된 경우에만 수신이 가능하다.
        if (!(m_connectState == ConnectState.connecting ||
                m_connectState == ConnectState.connected))
            return false;

        int e = _recv(m_clientPtr);
        Log.d("MsgClient", String.format("recv() result is %d.", e));

        if (e == 0)
            return true;
        else
            return false;
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

                if (m_connectState == ConnectState.connecting ||
                        m_connectState == ConnectState.connected) {
                    recv();
                    continue;
                }

                // 연결 재시도
                while (true) {
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

    public void onRoster(final ArrayList<RosterItemData> rosters) {
        m_handler.post(new Runnable() {
            public void run() {
                m_msgClientEvent.onRoster(rosters);
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
    public native int _recv(long clientPtr);
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
    public static void callbackRoster(ArrayList<RosterItemData> rosters) {
        MsgClient.Instance().onRoster(rosters);
    }

    /*
     * 내부 함수
     */
}
