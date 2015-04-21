#include "message_test.h"

MessageTest::MessageTest(JNIEnv *env, jobject obj) :
      m_session( 0 )
    , m_messageEventFilter( 0 )
    , m_chatStateFilter( 0 )
    , m_env(env)
    , m_obj(obj)
{
}

MessageTest::~MessageTest()
{
}
