#ifndef THREAD_H
#define THREAD_H
#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <functional>
#include <string>
#include <atomic>
#include "CountDownLatch.h"
#include "noncopyable.h"
using namespace std;

class Thread : noncopyable{
public:
    typedef std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const string& name = string());
    ~Thread();
    void start();
    int join();
    bool started() const {return started_;}
    pid_t tid() const {return tid_;}
    const string& name() const {return name_;}
    static int numCreated() {return numCreated_;}
private:
    void setDefaultName();
    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    string name_;
    CountDownLatch latch_;
    static atomic_int32_t numCreated_;
};

#endif