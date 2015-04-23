#ifndef THREAD_LOCK_H__
#define THREAD_LOCK_H__

#include <pthread.h>

namespace jd {

class ThreadLock
{
public:
    ThreadLock();
    virtual ~ThreadLock();

    void Lock();
    void Unlock();
private:
    pthread_mutex_t mutexlock;
};  // class ThreadLock

ThreadLock::ThreadLock()
{
    // init lock here
    pthread_mutex_init(&mutexlock, 0);
}

ThreadLock::~ThreadLock()
{
    // deinit lock here
    pthread_mutex_destroy(&mutexlock);
}
void ThreadLock::Lock()
{
    // lock
    pthread_mutex_lock(&mutexlock);
}
void ThreadLock::Unlock()
{
    // unlock
    pthread_mutex_unlock(&mutexlock);
}

class AutoLock
{
public:
    AutoLock(ThreadLock& lock);
    virtual ~AutoLock();

private:
    ThreadLock m_lock;

};

AutoLock::AutoLock(ThreadLock& lock) : m_lock(lock)
{
    m_lock.Lock();
}

AutoLock::~AutoLock()
{
    m_lock.Unlock();
}



};  // namespace jd
#endif  // THREAD_LOCK_H__