#include "client.h"

#include <android/log.h>

#define  LOG_TAG    "jd::client"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)


namespace jd {

    client::client(JNIEnv* env, jobject obj) :
               m_env(env)
             , m_obj(obj)
             , m_client(NULL)
             , m_session(NULL)
             , m_messageEventFilter(NULL)
             , m_chatStateFilter(NULL) {

    }

    client::~client() {

    }

    bool client::connect(string jid, string pwd, string host, int port) {

    }

    bool client::disConnect() {

    }

    ConnectionError client::recv() {

    }

    // ConnectionListener
    void client::onConnect() {

    }

    void client::onDisconnect() {

    }

    bool client::onTLSConnect(const CertInfo& info) {

    }

    // MessageHandler
    void client::handleMessage(const Message& msg, MessageSession* /*session*/) {

    }

    // MessageEnvetHandler
    void client::handleMessageEvent(const JID& from, MessageEventType event) {

    }

    // ChatStateHandler
    void client::handleChatState(const JID& from, ChatStateType state) {

    }

    // MessageSessionHandler
    void client::handleMessageSession(MessageSession* session) {

    }

    // LogHandler
    void client::handleLog(LogLevel level, LogArea area, const string& message) {

    }
};