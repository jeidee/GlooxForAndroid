#include "msg_client.h"
#include "jni_util.h"

#include <android/log.h>

#define  LOG_TAG    "jd::MsgClient"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


namespace jd {

MsgClient::MsgClient(JavaVM* jvm, JNIEnv* env, jobject obj) :
           m_jvm(jvm)
         , m_env(env)
         , m_obj(obj)
         , m_client(NULL)
         , m_session(NULL)
         , m_messageEventFilter(NULL)
         , m_chatStateFilter(NULL) {

    m_loginInfo.clear();
}

MsgClient::~MsgClient() {

}

void MsgClient::setLoginInfo(string jid, string pwd, string host, int port) {
    m_loginInfo.isSet = true;
    m_loginInfo.jid = jid;
    m_loginInfo.pwd = pwd;
    m_loginInfo.host = host;
    m_loginInfo.port = port;
}

bool MsgClient::connect() {
    if (m_loginInfo.isSet == false) {
        LOGE("m_loginInfo.isSet == false.");
        return false;
    }

    LOGD("jid = %s, pwd = %s, host = %s, port = %d"
        , m_loginInfo.jid.c_str()
        , m_loginInfo.pwd.c_str()
        , m_loginInfo.host.c_str()
        , m_loginInfo.port);

    JID j(m_loginInfo.jid.c_str());
    m_client = new Client(j, m_loginInfo.pwd.c_str());

    ConnectionTCPClient* conn = new ConnectionTCPClient( m_client, m_client->logInstance()
        , m_loginInfo.host.c_str()
        , m_loginInfo.port);

    m_client->setConnectionImpl(conn);

    m_client->registerConnectionListener(this);
    m_client->registerMessageSessionHandler(this, 0);
    m_client->disco()->setVersion("jd", GLOOX_VERSION, "android");
    m_client->disco()->setIdentity("client", "jd");
    m_client->disco()->addFeature(XMLNS_CHAT_STATES);

    m_client->logInstance().registerLogHandler(LogLevelDebug, LogAreaAll, this);

    return m_client->connect(false);
}

bool MsgClient::disConnect() {
    m_client->disconnect();

}

ConnectionError MsgClient::recv() {
    ConnectionError e = m_client->recv();
    return e;
}

// ConnectionListener
void MsgClient::onConnect() {
    LOGD("connected!!!");

    callbackConnect();

//    m_env->CallVoidMethod(m_obj, m_cbOnConnect, m_env->NewStringUTF("[C->J] onTest recv called"));
//
//
//    static jmethodID cb = NULL;
//    jclass cls = m_env->GetObjectClass(m_obj);
//
//    if (cb == NULL) {
//        cb = m_env->GetMethodID(cls, "onConnect", "()V");
//    }
//
//    m_env->CallVoidMethod(m_obj, cb);
//
    m_env = getJNIEnv(m_jvm);
//    callbackJavaMethod(m_env, m_obj, "onConnect", "()V");
}

void MsgClient::onDisconnect(ConnectionError e) {
    LOGD("disconnected: %d", e);

    if (e == ConnAuthenticationFailed) {
        LOGE("auth failed. reason: %d", m_client->authError());
    }

    callbackDisconnect((int)e);
}

bool MsgClient::onTLSConnect(const CertInfo& info) {
    time_t from(info.date_from);
    time_t to(info.date_to);

    LOGD( "status: %d\nissuer: %s\npeer: %s\nprotocol: %s\nmac: %s\ncipher: %s\ncompression: %s\n"
         "from: %s\nto: %s\n",
         info.status, info.issuer.c_str(), info.server.c_str(),
         info.protocol.c_str(), info.mac.c_str(), info.cipher.c_str(),
         info.compression.c_str(), ctime( &from ), ctime( &to ) );

    return true;
}

// MessageHandler
void MsgClient::handleMessage(const Message& msg, MessageSession* /*session*/) {
    LOGD( "type: %d, subject: %s, message: %s, thread id: %s\n", msg.subtype(),
         msg.subject().c_str(), msg.body().c_str(), msg.thread().c_str() );

//        string re = "You said:\n> " + msg.body() + "\nI like that statement.";
//        string sub;
//        if( !msg.subject().empty() )
//            sub = "Re: " +  msg.subject();
//
//        m_messageEventFilter->raiseMessageEvent( MessageEventDisplayed );
//        sleep( 1 );
//        m_messageEventFilter->raiseMessageEvent( MessageEventComposing );
//        m_chatStateFilter->setChatState( ChatStateComposing );
//        sleep( 2 );
//        m_session->send( re, sub );
//
//        if( msg.body() == "quit" )
//            j->disconnect();
}

// MessageEnvetHandler
void MsgClient::handleMessageEvent(const JID& from, MessageEventType event) {
    LOGD( "received event: %d from: %s\n", event, from.full().c_str());
}

// ChatStateHandler
void MsgClient::handleChatState(const JID& from, ChatStateType state) {
    LOGD( "received state: %d from: %s\n", state, from.full().c_str() );
}

// MessageSessionHandler
void MsgClient::handleMessageSession(MessageSession* session) {
    LOGD( "got new session\n");
    // this example can handle only one session. so we get rid of the old session
    m_client->disposeMessageSession(m_session);
    m_session = session;
    m_session->registerMessageHandler(this);
    m_messageEventFilter = new MessageEventFilter(m_session);
    m_messageEventFilter->registerMessageEventHandler(this);
    m_chatStateFilter = new ChatStateFilter(m_session);
    m_chatStateFilter->registerChatStateHandler(this);
}

// LogHandler
void MsgClient::handleLog(LogLevel level, LogArea area, const string& message) {
    LOGD( "log: level: %d, area: %d, %s\n", level, area, message.c_str());
}

/*
 * jni callback functions
 */

void MsgClient::callbackConnect(){
    JniMethodInfo methodInfo;
    if (! getStaticMethodInfo(m_jvm, methodInfo, "com/jeidee/glooxforandroid/MsgClient", "callbackConnect", "()V"))
    {
        return;
    }

    //jstring stringArg = methodInfo.env->NewStringUTF(value);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
    //msg.methodInfo.env->DeleteLocalRef(stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}

void MsgClient::callbackDisconnect(int e){
    JniMethodInfo methodInfo;
    if (! getStaticMethodInfo(m_jvm, methodInfo, "com/jeidee/glooxforandroid/MsgClient", "callbackDisconnect", "(I)V"))
    {
        return;
    }

    //jstring stringArg = methodInfo.env->NewStringUTF(value);
    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, e);
    //msg.methodInfo.env->DeleteLocalRef(stringArg);
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
}


};  // namespace jd