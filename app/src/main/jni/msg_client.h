#ifndef JD_MSG_CLIENT_H__
#define JD_MSG_CLIENT_H__

#include "gloox/client.h"
#include "gloox/messagesessionhandler.h"
#include "gloox/messageeventhandler.h"
#include "gloox/messageeventfilter.h"
#include "gloox/chatstatehandler.h"
#include "gloox/chatstatefilter.h"
#include "gloox/connectionlistener.h"
#include "gloox/disco.h"
#include "gloox/message.h"
#include "gloox/gloox.h"
#include "gloox/lastactivity.h"
#include "gloox/loghandler.h"
#include "gloox/logsink.h"
#include "gloox/connectiontcpclient.h"
#include "gloox/connectionbosh.h"
#include "gloox/connectionsocks5proxy.h"
#include "gloox/connectionhttpproxy.h"
#include "gloox/messagehandler.h"
#include "gloox/rostermanager.h"

using namespace gloox;

#include <jni.h>
#include <string>

using namespace std;

namespace jd {
typedef struct {
    bool isSet;
    string jid;
    string pwd;
    string host;
    int port;

    void clear() {
        isSet = false;
        jid = "";
        pwd = "";
        host = "";
        port = 0;
    }
} LoginInfo;

class MsgClient : public MessageSessionHandler
    , ConnectionListener
    , LogHandler
    , MessageEventHandler
    , MessageHandler
    , ChatStateHandler
    , RosterListener
{
public:
    MsgClient(JavaVM* jvm, JNIEnv* env, jobject obj);
    virtual ~MsgClient();

    void setLoginInfo(string jid, string pwd, string host, int port);
    bool connect();
    bool disConnect();

    ConnectionError recv();

    // ConnectionListener
    virtual void onConnect();
    virtual void onDisconnect(ConnectionError e);
    virtual bool onTLSConnect(const CertInfo& info);

    // MessageHandler
    virtual void handleMessage(const Message& msg, MessageSession* /*session*/);

    // MessageEnvetHandler
    virtual void handleMessageEvent(const JID& from, MessageEventType event);

    // ChatStateHandler
    virtual void handleChatState(const JID& from, ChatStateType state);

    // MessageSessionHandler
    virtual void handleMessageSession(MessageSession* session);

    // LogHandler
    virtual void handleLog(LogLevel level, LogArea area, const string& message);

private:
    /*
     * jni callback functions
     */
    void callbackConnect();
    void callbackDisconnect(int e);

    MessageSession*  m_session;
    MessageEventFilter*  m_messageEventFilter;
    ChatStateFilter*     m_chatStateFilter;
    Client*          m_client;
    LoginInfo        m_loginInfo;


    JavaVM*     m_jvm;
    JNIEnv*     m_env;
    jobject     m_obj;
    jmethodID   m_cbOnConnect;
};  // class MsgClient
};  // namespace jd
#endif  // JD_MSG_CLIENT_H__