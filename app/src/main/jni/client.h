#ifndef CLIENT_H__
#define CLIENT_H__

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
#include "gloox/connectionsocks5proxy.h"
#include "gloox/connectionhttpproxy.h"
#include "gloox/messagehandler.h"

using namespace gloox;

#include <jni.h>

#include <string>

using namespace std;

namespace jd {

class client : public MessageSessionHandler
    , ConnectionListener
    , LogHandler
    , MessageEventHandler
    , MessageHandler
    , ChatStateHandler
{
public:
    client(JNIEnv* env, jobject obj);
    virtual ~client();

    bool connect(string jid, string pwd, string host, int port);
    bool disConnect();

    ConnectionError recv();

    // ConnectionListener
    virtual void onConnect();
    virtual void onDisconnect();
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
    Client*          m_client;
    MessageSession*  m_session;
    MessageEventFilter*  m_messageEventFilter;
    ChatStateFilter*     m_chatStateFilter;

    JNIEnv*     m_env;
    jobject     m_obj;
};   // class client

};  // namespace sg

#endif  // CLIENT_H__